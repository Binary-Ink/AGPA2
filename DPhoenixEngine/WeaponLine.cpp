#include "WeaponLine.h"

DPhoenix::WeaponLine::WeaponLine(CharacterClass * attacker, XMFLOAT3 direction, XMFLOAT3 start, 
								AvailableActions action, TeamTypes teamType, 
								Team* playerTeam, Team* enemyTeam,
								TextureMgr* texMgr, ID3D11Device* d3dDevice)
{
	mWeaponLine = new Line();

	mWeaponLine->Init(start, direction, 200.0f);
	mWeaponLine->mEntityType = ENTITY_WEAPON_LINE;
	mWeaponLine->mCollidable = true;

	mAttacker = attacker;
	mAction = action;
	mWeaponStage = FIRE_LINE_WS;

	mTeamType = teamType;
	mPlayerTeam = playerTeam;
	mEnemyTeam = enemyTeam;

	switch (mAction)
	{
		case PISTOL_ACTION:
			mCoverDmg = 0.5f;
			mWeaponModifier = 1.1f;
			mPlayerTeam->mTPPool -= 2;
		break;

		case SHOTGUN_ACTION:
			mCoverDmg = 0.6f;
			mWeaponModifier = 1.2f;
			mPlayerTeam->mTPPool -= 3;
		break;
	}

	isHit = false;
	mDmg = -1;

	mTexMgr = texMgr;
	md3dDevice = d3dDevice;
}

DPhoenix::WeaponLine::~WeaponLine()
{
}

void DPhoenix::WeaponLine::Update(float dt)
{
	mStageTimer.Tick();

	float dmgOverallModifier = 1.0f;

	bool keepLooping = true;

	switch (mWeaponStage)
	{
		case FIRE_LINE_WS:
			//should only last one frame before checking
			//as collision should have happened			
			mWeaponStage = CHECK_LINE_WS;
		break;

		case CHECK_LINE_WS:
			//big one !!!!!!
			if (mWeaponLine->mCollided)
				mWeaponLine->SortCollisions();

			for (int i = 0; i < mWeaponLine->mCollidedEntities.size() && keepLooping; i++)
			{
				if (mWeaponLine->mCollidedEntities[i]->mEntityType == ENTITY_COVER_MESH)
				{
					dmgOverallModifier *= mCoverDmg;
					//add billboard sprite 'sparks'
					mBillboards.push_back(new DPhoenix::BillboardSprite());
					mBillboards.back()->LoadBS("Textures\\Impact\\Bang.png", mTexMgr, 15.0f, 15.0f, md3dDevice);
					mBillboards.back()->mPosition = mWeaponLine->mIntersectionPoints[i];
					//add FX
				}

				if (mTeamType == PLAYER_TEAM && mWeaponLine->mCollidedEntities[i]->mEntityType == ENTITY_ENEMY_MESH)
				{
					CharacterClass* victim;

					for (int j = 0; j < mEnemyTeam->mTeamMembers.size(); j++)
					{
						if (mEnemyTeam->mTeamMembers[j]->mModelInstance->mId ==
							mWeaponLine->mCollidedEntities[i]->mId)
						{
							victim = mEnemyTeam->mTeamMembers[j];
						}						
					}

					mDmg = mAttacker->WeaponAttack(victim, mWeaponModifier, dmgOverallModifier);
					isHit = true;

					if (mDmg > -1)
					{
						victim->TakeDamage(mDmg);
						//add billboard sprite 'bang'
						mBillboards.push_back(new DPhoenix::BillboardSprite());
						mBillboards.back()->LoadBS("Textures\\Impact\\Bang.png", mTexMgr, 15.0f, 15.0f, md3dDevice);
						mBillboards.back()->mPosition = mWeaponLine->mIntersectionPoints[i];
					}
					else
					{
						//miss / smoke animation
					}

					keepLooping = false;
					mDmgPosition = mWeaponLine->mIntersectionPoints[i];

					//add FX
				}

				if (mTeamType == ENEMY_TEAM && mWeaponLine->mCollidedEntities[i]->mEntityType == ENTITY_PLAYER_MESH)
				{
					CharacterClass* victim;

					for (int j = 0; j < mPlayerTeam->mTeamMembers.size(); j++)
					{
						if (mPlayerTeam->mTeamMembers[j]->mModelInstance->mId ==
							mWeaponLine->mCollidedEntities[i]->mId)
						{
							victim = mPlayerTeam->mTeamMembers[j];
						}
					}

					mDmg = mAttacker->WeaponAttack(victim, mWeaponModifier, dmgOverallModifier);
					isHit = true;

					if (mDmg > -1)
					{
						victim->TakeDamage(mDmg);
						//add billboard sprite 'bang'
						mBillboards.push_back(new DPhoenix::BillboardSprite());
						mBillboards.back()->LoadBS("Textures\\Impact\\Bang.png", mTexMgr, 15.0f, 15.0f, md3dDevice);
						mBillboards.back()->mPosition = mWeaponLine->mIntersectionPoints[i];
					}
					else
					{
						//miss / smoke animation
					}

					//add FX
				}

				if (mWeaponLine->mCollidedEntities[i]->mEntityType == ENTITY_WALL_MESH)
				{
					keepLooping = false;
					//add billboard sprite 'bang'
					mBillboards.push_back(new DPhoenix::BillboardSprite());
					mBillboards.back()->LoadBS("Textures\\Impact\\Bang.png", mTexMgr, 15.0f, 15.0f, md3dDevice);
					mBillboards.back()->mPosition = mWeaponLine->mIntersectionPoints[i];

					//add FX
				}

			}
			
			mWeaponLine->mCollidable = false;
			mWeaponLine->mAlive = false;

			mStageTimer.Reset();
			mWeaponStage = ANIMATE_WS;
		break;

		case ANIMATE_WS:

			if (mStageTimer.TotalTime() <= 2.5f)
			{
				mDmgPosition.y += 1.0f * dt;

				for (int i = 0; i < mBillboards.size(); i++)
				{
					mBillboards[i]->UpdateBS(dt);
				}

			}
			else
			{
				mStageTimer.Reset();
				mWeaponStage = COMPLETE_WS;
			}


		break;

		case COMPLETE_WS:

		break;
	}
}
