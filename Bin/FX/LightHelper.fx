//FX file header to help with common lighting operations

//direcional light (from undefined distance - e.g. the sun)
struct DirectionalLight
{
	float4 Ambient;		//ambient color (how ambient light is affected)
	float4 Diffuse;		//diffuse color (for 'scatter')
	float4 Specular;	//specular color (for shines)
	float3 Direction;	//direction light is facing
	float pad;			//pads the struct so arrays can be sent
};

//point light (from a position, glow in all directions)
struct PointLight
{
	float4 Ambient;		//ambient color (how ambient light is affected)
	float4 Diffuse;		//diffuse color (for 'scatter')
	float4 Specular;	//specular color (for shines)

	float3 Position;	//position of light source
	float Range;		//how far the light reaches

	float3 Att;			//attenuation (drop-off of intensity)
	float pad;			//padding simply to fit GPU memory
};

//point light (from a position, shine cone in direction)
struct SpotLight
{
	float4 Ambient;		//ambient color (how ambient light is affected)
	float4 Diffuse;		//diffuse color (for 'scatter')
	float4 Specular;	//specular color (for shines)

	float3 Position;	//position of light source
	float Range;		//how far the light reaches

	float3 Direction;	//direction light is facing
	float Spot;			//cone radius

	float3 Att;			//attenuation (drop-off of intensity)
	float pad;			//padding simply to fit GPU memory
};

//material of surface
//affects interaction with light sources
struct Material
{
	float4 Ambient;		//ambient color (how surface is affected by ambient light)
	float4 Diffuse;		//diffuse color (how color is spread over the surface)
	float4 Specular;	//shininess - w = SpecPower
	float4 Reflect;		//reflect value - is surface reflective?
};



//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a directional light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeDirectionalLight(Material mat, DirectionalLight L,
	float3 normal, float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -L.Direction;

	// Add ambient term.
	ambient = mat.Ambient * L.Ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light (hence dot product).
	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching (optimised)
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		//here we take the lightvec (negated) and surface normal
		//using the reflect function to get a direction
		float3 v = reflect(-lightVec, normal);
		//the specular power considers the dot product to the eye / camera
		//to set the 'shininess'
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
		//here we then multiply the light / material and factors to get final values
		//of colour from the light for this pixel
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}
}

//---------------------------------------------------------------------------------------
// Transforms a normal map sample to world space.
//---------------------------------------------------------------------------------------
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float4 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1] to work in world space.
	float3 normalT = 2.0f*normalMapSample - 1.0f;

	//Build orthonormal basis (for transforming from surface / tangent space)
	//(better explained in diagrams!)
	float3 N = unitNormalW;	//normal in world space
	float3 T = normalize(tangentW.xyz - dot(tangentW.xyz, N)*N); //tangent
	float3 B = tangentW.w * cross(N, T);	//bitangent

	//TBN matrix (tangent / bitangent / normal)
	float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
	float3 bumpedNormalW = mul(normalT, TBN);
	//return the normal at this position in world space
	return bumpedNormalW;
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a point light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = L.Position - pos;

	// The distance from surface to light.
	float d = length(lightVec);

	// Range test (no light if outside range).
	if (d > L.Range)
		return;

	// Normalize the light vector.
	lightVec /= d;

	// Ambient term.
	ambient = mat.Ambient * L.Ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		//here we take the lightvec (negated) and surface normal
		//using the reflect function to get a direction
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
		//here we then multiply the light / material and factors to get final values
		//of colour from the light for this pixel
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}

	//get attenuation denominator

	// Attenuate based on distance and facing direction
	float att = 1.0f / dot(L.Att, float3(1.0f, d, d*d));

	//update diffuse and specular values based on attenuation
	//diffuse *= att;
	spec *= att;

	//new diffuse calc!!!!
	//we get a percentage of the intensity outside of the range and interpolate
	//brilliant right????
	//Yeah, I know -- we'll see lol!!!
	float lerpVal = d / L.Range;
	float4 noLight = float4(0.0f, 0.0f, 0.0f, 1.0f);

	diffuse = lerp(diffuse, noLight, lerpVal);


}

// -------------------------------------------------------------------------------------- -
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a spotlight.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = L.Position - pos;

	// The distance from surface to light.
	float d = length(lightVec);

	// Range test.
	if (d > L.Range)
		return;

	// Normalize the light vector.
	lightVec /= d;

	// Ambient term.
	ambient = mat.Ambient * L.Ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}

	// Scale by spotlight factor and attenuate.
	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);

	// Scale by spotlight factor and attenuate.
	float att = spot / dot(L.Att, float3(1.0f, d, d*d));

	ambient *= spot;
	diffuse *= att;
	spec *= att;
}

