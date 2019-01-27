#include "BTLTMap.h"

DPhoenix::BTLTMap::BTLTMap(std::string filename, TextureMgr* mTexMgr, ID3D11Device* md3dDevice,
	std::vector<XMFLOAT3>& enemySpawnVec,
	std::vector<XMFLOAT3>& playerSpawnVec,
	std::vector<XMFLOAT3>& beaconSpawnVec,
	DPhoenix::GeometryGenerator::MeshData* _Box,
	std::vector<std::string>& _floorColorMaps, std::vector<std::string>& _floorNormalMaps,
	std::vector<std::string>& _wallColorMaps, std::vector<std::string>& _wallNormalMaps,
	std::vector<std::string>& _coverColorMaps, std::vector<std::string>& _coverNormalMaps,
	int tilesWidth, int tilesLength,
	float tileSize,
	std::vector<XMFLOAT3>& waterSpawnVec)
{
	//get from CSV file and populate 2D vector
	std::ifstream inMapFile(filename);

	//helpers for parsing CSV file
	std::string value;
	std::string ignore;
	//caluclate the width and height in tiles
	mWidth = tilesWidth;
	mLength = tilesLength;
	//size of tiles (assumed square)

	//values used in constructing map
	int row = mLength - 1; //counting backwards because it stacks tiles
	int col = 0;
	int count = 0;
	   
	//set the vector values based on our tile sizes
	//(build 2D array for tiles)
	mTiles.resize(mWidth);
	for (int i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].resize(mLength);
	}
	XMFLOAT3 objectPos; //used to set spawn positions

	//size to 4 each as per designs - could be defined with separate const vars
	//for readability and extensibility
	playerSpawnVec.resize(4);
	enemySpawnVec.resize(4);

	//map parsing; starting at the top row and going by column
	//after each row has the column value filled, decrement (go down a row)
	//and repeat action - builds map array as it appears in the spreadsheet

	//seed once
	srand(time(0));

	//read in map file (CSV) and continue while open
	while (inMapFile.good())
	{
		//get line until next comma - store in value
		std::getline(inMapFile, value, '\n');

		//if we have an actual line
		if (value.length() > 1)
		{
			//loop through the chars in the value given
			for (int i = 0; i < value.length(); i++)
			{
				//we'll be using this portion of the ascii table
				//which doesn't include commas or file header weirdness
				//hence we can check we are between these values
				int indexStart = (int)'0';
				int indexEnd = (int)'z';
				//if the char value is within the ascii table values set
				if ((int)value[i] >= indexStart && (int)value[i] <= indexEnd)
				{
					//check what the value is for this tile
					switch (value[i])
					{
					case '0':
						mTiles[col][row] = new MapBlock(DPhoenix::NULLSPACE_MAPBLOCK,
														mTexMgr, md3dDevice, _Box,
														_floorColorMaps, _floorNormalMaps,
														_wallColorMaps, _wallNormalMaps,
														_coverColorMaps, _coverNormalMaps,
														col, row, tileSize, DPhoenix::NO_SP_FLAG);
						break;
					case 'f':
						mTiles[col][row] = new MapBlock(DPhoenix::FLOOR_MAPBLOCK,
													mTexMgr, md3dDevice, _Box,
													_floorColorMaps, _floorNormalMaps,
													_wallColorMaps, _wallNormalMaps,
													_coverColorMaps, _coverNormalMaps,
													col, row, tileSize, DPhoenix::NO_SP_FLAG);
						break;
					case 'c':
						mTiles[col][row] = new MapBlock(DPhoenix::COVER_MAPBLOCK,
											mTexMgr, md3dDevice, _Box,
											_floorColorMaps, _floorNormalMaps,
											_wallColorMaps, _wallNormalMaps,
											_coverColorMaps, _coverNormalMaps,
											col, row, tileSize, DPhoenix::NO_SP_FLAG);
						break;
					case 'b':
						mTiles[col][row] = new MapBlock(DPhoenix::WALL_MAPBLOCK,
											mTexMgr, md3dDevice, _Box,
											_floorColorMaps, _floorNormalMaps,
											_wallColorMaps, _wallNormalMaps,
											_coverColorMaps, _coverNormalMaps,
											col, row, tileSize, DPhoenix::NO_SP_FLAG);

						
						break;
					case 'l': // Lower Case L - case below is number 1
						mTiles[col][row] = new MapBlock(DPhoenix::BEACON_MAPBLOCK,
											mTexMgr, md3dDevice, _Box,
											_floorColorMaps, _floorNormalMaps,
											_wallColorMaps, _wallNormalMaps,
											_coverColorMaps, _coverNormalMaps,
											col, row, tileSize, DPhoenix::NO_SP_FLAG);

						beaconSpawnVec.push_back(XMFLOAT3(col * tileSize, 20.0f, row * tileSize));
						break;
					case '1':
						
						mTiles[col][row] = new MapBlock(DPhoenix::FLOOR_MAPBLOCK,
							mTexMgr, md3dDevice, _Box,
							_floorColorMaps, _floorNormalMaps,
							_wallColorMaps, _wallNormalMaps,
							_coverColorMaps, _coverNormalMaps,
							col, row, tileSize, DPhoenix::PLAYER_SP_FLAG);

						playerSpawnVec[0] = XMFLOAT3(col * tileSize, tileSize / 2 + 5.0f, row * tileSize);

						break;
					case '2':

						mTiles[col][row] = new MapBlock(DPhoenix::FLOOR_MAPBLOCK,
							mTexMgr, md3dDevice, _Box,
							_floorColorMaps, _floorNormalMaps,
							_wallColorMaps, _wallNormalMaps,
							_coverColorMaps, _coverNormalMaps,
							col, row, tileSize, DPhoenix::PLAYER_SP_FLAG);

						playerSpawnVec[1] = XMFLOAT3(col * tileSize, tileSize / 2 + 5.0f, row * tileSize);

					break;
					case '3':

						mTiles[col][row] = new MapBlock(DPhoenix::FLOOR_MAPBLOCK,
							mTexMgr, md3dDevice, _Box,
							_floorColorMaps, _floorNormalMaps,
							_wallColorMaps, _wallNormalMaps,
							_coverColorMaps, _coverNormalMaps,
							col, row, tileSize, DPhoenix::PLAYER_SP_FLAG);

						playerSpawnVec[2] = XMFLOAT3(col * tileSize, tileSize / 2 + 5.0f, row * tileSize);

					break;
					case '4':

						mTiles[col][row] = new MapBlock(DPhoenix::FLOOR_MAPBLOCK,
							mTexMgr, md3dDevice, _Box,
							_floorColorMaps, _floorNormalMaps,
							_wallColorMaps, _wallNormalMaps,
							_coverColorMaps, _coverNormalMaps,
							col, row, tileSize, DPhoenix::PLAYER_SP_FLAG);

						playerSpawnVec[3] = XMFLOAT3(col * tileSize, tileSize / 2 + 5.0f, row * tileSize);

					break;
					case '9':

						mTiles[col][row] = new MapBlock(DPhoenix::FLOOR_MAPBLOCK,
							mTexMgr, md3dDevice, _Box,
							_floorColorMaps, _floorNormalMaps,
							_wallColorMaps, _wallNormalMaps,
							_coverColorMaps, _coverNormalMaps,
							col, row, tileSize, DPhoenix::ENEMY_SP_FLAG);

						enemySpawnVec[0] = XMFLOAT3(col * tileSize, tileSize / 2 + 5.0f, row * tileSize);

						break;
					case '8':

						mTiles[col][row] = new MapBlock(DPhoenix::FLOOR_MAPBLOCK,
							mTexMgr, md3dDevice, _Box,
							_floorColorMaps, _floorNormalMaps,
							_wallColorMaps, _wallNormalMaps,
							_coverColorMaps, _coverNormalMaps,
							col, row, tileSize, DPhoenix::ENEMY_SP_FLAG);

						enemySpawnVec[1] = XMFLOAT3(col * tileSize, tileSize / 2 + 5.0f, row * tileSize);

					break;
					case '7':

						mTiles[col][row] = new MapBlock(DPhoenix::FLOOR_MAPBLOCK,
							mTexMgr, md3dDevice, _Box,
							_floorColorMaps, _floorNormalMaps,
							_wallColorMaps, _wallNormalMaps,
							_coverColorMaps, _coverNormalMaps,
							col, row, tileSize, DPhoenix::ENEMY_SP_FLAG);

						enemySpawnVec[2] = XMFLOAT3(col * tileSize, tileSize / 2 + 5.0f, row * tileSize);

					break;
					case '6':

						mTiles[col][row] = new MapBlock(DPhoenix::FLOOR_MAPBLOCK,
							mTexMgr, md3dDevice, _Box,
							_floorColorMaps, _floorNormalMaps,
							_wallColorMaps, _wallNormalMaps,
							_coverColorMaps, _coverNormalMaps,
							col, row, tileSize, DPhoenix::ENEMY_SP_FLAG);

						enemySpawnVec[3] = XMFLOAT3(col * tileSize, tileSize / 2 + 5.0f, row * tileSize);
					break;
					case 'w':
						//set water block and add spawn vector to position
						mTiles[col][row] = new MapBlock(DPhoenix::WATER_MAPBLOCK,
							mTexMgr, md3dDevice, _Box,
							_floorColorMaps, _floorNormalMaps,
							_wallColorMaps, _wallNormalMaps,
							_coverColorMaps, _coverNormalMaps,
							col, row, tileSize, DPhoenix::NO_SP_FLAG);

						waterSpawnVec.push_back(XMFLOAT3(col * tileSize, 10.0f, row * tileSize)); 
						break; 

					}
					//increment column
					col++;
				}
			}
			//decrement row; tile rows stack on top of each other
			row--; col = 0;
		}
	}

}

