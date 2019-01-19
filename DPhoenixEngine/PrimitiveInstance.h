#pragma once

#include "D3DUtil.h"

namespace DPhoenix
{
	enum PrimitiveTypes
	{
		PRIMITIVE_COLOR,
		PRIMITIVE_TEX,
		PRIMITIVE_LITTEX
	};

	class PrimitiveInstance : public Entity
	{
	private:
		GeometryGenerator::MeshData* mMeshPtr;

		//vertex and index buffers for the quad / sprite
		ID3D11Buffer* mPrimitiveVB;
		ID3D11Buffer* mPrimitiveIB;
		//primitive type relating to input layout for rendering
		PrimitiveTypes mPrimitiveType;

	public:

		float mOpacity;

		//texture map / color map / diffuse map
		ID3D11ShaderResourceView* mDiffuseMap;
		Material* mMaterial;	//material to react with light
		
		//normal map
		ID3D11ShaderResourceView* mNormalMap;

		//values for texture scaling
		float mTexScaleU; float mTexScaleV;
		
		//constructor / destructor
		PrimitiveInstance();
		~PrimitiveInstance();

		//here we will update the instance's movement
		//we will experiment with moving relatively around the forward vector
		//at some point later on
		void Update(float deltaTime, bool moveRelative);

		//this is to load an instance of a mesh using only position and color data
		//we get a pointer to the mesh data from GeometryGenerator, a defined color
		//(r,g,b,a) and also the device to create the buffers
		void LoadColorInstance(ID3D11Device* md3dDevice, XMFLOAT4 color,
			GeometryGenerator::MeshData* meshPtr);

		//this is to load an instance of a mesh using only position and UV data
		//we get a pointer to the mesh data from GeometryGenerator, a texture filename
		//the device to create the buffers and a pointer to the texture manager
		void LoadBasicTexInstance(ID3D11Device* md3dDevice, std::string texFilename,
			GeometryGenerator::MeshData* meshPtr, TextureMgr* texMgr);

		//this is to load an instance of a mesh using full data
		//this doesn't include a normal map (coming soon!) but includes a material to react with light
		//we get a pointer to the mesh data from GeometryGenerator, a texture filename
		//the device to create the buffers and a pointer to the texture manager
		void LoadLitTexInstance(ID3D11Device* md3dDevice, std::string texFilename,
			GeometryGenerator::MeshData* meshPtr, TextureMgr* texMgr);

		//this generates the World matrix based on scale, 
		//translation (movement) and rotation for rendering
		XMMATRIX CalculateTransforms();

		//this renders the individual instance to the device context given
		void Render(ID3D11DeviceContext* dc);

		//faces the forward vector
		void SetFacingRotation(float offsetDeg);
	};
}

