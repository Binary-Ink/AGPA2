#pragma once
#include "Team.h"
#include "BillboardSprite.h"

namespace DPhoenix
{
	enum WeaponStages
	{
		FIRE_LINE_WS,
		CHECK_LINE_WS,
		ANIMATE_WS,
		COMPLETE_WS
	};

	class WeaponLine
	{
		public:
			Line* mWeaponLine;
			CharacterClass* mAttacker;
			GameTimer mStageTimer;
			WeaponStages mWeaponStage;
			AvailableActions mAction;
			XMFLOAT3 mDmgPosition;
			int mDmg;
			bool isHit;
			std::vector<BillboardSprite*> mBillboards;
			float mCoverDmg;
			float mWeaponModifier;
			TeamTypes mTeamType;

			TextureMgr* mTexMgr;
			ID3D11Device* md3dDevice;

			//pointers need passing ----------
			Team* mPlayerTeam;
			Team* mEnemyTeam;

			WeaponLine(CharacterClass* attacker, XMFLOAT3 direction, XMFLOAT3 start, 
						AvailableActions action, TeamTypes teamType, Team* playerTeam,
						Team* enemyTeam, TextureMgr* texMgr, ID3D11Device* d3dDevice);
			~WeaponLine();

			void Update(float dt);
	};
}