int DPhoenix::BTLTMap::GetColFromPosition(XMFLOAT3 _pos)
{
	float x = round(_pos.x / 20.0f);

	if (x < 0 || x > mWidth)
		return -1;
	else
		return x;
}

int DPhoenix::BTLTMap::GetRowFromPosition(XMFLOAT3 _pos)
{
	float z = round(_pos.z / 20.0f);

	if (z < 0 || z > mLength)
		return -1;
	else
		return z;
}

XMFLOAT3 DPhoenix::BTLTMap::GetPositionAboveFromMapRef(int col, int row)
{
	XMFLOAT3 position;

	position.x = col * 20.0f;

	if (mTiles[col][row]->mMapBlockType == FLOOR_MAPBLOCK ||
		mTiles[col][row]->mMapBlockType == BEACON_MAPBLOCK ||
		mTiles[col][row]->mMapBlockType == WATER_MAPBLOCK)
		position.y = 10.5f; 
	
	if (mTiles[col][row]->mMapBlockType == FLOOR_MAPBLOCK ||
		mTiles[col][row]->mMapBlockType == BEACON_MAPBLOCK)
		position.y = 10.5f;

	if (mTiles[col][row]->mMapBlockType == COVER_MAPBLOCK)
		position.y = 20.5f;

	if (mTiles[col][row]->mMapBlockType == WALL_MAPBLOCK)
		position.y = 30.5f;

	position.z = row * 20.0f;

	return position;
}
