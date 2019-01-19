#pragma once
#include "Team.h"
#include "BillboardSprite.h"

namespace DPhoenix
{
	enum BeaconStates
	{
		BEACON_UNLIT,
		BEACON_PLAYER_LIT,
		BEACON_ENEMY_LIT
	};

	class Beacon
	{
		public:
			//ALL THE TEXTURES!!! =================================
			//Poles ---------------------------------------------
			ID3D11ShaderResourceView* mPoleUnlitDiffuseMap;
			ID3D11ShaderResourceView* mPolePlayerLitDiffuseMap;
			ID3D11ShaderResourceView* mPoleEnemyLitDiffuseMap;

			ID3D11ShaderResourceView* mCurrentPoleNormalMap;

			//Beacon ---------------------------------------------
			ID3D11ShaderResourceView* mBeaconUnlitDiffuseMap;
			ID3D11ShaderResourceView* mBeaconPlayerLitDiffuseMap;
			ID3D11ShaderResourceView* mBeaconEnemyLitDiffuseMap;

			ID3D11ShaderResourceView* mCurrentBeaconNormalMap;

			//primitives -----------------------------------------
			PrimitiveInstance* mPoleMeshInstance;
			PrimitiveInstance* mBeaconMeshInstance;

			//Lights --------------------------------------------
			PointLight* mRadiance;
			float mRadianceRange;
			bool mIsGrowing;

			BeaconStates mBeaconState;

			TextureMgr* mTexMgr;
			ID3D11Device* md3dDevice;

			//methods -------------------------------------
			Beacon(XMFLOAT3 position, TextureMgr* texMgr, ID3D11Device* d3dDevice,
				GeometryGenerator::MeshData* sphere, GeometryGenerator::MeshData* pole);
			~Beacon();

			void Update(float dt);

			void LightBeacon(bool isPlayer);
			
	};
}