//------------------------------------------------
//Cel Shading FX
//------------------------------------------------

void CalculateCelShading(float inRed, float inGreen, float inBlue, 
	out float outRed, out float outGreen, out float outBlue)
	
{
	//initialise output vars
	outRed = 0.0f; 
	outGreen = 0.0f; 
	outBlue = 0.0f; 
	
	//clamp red values
	if (inRed < 0.1f)
		outRed = 0.0f; 
	if (inRed >= 0.1f && inRed <= 0.25f)
		outRed = 0.25f;
	if (inRed >= 0.25f && inRed <= 0.5f)
		outRed = 0.5f; 
	if (inRed >= 0.5f && inRed <= 0.75f)
		outRed = 0.75f; 
	if (inRed > 0.75)
		outRed = 1.0f; 

	//clamp green values
	if (inGreen < 0.1f)
		outGreen = 0.0f; 
	if (inGreen >= 0.1f && inGreen <= 0.25f)
		outGreen = 0.25f;
	if (inGreen >= 0.25f && inGreen <= 0.5f)
		outGreen = 0.5f; 
	if (inGreen >= 0.5f && inGreen <= 0.75f)
		outGreen = 0.75f; 
	if (inGreen > 0.75)
		outGreen = 1.0f; 
		
	//clamp blue values
	if (inBlue < 0.1f)
		outBlue = 0.0f; 
	if (inBlue >= 0.1f && inBlue <= 0.25f)
		outBlue = 0.25f;
	if (inBlue >= 0.25f && inBlue <= 0.5f)
		outBlue = 0.5f; 
	if (inBlue >= 0.5f && inBlue <= 0.75f)
		outBlue = 0.75f; 
	if (inBlue > 0.75)
		outBlue = 1.0f; 
}

//------------------------
//INVERSE COLOURS FX
//------------------------	

void CalculateInverse(float inRed, float inGreen, float inBlue, 
				out float outRed, out float outGreen, out float outBlue)
				
{
	//initialise output vars
	outRed = 0.0f; 
	outGreen = 0.0f; 
	outBlue = 0.0f; 
	
	outRed = 1.0f - inRed; 
	outGreen = 1.0f - inGreen; 
	outBlue = 1.0f - inBlue; 
	}




//--------------ALTERNATE CEL AND INVERSE SHADER
//--------------UNUSED IN CODE BUT CAN BE SUBSTITUTED AS ALTERNATIVE


//void CelShadeClamping(float r, float g, float b,
//	out float celR, out float celG, out float celB) {

//	celR = 0.0f; 
//	celG = 0.0f; 
//	celB = 0.0f;


	//RED CALCS
//	if (r < 0.1f) celR = 0.0f; //if less than 0.1, clamp to 0
//	if (r >= 0.1f && r >= 0.25f) celR = 0.25f;
//	if (r < 0.25f && r >= 0.5f) celR = 0.5f;
//	if (r < 0.5f && r >= 0.75f) celR = 0.75f;
//	if (r >= 0.75f) celR = 1.0f;

	//GREEN CALCS
//	if (g < 0.1f) celG = 0.0f; //if less than 0.1, clamp to 0
//	if (g >= 0.1f && g >= 0.25f) celG = 0.25f;
//	if (g < 0.75f && g >= 0.5f) celG = 0.5f;
//	if (g < 1.0f && g >= 0.75f) celG = 0.75f;
//	if (g >= 0.75f) celG = 1.0f;

	//BLUE CALCS
//	if (b < 0.1f) celB = 0.0f; //if less than 0.1, clamp to 0
//	if (b >= 0.1f && b >= 0.25f) celB = 0.25f;
//	if (b < 0.75f && b >= 0.5f) celB = 0.5f;
//	if (b < 1.0f && b >= 0.75f) celB = 0.75f;
//	if (b >= 0.75f) celB = 1.0f;
//}

//INVERSE FUNCTION
//void Invert(float r, float g, float b,
//	out float invR, out float invG, out float invB) {

//	invR = 1.0f - r;
//	invG = 1.0f - g;
//	invB = 1.0f - b;
//}

//----------------END OF ALTERNATIVE CODE