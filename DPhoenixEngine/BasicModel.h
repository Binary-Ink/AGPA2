#pragma once
#include "D3DUtil.h"

namespace DPhoenix
{
	//this is a copy from the vertex namespace for convenience
	//dependency issues from header files led to this solution
	struct PosNormalTexTan
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT4 TangentU;
	};

	class BasicModel
	{
	public:
		//constructor to load in using assimp importer given filename and texture path
		BasicModel(ID3D11Device* device, TextureMgr& texMgr, 
			const std::string& modelFilename, const std::string& texturePath);
		~BasicModel();
		//number of subsets
		UINT SubsetCount;
		//materials and diffuse / normal maps
		std::vector<Material> Mat;
		std::vector<ID3D11ShaderResourceView*> DiffuseMapSRV;
		std::vector<ID3D11ShaderResourceView*> NormalMapSRV;

		// Keep CPU copies of the mesh data to read from.  
		std::vector<PosNormalTexTan> Vertices;
		std::vector<USHORT> Indices;
		std::vector<MeshGeometry::Subset> Subsets;
		//the loaded mesh data
		MeshGeometry ModelMesh;
	};

	//class for each instance - entity type for collisions etc.
	class BasicModelInstance : public Entity
	{
		public:

			float mOpacity;

			BasicModel* Model;

			BasicModelInstance();
			~BasicModelInstance();

			void Update(float deltaTime, bool moveRelative);
			XMMATRIX CalculateTransforms();
			void SetFacingRotation(float offsetDeg);
	};
}
