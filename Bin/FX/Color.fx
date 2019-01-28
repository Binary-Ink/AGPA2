//Color shader - basic FX

//constant buffer - per object 
//- world view projection combined matrix
// - this is to tranform the vertices
cbuffer cbPerObject
{
	float4x4 gWorldViewProj;

	float gOpacityValue = 1.0f;
};

//struct for vertex input
struct VertexIn
{
	float3 Pos : POSITION;	//x,y,z position in world space
	float4 Color : COLOR;	//color of each vertex (r,g,b,a)
};

//struct for vertex output / pixel input
struct VertexOut
{
	//SV is System Value - used for final outputs
	//of 'processed' vertices
	float4 PosH : SV_POSITION;	
	float4 Color : COLOR;		//color in r,g,b,a
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

	//Just pass vertex color into the pixel shader
	vout.Color = vin.Color;

	//this returns the VertexOut struct so it can then be used by the
	//Pixel Shader
	return vout;
}

//pixel shader
float4 PS(VertexOut pin) : SV_TARGET
{
	float4 outputColor = pin.Color;
	outputColor.a = gOpacityValue;

	//simply return the color value given for the vertex
	return outputColor;
}

//name of technique so it can be managed by our Effects class
technique11 ColorTech
{
	//onlu one pass; multiple passes can be used for more complex effects
	pass P0
	{
		//vertex shader and pixel shader set to above functions
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}

