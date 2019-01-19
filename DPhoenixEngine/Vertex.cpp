#include "Vertex.h"
#include "Effect.h"

//this is where we define the input layouts for the shader
#pragma region InputLayoutDesc

const D3D11_INPUT_ELEMENT_DESC DPhoenix::InputLayoutDesc::SpritePosTex[2] =
{
	//the first value (FLOAT3) is for the vertex POSITION
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//the second value (FLOAT2) is for the vrtex TEXCOORD
	//is 12 as that is the byte size for the previous input element
	//32 / 8 = 4 | 4 * 3 = 12   
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC DPhoenix::InputLayoutDesc::PosColor[2] =
{
	//the first value (FLOAT3) is for the vertex POSITION
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//the second value (FLOAT4) is for the vrtex COLOR
	//is 12 as that is the byte size for the previous input element
	//32 / 8 = 4 | 4 * 3 = 12   
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC DPhoenix::InputLayoutDesc::PosTex[2] =
{
	//the first value (FLOAT3) is for the vertex POSITION
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//the second value (FLOAT2) is for the vrtex TEXCOORD
	//is 12 as that is the byte size for the previous input element
	//32 / 8 = 4 | 4 * 3 = 12   
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC DPhoenix::InputLayoutDesc::PosNormalTexTan[4] =
{
	//note the values and offsets need to match - 0, 12, 24, 32 (last one has two * 4 as only XMFLOAT2)
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC DPhoenix::InputLayoutDesc::Pos[1] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC DPhoenix::InputLayoutDesc::PosNormalTexTanSkinned[6] =
{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "WEIGHTS",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEINDICES",	0, DXGI_FORMAT_R8G8B8A8_UINT,   0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

//initialisation of input layouts
#pragma region InputLayouts

//set input layouts to zero before initialisation
ID3D11InputLayout* DPhoenix::InputLayouts::SpritePosTex = 0;
ID3D11InputLayout* DPhoenix::InputLayouts::PosColor = 0;
ID3D11InputLayout* DPhoenix::InputLayouts::PosTex = 0;
ID3D11InputLayout* DPhoenix::InputLayouts::PosNormalTexTan = 0;
ID3D11InputLayout* DPhoenix::InputLayouts::Pos = 0;
ID3D11InputLayout* DPhoenix::InputLayouts::PosNormalTexTanSkinned = 0;

//initialise all input layouts
void DPhoenix::InputLayouts::InitAll(ID3D11Device* device)
{
	//shader 'pass' description truct
	D3DX11_PASS_DESC passDesc;

	//
	// SpritePosTex
	//
	//get the first pass and put in the pass description struct
	Effects::SpriteFX->SpriteTech->GetPassByIndex(0)->GetDesc(&passDesc);
	//create the input layout
	HR(device->CreateInputLayout(
		InputLayoutDesc::SpritePosTex,		//the desc for the input layout
		2,									//number of input data types
		passDesc.pIAInputSignature,			//pointer to the compiled shader
		passDesc.IAInputSignatureSize,		//size of compiled shader
		&SpritePosTex));					//output, pointer to input layout object

	//
	// PosColor
	//
	//get the first pass and put in the pass description struct
	Effects::ColorFX->ColorTech->GetPassByIndex(0)->GetDesc(&passDesc);
	//create the input layout
	HR(device->CreateInputLayout(
		InputLayoutDesc::PosColor,			//the desc for the input layout
		2,									//number of input data types
		passDesc.pIAInputSignature,			//pointer to the compiled shader
		passDesc.IAInputSignatureSize,		//size of compiled shader
		&PosColor));

	//
	// PosTex
	//
	//get the first pass and put in the pass description struct
	Effects::BasicTexFX->BasicTexTech->GetPassByIndex(0)->GetDesc(&passDesc);
	//create the input layout
	HR(device->CreateInputLayout(
		InputLayoutDesc::PosTex,			//the desc for the input layout
		2,									//number of input data types
		passDesc.pIAInputSignature,			//pointer to the compiled shader
		passDesc.IAInputSignatureSize,		//size of compiled shader
		&PosTex));

	//
	// PosNormalTexTan
	//
	//get the first pass and put in the pass description struct
	Effects::LitTexFX->LitTexTech->GetPassByIndex(0)->GetDesc(&passDesc);
	//create the input layout
	HR(device->CreateInputLayout(
		InputLayoutDesc::PosNormalTexTan,	//the desc for the input layout
		4,									//number of input data types
		passDesc.pIAInputSignature,			//pointer to the compiled shader
		passDesc.IAInputSignatureSize,		//size of compiled shader
		&PosNormalTexTan));

	//
	// Pos
	//
	//get the first pass and put in the pass description struct
	Effects::SkyFX->SkyTech->GetPassByIndex(0)->GetDesc(&passDesc);
	//create the input layout
	HR(device->CreateInputLayout(
		InputLayoutDesc::Pos,	//the desc for the input layout
		1,									//number of input data types
		passDesc.pIAInputSignature,			//pointer to the compiled shader
		passDesc.IAInputSignatureSize,		//size of compiled shader
		&Pos));

	//
	// PosNormalTexTanSkinned
	//
	//get the first pass and put in the pass description struct
	Effects::LitTexNormalFX->LitTexNormalSkinnedTech->GetPassByIndex(0)->GetDesc(&passDesc);
	//create the input layout
	HR(device->CreateInputLayout(
		InputLayoutDesc::PosNormalTexTanSkinned,	//the desc for the input layout
		6,									//number of input data types
		passDesc.pIAInputSignature,			//pointer to the compiled shader
		passDesc.IAInputSignatureSize,		//size of compiled shader
		&PosNormalTexTanSkinned));
}

//release memory of input layouts
void DPhoenix::InputLayouts::DestroyAll()
{
	ReleaseCOM(SpritePosTex);
	ReleaseCOM(PosColor);
	ReleaseCOM(PosTex);
	ReleaseCOM(PosNormalTexTan);
	ReleaseCOM(Pos);
	ReleaseCOM(PosNormalTexTanSkinned);
}

#pragma endregion

