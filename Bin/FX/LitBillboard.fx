//textures with lighting applied
#include "LightHelper.fx"

cbuffer cbPerFrame
{
	//array of directional lights
	DirectionalLight gDirLights[3];
	float gDirLightCount = 0.0f;
	//array of point lights
	PointLight gPointLights[10];
	float gPointLightCount = 0.0f;
	//array of spot lights
	SpotLight gSpotLights[10];
	float gSpotLightCount = 0.0f;

	//camera (eye) position
	float3 gEyePosW;

	//fog vars
	bool	gFogEnabled = false;//is fog enabled?
	float	gFogStart;			//dist where fog begins
	float	gFogRange;			//range of fog from begin to full
	float4	gFogColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	//float values
	float gColumn = 0.0f;				//current spritesheet column (0-indexed)
	float gNumCols = 1.0f;				//number of spritesheet columns
	float gFlipValue = 1.0f;			//flip texture (-1.0f) or not (1.0f)
	float gLerpValue = 0.0f;			//lerp between texture and alt color
	float gOpacityValue = 1.0f;			//opacity value
	//alternative colour (to lerp to)
	float4 gAltColor = float4(1.0f, 1.0f, 1.0f, 0.0f);

};

//constant buffer - per object 
cbuffer cbPerObject
{
	//world (for world space transforms)
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;	//for normal transforms
	float4x4 gWorldViewProj;		//for transform to world with camera view / proj space

	Material gMaterial;				//for reacting with light
	bool gUseTexture = true;		//set for using textures (could adjust this?)

									//ambient light color
	float4 gAmbientColor : AMBIENT = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//ambient light intensity
	float gAmbientIntensity = 1.0f;
};


// Nonnumeric values cannot be added to a cbuffer.
//this is the texture / color / diffuse map for display
Texture2D gDiffuseMap;

//linear mipmapping and clamp texture to prevent
//edge issues
SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

//struct for vertex input
struct VertexIn
{
	//x,y,z position in world space
	float3 PosL : POSITION;
	//normal x, y, z (facing direction)
	float3 NormalL : NORMAL;
	//texture u, v
	float2 Tex : TEXCOORD;
	//Tangent x, y, z, w	(for 'surface' space)
	float4 TangentL : TANGENT;
};

//struct for vertex output / pixel input
struct VertexOut
{
	//SV is System Value - used for final outputs
	//of 'processed' vertices
	float4 PosH : SV_POSITION;		//position (full transformation)
	float3 PosW       : POSITION;	//position (world transformation)
	float3 NormalW    : NORMAL;		//normal (world transformation)
	float4 TangentW   : TANGENT;
	//texture u, v
	float2 Tex : TEXCOORD0;			//tex coords
};


//Vertex shader
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	//----------------------------------------------------------
	//the float4 here adds a fourth dimension with a value of 1.0f
	//to ensure the multiplication with vertices is valid
	//this is standard and will not change
	//the xyz at the end is so it only stores those dimensions in the output position
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	//Transform to homogenous clip space
	//(what we can see from our camera)
	//(multiply vertex by matrix)
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	vout.TangentW = mul(vin.TangentL, gWorld);

	//U co-ordinates - horizontal texture stuff
	//interesting breakdown - based on current portion of the horizontal texture
	//works out which column we are on
	vout.Tex[0] = ((1.0f / gNumCols) * gColumn + vin.Tex[0] / gNumCols);

	//if the flip value is -1.0f...
	if (gFlipValue == -1.0f)
	{
		//then we flip the texture by subtracting from 1.0f
		vout.Tex[0] = 1.0f - vout.Tex[0];
	}

	//V co-ordinates - vertical texture position
	vout.Tex[1] = vin.Tex[1];

	//this returns the VertexOut struct so it can then be used by the
	//Pixel Shader
	return vout;
}


//pixel shader
float4 PS(VertexOut pin) : SV_TARGET
{
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

	// The toEye (camera) vector is used in lighting.
	float3 toEye = gEyePosW - pin.PosW;

	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;

	// Default to multiplicative identity.
	float4 texColor = float4(1, 1, 1, 1);
	//Sample texture.
	texColor = gDiffuseMap.Sample(samLinear, pin.Tex);

	// Discard pixel if texture alpha < 0.1.  Note that we do this
	// test as soon as possible so that we can potentially exit the shader 
	// early, thereby skipping the rest of the shader code.
	clip(texColor.a - 0.1f);

	//
	// Lighting.
	//
	float4 litColor = texColor;

	//main lighting if statement - if no lights enabled, 
	//do no lighting calcs and just use ambient values with texture
	if (gDirLightCount > 0 || gPointLightCount > 0 || gSpotLightCount > 0)
	{
		// Start with a sum of zero. 
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

		//directional lights checking
		if (gDirLightCount > 0)
		{
			// Sum the light contribution from each light source.  
			[unroll]	//shader optimisation for loops
			for (int i = 0; i < gDirLightCount; ++i)
			{
				//use the compute directional light function to get light values
				float4 A, D, S;
				ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye,
					A, D, S);
				//add the ambient, diffuse and specular contributions from each light
				ambient += A;
				diffuse += D;
				spec += S;
			}
		}

		if (gPointLightCount > 0)
		{
			// Sum the light contribution from each light source.  
			[unroll]	//shader optimisation for loops
			for (int i = 0; i < gPointLightCount; ++i)
			{
				//use the compute point light function to get light values
				float4 A, D, S;
				ComputePointLight(gMaterial, gPointLights[i], pin.PosW, pin.NormalW, toEye,
					A, D, S);
				//add the ambient, diffuse and specular contributions from each light
				ambient += A;
				diffuse += D;
				spec += S;
			}
		}

		if (gSpotLightCount > 0)
		{
			// Sum the light contribution from each light source.  
			[unroll]
			for (int i = 0; i < gSpotLightCount; ++i)
			{
				float4 A, D, S;
				ComputeSpotLight(gMaterial, gSpotLights[i], pin.PosW, pin.NormalW, toEye,
					A, D, S);

				ambient += A;
				diffuse += D;
				spec += S;
			}
		}

		//the final lit color is based on the sum of the lighting contributions
		litColor = texColor*(ambient + diffuse) + spec;
	}
	else
	{
		//light texture by ambient values
		litColor = litColor  * gAmbientColor * gAmbientIntensity;
	}

	//
	// Fogging
	//
	if (gFogEnabled)
	{
		//based on distance, start and range of fog, what is the lerp factor?
		//(saturate clamps between 0.0f and 1.0f)
		float fogLerp = saturate((distToEye - gFogStart) / gFogRange);

		// Blend the fog color and the lit color.
		litColor = lerp(litColor, gFogColor, fogLerp);
	}

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * texColor.a;

	return litColor;
}

//name of technique so it can be managed by our Effects class
technique11 LitBillboardTech
{
	//onlu one pass; multiple passes can be used for more complex effects
	pass P0
	{
		//vertex shader and pixel shader set to above functions
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}

