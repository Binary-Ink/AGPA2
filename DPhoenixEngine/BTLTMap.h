#pragma once

#include "MapBlock.h"

namespace DPhoenix
{
	class BTLTMap
	{

		public:

			//using vectors as opposed to arrays 
			//simply so they can be dynamically resized without
			//resorting to globals
			std::vector<std::vector<MapBlock*>> mTiles;

			//width and length in tiles
			int mWidth;
			int mLength;

			//constructor - takes in map data filename, texture manager, d3d device
			//and amount of tiles per panel and amount of panels (rows and columns)
			//as well as XMFLOAT3 origin points for spawn locations
			BTLTMap(std::string filename, TextureMgr* mTexMgr, ID3D11Device* md3dDevice,
				std::vector<XMFLOAT3>& enemySpawnVec,
				std::vector<XMFLOAT3>& playerSpawnVec,
				std::vector<XMFLOAT3>& beaconSpawnVec,
				DPhoenix::GeometryGenerator::MeshData* _Box,
				std::vector<std::string>& _floorColorMaps, std::vector<std::string>& _floorNormalMaps,
				std::vector<std::string>& _wallColorMaps, std::vector<std::string>& _wallNormalMaps,
				std::vector<std::string>& _coverColorMaps, std::vector<std::string>& _coverNormalMaps,
				int tilesWidth, int tilesLength,
				float tileSize,
				std::vector<XMFLOAT3>& waterSpawnVec);

			~BTLTMap() { };
					   
			int GetColFromPosition(XMFLOAT3 _pos);
			int GetRowFromPosition(XMFLOAT3 _pos);
			XMFLOAT3 GetPositionAboveFromMapRef(int col, int row);
	};
}