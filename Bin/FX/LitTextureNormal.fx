//textures with lighting applied
//call in light helper methods and structs
#include "LightHelper.fx"
//constant buffer per frame
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
	
	//post effect vars
	bool gCelShaderEnabled = false; 
	bool gInvertEnabled = false; 
	
	//water effect vars
	bool gWaterShaderEnabled = false; 
	float3 gRefractRatioColor = float3(0.752f, 0.752f, 0.752f); 
	
	//fresnel vars
	float gFresnelBias = 0.5f; 
	float gFresnelPower = 2.0f; 
	float gFresnelScale = 15.0f; 
	
	//lerp colour with fresnel effect
	float gWaterTexLerp = 0.5f; 

};

cbuffer cbSkinned
{
	float4x4 gBoneTransforms[192];
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

	float gOpacityValue = 0.5f;
};



// Nonnumeric values cannot be added to a cbuffer.
//this is the texture / color / diffuse map for display
Texture2D gDiffuseMap;
Texture2D gNormalMap;

//this is here in case reflection or refraction are needed
TextureCube gReflectCubeMap; 

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC; 
	MaxAnisotropy = 4; 
	
	AddressU = WRAP; 
	AddressV = WRAP; 
	}; 

//linear mipmapping and clamp texture to prevent
//edge issues
SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

//skinned vertex in
struct SkinnedVertexIn
{
	float3 PosL       : POSITION;
	float3 NormalL    : NORMAL;
	float2 Tex        : TEXCOORD;
	float4 TangentL   : TANGENT;
	float3 Weights    : WEIGHTS;
	uint4 BoneIndices : BONEINDICES;
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

//Vertex shader for skinned meshes
VertexOut SkinnedVS(SkinnedVertexIn vin)
{
	VertexOut vout;

	// Init array or else we get strange warnings about SV_POSITION.
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//limit to 2 bones following experimentation with models
	weights[0] = vin.Weights.x;
	weights[1] = 1.0f - weights[0];

	//set initial local position / normal / tangent
	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);
	float3 tangentL = float3(0.0f, 0.0f, 0.0f);
	//loop through the two bone weights
	for (int i = 0; i < 2; ++i)
	{
		// Assume no nonuniform scaling when transforming normals, so 
		// that we do not have to use the inverse-transpose
		//here we add to the values the multiplication result of the weight * position / normal / tan
		// * the bone transform matrix at the index the vertex realtes to for the weight
		posL += weights[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndices[i]]).xyz;
		normalL += weights[i] * mul(vin.NormalL, (float3x3)gBoneTransforms[vin.BoneIndices[i]]);
		tangentL += weights[i] * mul(vin.TangentL.xyz, (float3x3)gBoneTransforms[vin.BoneIndices[i]]);
	}

	// Transform to world space.
	vout.PosW = mul(float4(posL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(normalL, (float3x3)gWorldInvTranspose);
	vout.TangentW = float4(mul(tangentL, (float3x3)gWorld), vin.TangentL.w);

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(posL, 1.0f), gWorldViewProj);

	//set texture co-ordinates
	vout.Tex = vin.Tex;

	//this returns the VertexOut struct so it can then be used by the
	//Pixel Shader
	return vout;
}

