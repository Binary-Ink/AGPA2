#pragma once
#include "D3DUtil.h"

//put all vertex structs in thir own namespace for convenience
namespace Vertex
{
	//3 floats for x,y,z position
	//2 floats for u,v texture co-ords
	struct SpritePosTex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
	};

	//position and color value
	//3 floats for position (x,y,z)
	//4 floats for color (r,g,b,a)
	struct PosColor
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

	//position and texture co-ords
	struct PosTex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
	};
	//position, normal, texture, tangent (for lighting)
	struct PosNormalTexTan
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT4 TangentU;
	};

	//for skybox
	struct Pos
	{
		float x;
		float y;
		float z;
	};

	//for skinned / rigged models
	//weights and indices relate to skeleton
	struct PosNormalTexTanSkinned
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT4 TangentU;
		XMFLOAT3 Weights;
		BYTE BoneIndices[4];
	};
}

namespace DPhoenix
{
	//InputLayoutDesc class necessary to define 'input layout'
	//for the shader (e.g. how the data should be treated)
	class InputLayoutDesc
	{
	public:
		// Init like const int A::a[4] = {0, 1, 2, 3}; in .cpp file.
		static const D3D11_INPUT_ELEMENT_DESC SpritePosTex[2];
		static const D3D11_INPUT_ELEMENT_DESC PosColor[2];
		static const D3D11_INPUT_ELEMENT_DESC PosTex[2];
		static const D3D11_INPUT_ELEMENT_DESC PosNormalTexTan[4];
		static const D3D11_INPUT_ELEMENT_DESC Pos[1];
		static const D3D11_INPUT_ELEMENT_DESC PosNormalTexTanSkinned[6];
	};

	//class to manage all input layouts so they are all available at compile time
	class InputLayouts
	{
	public:
		static void InitAll(ID3D11Device* device);
		static void DestroyAll();

		static ID3D11InputLayout* SpritePosTex;
		static ID3D11InputLayout* PosColor;
		static ID3D11InputLayout* PosTex;
		static ID3D11InputLayout* PosNormalTexTan;
		static ID3D11InputLayout* Pos;
		static ID3D11InputLayout* PosNormalTexTanSkinned;
	};
}

