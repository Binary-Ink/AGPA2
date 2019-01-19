#pragma once
#include "Team.h"
#include "BillboardSprite.h"

namespace DPhoenix
{

	enum WeaponMeshStages
	{
		SPAWN_MESH_WMS,
		FIRING_MESH_WMS,
		FADE_OUT_WMS,
		COMPLETE_WMS
	};

	enum WeaponTypes
	{
		MAGIC_WT,
		WEAPON_WT
	};

	class WeaponMesh
	{
	public:
		PrimitiveInstance* mWeaponMeshInstance;
		CharacterClass* mAttacker;
		GameTimer mStageTimer;
		WeaponMeshStages mWeaponStage;
		AvailableActions mAction;
		std::vector<XMFLOAT3> mDmgPositions;
		std::vector<int> mDmgValues;
		std::vector<bool> mHasCollided;
		std::vector<BillboardSprite*> mBillboards;
		float mCoverDmg;
		float mWeaponModifier;
		TeamTypes mTeamType;
		PointLight* mRadiance;
		float mDistanceTravelled;
		float mMaxDistance;
		float mMaxScale;
		float mScale;
		WeaponTypes mWeaponType;

		TextureMgr* mTexMgr;
		ID3D11Device* md3dDevice;

		//pointers need passing ----------
		Team* mPlayerTeam;
		Team* mEnemyTeam;

		WeaponMesh(CharacterClass* attacker, XMFLOAT3 direction, XMFLOAT3 start,
			AvailableActions action, TeamTypes teamType, Team* playerTeam,
			Team* enemyTeam, TextureMgr* texMgr, ID3D11Device* d3dDevice, 
			GeometryGenerator::MeshData* sphere);
		~WeaponMesh();

		void Update(float dt);
	};
}