//Vertex shader
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	//the float4 here adds a fourth dimension with a value of 1.0f
	//to ensure the multiplication with vertices is valid
	//this is standard and will not change
	//the xyz at the end is so it only stores those dimensions in the output position
	//these are in world space for lighting calculations
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	//Transform to homogenous clip space
	//(what we can see from our camera)
	//(multiply vertex by matrix)
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	//set texture co-ordinates
	vout.Tex = vin.Tex;
	//tangent
	vout.TangentW = mul(vin.TangentL, gWorld);

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
	// Normal mapping
	//

	float3 normalMapSample = gNormalMap.Sample(samLinear, pin.Tex).rgb;
	float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);

	//
	// Lighting.
	//
	float4 litColor = texColor;
	
	//FOR WATER -------------------------------
	//lerp the reflection and refraction before lighting
	if(gWaterShaderEnabled)
	{
		//compute reflection
		float3 incident = -toEye; 
		
		//this is fresnel calculation for reflection
		float reflectionFactor = gFresnelBias + 
			gFresnelScale * pow((1.0f + dot(incident, pin.NormalW)), gFresnelPower); 
			
		float3 reflectionVector = reflect(incident, pin.NormalW); 
		float4 reflectedColor = gReflectCubeMap.Sample(samAnisotropic, reflectionVector);
		
		float3 TRed = refract(incident, pin.NormalW, gRefractRatioColor.x); 
		float3 TGreen = refract(incident, pin.NormalW, gRefractRatioColor.y);
		float3 TBlue = refract(incident, pin.NormalW, gRefractRatioColor.z);
		
		float4 refractedColor; 
		refractedColor.r = gReflectCubeMap.Sample(samAnisotropic, TRed).r; 
		refractedColor.g = gReflectCubeMap.Sample(samAnisotropic, TGreen).g; 
		refractedColor.b = gReflectCubeMap.Sample(samAnisotropic, TBlue).b; 
		refractedColor.a = 1.0f; 
		
		//compute final colour
		float4 fresnelColor = lerp(refractedColor, reflectedColor, reflectionFactor); 
		
		//and blend with surface texture for lerp amount given 
		texColor = lerp(fresnelColor, texColor, gWaterTexLerp); 
	}
		
		
		
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
				ComputeDirectionalLight(gMaterial, gDirLights[i], bumpedNormalW, toEye,
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
				ComputePointLight(gMaterial, gPointLights[i], pin.PosW, bumpedNormalW, toEye,
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
			[unroll]	//shader optimisation for loops
			for (int i = 0; i < gSpotLightCount; ++i)
			{
				//use the compute point light function to get light values
				float4 A, D, S;
				ComputeSpotLight(gMaterial, gSpotLights[i], pin.PosW, bumpedNormalW, toEye,
					A, D, S);
				//add the ambient, diffuse and specular contributions from each light
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

	

	//CEL SHADING -------------------
	
	if (gCelShaderEnabled){
	
	float4 celColor = float4(0.0f, 0.0f, 0.0f, 0.0f); 
	
	CalculateCelShading(litColor.r, litColor.g, litColor.b, 
						celColor.r, celColor.g, celColor.b); 
						
	litColor = celColor; 
	}
	
	//INVERT --------------------
	if(gInvertEnabled)
	{
		float4 invColor = float4(0.0f, 0.f, 0.0f, 0.0f); 
		
		CalculateInverse(litColor.r, litColor.g, litColor.b,
			invColor.r, invColor.g, invColor.b); 
			
		litColor = invColor; 
	}
	
	
				//ALTERNATE CEL SHADING & INVERT
				//float3 celColor = float3 (litColor.r, litColor.g, litColor.b);

				//CelShadeClamping(litColor.r, litColor.g, litColor.b,
				//	celColor.r, celColor.g, celColor.b); 
				//END OF ALTERNATE CEL SHADING

				//INVERT
				//float3 invColor = float3 (litColor.r, litColor.g, litColor.b);

				//Invert(litColor.r, litColor.g, litColor.b,
				//	invColor.r, invColor.g, invColor.b);

				//END OF INVERT

	// Common to take alpha from diffuse material and texture.
	
	litColor.a = gMaterial.Diffuse.a * texColor.a;
	
	return float4(litColor.rgb, gOpacityValue);
	}

	//name of technique so it can be managed by our Effects class
	technique11 LitTexNormalTech
	{
	//only one pass; multiple passes can be used for more complex effects
	pass P0
	{
		//vertex shader and pixel shader set to above functions
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}

//name of technique so it can be managed by our Effects class
technique11 LitTexNormSkinnedTech
{
	//only one pass; multiple passes can be used for more complex effects
	pass P0
	{
		//vertex shader and pixel shader set to above functions
		SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}

