#pragma once
#include "D3DUtil.h"

namespace DPhoenix
{

	enum MapBlockTypes
	{
		FLOOR_MAPBLOCK,
		WALL_MAPBLOCK,
		COVER_MAPBLOCK,
		BEACON_MAPBLOCK,
		NULLSPACE_MAPBLOCK,
		WATER_MAPBLOCK
	};

	enum SpecialFlags
	{
		NO_SP_FLAG,
		PLAYER_SP_FLAG,
		ENEMY_SP_FLAG,
		HAPPY_PATH_SP_FLAG
	};

	class MapBlock
	{
		public:
			MapBlockTypes mMapBlockType;
			int mArrayXPos;
			int mArrayYPos;
			XMFLOAT3 mPosition;
			SpecialFlags mSpecialFlag;

			PrimitiveInstance* mMeshInstance;

			//methods ----------------------
			MapBlock(MapBlockTypes _blockType, TextureMgr * _texMgr,
				ID3D11Device * _md3dDevice, DPhoenix::GeometryGenerator::MeshData* _Box,
				std::vector<std::string>& _floorColorMaps, std::vector<std::string>& _floorNormalMaps,
				std::vector<std::string>& _wallColorMaps, std::vector<std::string>& _wallNormalMaps,
				std::vector<std::string>& _coverColorMaps, std::vector<std::string>& _coverNormalMaps,
				int _arrayXPos, int _arrayYPos, float _tileSize, SpecialFlags _spFlag);

			~MapBlock();

	};
}

