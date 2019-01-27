#include "MapBlock.h"

DPhoenix::MapBlock::MapBlock(MapBlockTypes _blockType, TextureMgr * _texMgr, 
							ID3D11Device * _md3dDevice, DPhoenix::GeometryGenerator::MeshData* _Box,
							std::vector<std::string>& _floorColorMaps, std::vector<std::string>& _floorNormalMaps,
							std::vector<std::string>& _wallColorMaps, std::vector<std::string>& _wallNormalMaps,
							std::vector<std::string>& _coverColorMaps, std::vector<std::string>& _coverNormalMaps,
							int _arrayXPos, int _arrayYPos, float _tileSize, SpecialFlags _spFlag)
{
	mMapBlockType = _blockType;
	mArrayXPos = _arrayXPos;
	mArrayYPos = _arrayYPos;

	mPosition.x = mArrayXPos * _tileSize;
	mPosition.z = mArrayYPos * _tileSize;
	
	mSpecialFlag = _spFlag;

	//different material types for effects
	Material* materialStandard = new Material();
	Material* materialShiny = new Material();

	materialStandard->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	materialStandard->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	materialStandard->Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f);
	materialStandard->Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	materialShiny->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	materialShiny->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	materialShiny->Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
	materialShiny->Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	int index = 0;

	switch (mMapBlockType)
	{
		case FLOOR_MAPBLOCK:
			
			index = rand() % (_floorColorMaps.size());

			mMeshInstance = new PrimitiveInstance();
			mMeshInstance->LoadLitTexInstance(_md3dDevice, _floorColorMaps[index], _Box, _texMgr);
			mMeshInstance->mMaterial = materialStandard;
			mMeshInstance->mNormalMap = _texMgr->CreateTexture(_floorNormalMaps[index]);

			mPosition.y = 0.0f;

			mMeshInstance->mPosition = mPosition;
			mMeshInstance->mHalfSizes.x = 10.0f;
			mMeshInstance->mHalfSizes.y = 10.0f;
			mMeshInstance->mHalfSizes.z = 10.0f;
			//Entity settings for collision????
			

		break;
		case WALL_MAPBLOCK:

			index = rand() % (_wallColorMaps.size());

			mMeshInstance = new PrimitiveInstance();
			mMeshInstance->LoadLitTexInstance(_md3dDevice, _wallColorMaps[index], _Box, _texMgr);
			mMeshInstance->mMaterial = materialShiny;
			mMeshInstance->mNormalMap = _texMgr->CreateTexture(_wallNormalMaps[index]);

			mPosition.y = _tileSize;

			mMeshInstance->mPosition = mPosition;
			mMeshInstance->mHalfSizes.x = 10.0f;
			mMeshInstance->mHalfSizes.y = 10.0f;
			mMeshInstance->mHalfSizes.z = 10.0f;


			//Entity settings for collision????

		break;
		case COVER_MAPBLOCK:

			index = rand() % (_coverColorMaps.size());

			mMeshInstance = new PrimitiveInstance();
			mMeshInstance->LoadLitTexInstance(_md3dDevice, _coverColorMaps[index], _Box, _texMgr);
			mMeshInstance->mMaterial = materialStandard;
			mMeshInstance->mNormalMap = _texMgr->CreateTexture(_coverColorMaps[index]);

			mPosition.y = _tileSize / 2;

			mMeshInstance->mPosition = mPosition;
			mMeshInstance->mHalfSizes.x = 10.0f;
			mMeshInstance->mHalfSizes.y = 10.0f;
			mMeshInstance->mHalfSizes.z = 10.0f;
			//Entity settings for collision????

		break;
		case BEACON_MAPBLOCK:

			//same as floor but will have beacon on top - separate in case other settings applied in future			

			index = rand() % (_floorColorMaps.size());

			mMeshInstance = new PrimitiveInstance();
			mMeshInstance->LoadLitTexInstance(_md3dDevice, _floorColorMaps[index], _Box, _texMgr);
			mMeshInstance->mMaterial = materialStandard;
			mMeshInstance->mNormalMap = _texMgr->CreateTexture(_floorNormalMaps[index]);

			mPosition.y = 0.0f;

			mMeshInstance->mPosition = mPosition;
			mMeshInstance->mHalfSizes.x = 10.0f;
			mMeshInstance->mHalfSizes.y = 10.0f;
			mMeshInstance->mHalfSizes.z = 10.0f;
			//Entity settings for collision????

		break;
		case NULLSPACE_MAPBLOCK:
		case WATER_MAPBLOCK:
			mMeshInstance = NULL;
		break;
	}

}

DPhoenix::MapBlock::~MapBlock()
{
}


