//Texture shader - basic FX

//constant buffer - per object 
//- world view projection combined matrix
// - this is to tranform the vertices
cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
	//ambient light color (default white)
	float4 gAmbientColor : AMBIENT = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//ambient light intensity
	float gAmbientIntensity = 1.0f;

	//scale for UV mapping of texture
	float gTexScaleU = 1.0f;
	float gTexScaleV = 1.0f;

	float gOpacityValue = 1.0f;
};

// Nonnumeric values cannot be added to a cbuffer.
//this is the texture to be applied
Texture2D gDiffuseMap;

//linear mipmapping and wrap texture
//that way we can scale and repeat it
SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};


//struct for vertex input
struct VertexIn
{
	//x,y,z position in world space
	float3 Pos : POSITION;
	//texture u, v
	float2 Tex : TEXCOORD;
};

//struct for vertex output / pixel input
struct VertexOut
{
	//SV is System Value - used for final outputs
	//of 'processed' vertices
	float4 PosH : SV_POSITION;
	//texture u, v
	float2 Tex : TEXCOORD0;
};

//Vertex shader
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	//Transform to homogenous clip space
	//(what we can see from our camera)
	//(multiply vertex by matrix)
	//the float4 here adds a fourth dimension with a value of 1.0f
	//to ensure the multiplication with vertices is valid
	//this is standard and will not change
	vout.PosH = mul(float4(vin.Pos, 1.0f), gWorldViewProj);

	//set texture co-ordinates
	vout.Tex[0] = vin.Tex[0] * gTexScaleU;	//U mapping
	vout.Tex[1] = vin.Tex[1] * gTexScaleV;	//V mappng

	//this returns the VertexOut struct so it can then be used by the
	//Pixel Shader
	return vout;
}

//pixel shader
float4 PS(VertexOut pin) : SV_TARGET
{
	//get the corresponding texel and convert to the pixel
	//to display
	float4 color = gDiffuseMap.Sample(samLinear, pin.Tex);
	//update color value based on ambient lighting
	color = color * gAmbientColor * gAmbientIntensity;

	color.a = gOpacityValue;

	return color;
}

//name of technique so it can be managed by our Effects class
technique11 BasicTexTech
{
	//only one pass; multiple passes can be used for more complex effects
	pass P0
	{
		//vertex shader and pixel shader set to above functions
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}

