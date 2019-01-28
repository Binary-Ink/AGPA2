//constant buffer per object (changes per sprite)
cbuffer cbPerObject
{
	//matrices for transformations
	float4x4 gWorld;				
	float4x4 gWorldViewProj;

	//float values
	float gColumn = 0.0f;				//current spritesheet column (0-indexed)
	float gNumCols = 1.0f;				//number of spritesheet columns
	float gFlipValue = 1.0f;			//flip texture (-1.0f) or not (1.0f)
	float gLerpValue = 0.0f;			//lerp between texture and alt color
	float gOpacityValue = 1.0f;			//opacity value

	//alternative colour (to lerp to)
	float4 gAltColor = float4(1.0f, 1.0f, 1.0f, 0.0f);

};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gSpriteTex;

//linear mipmapping and clamp texture to prevent
//edge issues
SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

//here comes the structs...
//struct for input into vertex shader
struct VertexShaderIn
{
	//position x, y, z
	float3 PosL    : POSITION;
	//texture u, v
	float2 Tex     : TEXCOORD;
};

//struct for output from vertex shader
//(input into pixel shader)
struct VertexShaderOut
{
	//position x, y, z
	float4 PosH       : SV_POSITION;
	//texture u, v
	float2 Tex        : TEXCOORD0;
};

//Vertex shader
VertexShaderOut VS_Sprite(VertexShaderIn vertex)
{
	//create new blank output struct for pixel shader
	VertexShaderOut vsOut = (VertexShaderOut)0;

	//crete strcut for the new position following transforms
	float4 newPos;
	newPos.x = vertex.PosL.x; newPos.y = vertex.PosL.y; newPos.z = vertex.PosL.z;
	newPos.w = 1.0f;

	//multiply vertices by matrices to apply transformations
	vsOut.PosH = mul(newPos, gWorldViewProj); 

	//U co-ordinates - horizontal texture stuff
	//interesting breakdown - based on current portion of the horizontal texture
	//works out which column we are on
	
	vsOut.Tex[0] = ((1.0f / gNumCols) * gColumn + vertex.Tex[0] / gNumCols);

	//vsOut.Tex[0] = vertex.Tex[0];

	//if the flip value is -1.0f...
	if (gFlipValue == -1.0f)
	{
		//then we flip the texture by subtracting from 1.0f
		vsOut.Tex[0] = 1.0f - vsOut.Tex[0];
	}

	//V co-ordinates - vertical texture position
	vsOut.Tex[1] = vertex.Tex[1];

	//and here it goes to the pixel shader
	return vsOut;
}

//Pixel shader
float4 PS_Main(VertexShaderOut frag) : SV_TARGET
{
	//get the corresponding texel and convert to the pixel
	//to display
	float4 color = gSpriteTex.Sample(samLinear, frag.Tex);

	//keep same alpha value to prevent issues with colour blending
	//we could do alpha blending separately
	float alphaValue = color.w;

	//lerp between current pixel color and alt color value
	//based on lerp value
	color = lerp(color, gAltColor, gLerpValue);
	//restore original alpha value
	color.w = alphaValue;

	//get the minimum of the texture alpha value / opacity value
	//(transparent areas of a texture should remain so)
	color.w = min(color.w, gOpacityValue);

	//return final pixel color
	return color;
}

//shader technique
technique11 Sprite
{
	//only one pass; go through the shaders once
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_Sprite()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Main()));
	}
}

