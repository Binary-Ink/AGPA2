#include "WeaponMesh.h"

DPhoenix::WeaponMesh::WeaponMesh(CharacterClass * attacker, XMFLOAT3 direction, XMFLOAT3 start,
	AvailableActions action, TeamTypes teamType,
	Team* playerTeam, Team* enemyTeam,
	TextureMgr* texMgr, ID3D11Device* d3dDevice, GeometryGenerator::MeshData* sphere)
{

	//sphere starts small - as it scales then so will the half-sizes
	Material* materialShiny = new Material();

	materialShiny->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	materialShiny->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	materialShiny->Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f);
	materialShiny->Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	mWeaponMeshInstance = new PrimitiveInstance();

	mRadiance = new PointLight();
	mRadiance->Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	
	//specular and attenuation constant
	mRadiance->Specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	mRadiance->Att = XMFLOAT3(1.0f, 0.1f, 0.02f);
	//could set button to update range
	mRadiance->Range = 75.0f;
	
	mTexMgr = texMgr;
	md3dDevice = d3dDevice;
	mScale = 1.0f;
	mDistanceTravelled = 0.0f;
	
	mAttacker = attacker;
	mAction = action;
	mWeaponStage = SPAWN_MESH_WMS;

	mTeamType = teamType;
	mPlayerTeam = playerTeam;
	mEnemyTeam = enemyTeam;


	switch (action)
	{
		case SCALE_THROW_ACTION:
			mCoverDmg = 0.4f;
			mWeaponModifier = 1.2f;
			mWeaponType = WEAPON_WT;

			mMaxScale = 8.0f;
			mMaxDistance = 750.0f;

			 mPlayerTeam->mTPPool -= 4; 
			 attacker->mMP -= 0;

			//set diffuse colour
			mRadiance->Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

			mWeaponMeshInstance->LoadLitTexInstance(md3dDevice, "Textures\\Weapons\\Scales_cm.png", sphere, mTexMgr);
			mWeaponMeshInstance->mMaterial = materialShiny;
			mWeaponMeshInstance->mNormalMap = mTexMgr->CreateTexture("Textures\\Weapons\\Scales_nm.png");
			mWeaponMeshInstance->mHalfSizes = XMFLOAT3(0.5f, 0.5f, 0.5f);
			mWeaponMeshInstance->mEntityType = ENTITY_WEAPON_MESH;
			mWeaponMeshInstance->mForwardVector = direction;
		break;

		case CLAW_THROW_ACTION:
			mCoverDmg = 0.4f;
			mWeaponModifier = 1.3f;
			mWeaponType = WEAPON_WT;

			mMaxScale = 8.0f;
			mMaxDistance = 750.0f;

			mPlayerTeam->mTPPool -= 4;
			attacker->mMP -= 0;

			//set diffuse colour
			mRadiance->Diffuse = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);

			mWeaponMeshInstance->LoadLitTexInstance(md3dDevice, "Textures\\Weapons\\Claw_cm.png", sphere, mTexMgr);
			mWeaponMeshInstance->mMaterial = materialShiny;
			mWeaponMeshInstance->mNormalMap = mTexMgr->CreateTexture("Textures\\Weapons\\Claw_nm.png");
			mWeaponMeshInstance->mHalfSizes = XMFLOAT3(0.5f, 0.5f, 0.5f);
			mWeaponMeshInstance->mEntityType = ENTITY_WEAPON_MESH;
			mWeaponMeshInstance->mForwardVector = direction;
		break;

		case ROCKET_LAUNCHER_ACTION:
			mCoverDmg = 0.7f;
			mWeaponModifier = 1.5f;
			mWeaponType = WEAPON_WT;

			mMaxScale = 8.0f;
			mMaxDistance = 750.0f;

			mPlayerTeam->mTPPool -= 10;
			attacker->mMP -= 0;

			//set diffuse colour
			mRadiance->Diffuse = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);

			mWeaponMeshInstance->LoadLitTexInstance(md3dDevice, "Textures\\Weapons\\Rocket_cm.png", sphere, mTexMgr);
			mWeaponMeshInstance->mMaterial = materialShiny;
			mWeaponMeshInstance->mNormalMap = mTexMgr->CreateTexture("Textures\\Weapons\\Rocket_nm.png");
			mWeaponMeshInstance->mHalfSizes = XMFLOAT3(0.5f, 0.5f, 0.5f);
			mWeaponMeshInstance->mEntityType = ENTITY_WEAPON_MESH;
			mWeaponMeshInstance->mForwardVector = direction;
		break;

		case FIRE_ACTION:
			mCoverDmg = 0.5f;
			mWeaponModifier = 1.1f;
			mWeaponType = MAGIC_WT;

			mMaxScale = 8.0f;
			mMaxDistance = 500.0f;

			mPlayerTeam->mTPPool -= 2;
			attacker->mMP -= 25;

			//set diffuse colour
			mRadiance->Diffuse = XMFLOAT4(1.0f, 0.25f, 0.0f, 1.0f);

			mWeaponMeshInstance->LoadLitTexInstance(md3dDevice, "Textures\\Weapons\\Fire_cm.png", sphere, mTexMgr);
			mWeaponMeshInstance->mMaterial = materialShiny;
			mWeaponMeshInstance->mNormalMap = mTexMgr->CreateTexture("Textures\\Weapons\\Fire_nm.png");
			mWeaponMeshInstance->mHalfSizes = XMFLOAT3(0.5f, 0.5f, 0.5f);
			mWeaponMeshInstance->mEntityType = ENTITY_WEAPON_MESH;
			mWeaponMeshInstance->mForwardVector = direction;
		break;

		case ICE_ACTION:
			mCoverDmg = 0.5f;
			mWeaponModifier = 1.1f;
			mWeaponType = MAGIC_WT;

			mMaxScale = 8.0f;
			mMaxDistance = 500.0f;

			mPlayerTeam->mTPPool -= 3;
			attacker->mMP -= 25;

			//set diffuse colour
			mRadiance->Diffuse = XMFLOAT4(0.25f, 1.0f, 0.25f, 1.0f);

			mWeaponMeshInstance->LoadLitTexInstance(md3dDevice, "Textures\\Weapons\\Ice_cm.png", sphere, mTexMgr);
			mWeaponMeshInstance->mMaterial = materialShiny;
			mWeaponMeshInstance->mNormalMap = mTexMgr->CreateTexture("Textures\\Weapons\\Ice_nm.png");
			mWeaponMeshInstance->mHalfSizes = XMFLOAT3(0.5f, 0.5f, 0.5f);
			mWeaponMeshInstance->mEntityType = ENTITY_WEAPON_MESH;
			mWeaponMeshInstance->mForwardVector = direction;
		break;

		case LIGHTNING_ACTION:
			mCoverDmg = 0.6f;
			mWeaponModifier = 1.2f;
			mWeaponType = MAGIC_WT;

			mMaxScale = 8.0f;
			mMaxDistance = 500.0f;

			mPlayerTeam->mTPPool -= 4;
			attacker->mMP -= 50;

			//set diffuse colour
			mRadiance->Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

			mWeaponMeshInstance->LoadLitTexInstance(md3dDevice, "Textures\\Weapons\\Lightning_cm.png", sphere, mTexMgr);
			mWeaponMeshInstance->mMaterial = materialShiny;
			mWeaponMeshInstance->mNormalMap = mTexMgr->CreateTexture("Textures\\Weapons\\Lightning_nm.png");
			mWeaponMeshInstance->mHalfSizes = XMFLOAT3(0.5f, 0.5f, 0.5f);
			mWeaponMeshInstance->mEntityType = ENTITY_WEAPON_MESH;
			mWeaponMeshInstance->mForwardVector = direction;
		break;

		case DARKNESS_ACTION:
			mCoverDmg = 0.7f;
			mWeaponModifier = 1.3f;
			mWeaponType = MAGIC_WT;

			mMaxScale = 30.0f;
			mMaxDistance = 500.0f;

			mPlayerTeam->mTPPool -= 5;
			attacker->mMP -= 100;

			//set diffuse colour
			mRadiance->Diffuse = XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f);

			mWeaponMeshInstance->LoadLitTexInstance(md3dDevice, "Textures\\Weapons\\Darkness_cm.png", sphere, mTexMgr);
			mWeaponMeshInstance->mMaterial = materialShiny;
			mWeaponMeshInstance->mNormalMap = mTexMgr->CreateTexture("Textures\\Weapons\\Darkness_nm.png");
			mWeaponMeshInstance->mHalfSizes = XMFLOAT3(0.5f, 0.5f, 0.5f);
			mWeaponMeshInstance->mEntityType = ENTITY_WEAPON_MESH;
			mWeaponMeshInstance->mForwardVector = direction;
		break;

		case FURBALL_ARCANA_ACTION:
			mCoverDmg = 0.5f;
			mWeaponModifier = 1.5f;
			mWeaponType = MAGIC_WT;

			mMaxScale = 8.0f;
			mMaxDistance = 500.0f;

			mPlayerTeam->mTPPool -= 10;
			attacker->mMP -= 50;

			//set diffuse colour
			mRadiance->Diffuse = XMFLOAT4(0.5f, 0.25f, 0.15f, 1.0f);

			mWeaponMeshInstance->LoadLitTexInstance(md3dDevice, "Textures\\Weapons\\Furball_cm.png", sphere, mTexMgr);
			mWeaponMeshInstance->mMaterial = materialShiny;
			mWeaponMeshInstance->mNormalMap = mTexMgr->CreateTexture("Textures\\Weapons\\Furball_nm.png");
			mWeaponMeshInstance->mHalfSizes = XMFLOAT3(0.5f, 0.5f, 0.5f);
			mWeaponMeshInstance->mEntityType = ENTITY_WEAPON_MESH;
			mWeaponMeshInstance->mForwardVector = direction;
		break;

	}

	XMFLOAT3 mStartPos;

	mStartPos.x = start.x + mWeaponMeshInstance->mForwardVector.x * (mMaxScale / 2.0f);
	mStartPos.y = start.y + mWeaponMeshInstance->mForwardVector.y * (mMaxScale / 2.0f);
	mStartPos.z = start.z + mWeaponMeshInstance->mForwardVector.z * (mMaxScale / 2.0f);


	//initial position but updated in UpdateScene
	mRadiance->Position = mStartPos;
	mWeaponMeshInstance->mPosition = mStartPos;
	mWeaponMeshInstance->mPrevPosition = mStartPos;

}

DPhoenix::WeaponMesh::~WeaponMesh()
{
}

void DPhoenix::WeaponMesh::Update(float dt)
{
	mStageTimer.Tick();

	float dmgOverallModifier = 1.0f;

	mWeaponMeshInstance->mRotation.x += 7.0f * dt;
	mWeaponMeshInstance->mRotation.y += 7.0f * dt;
	mWeaponMeshInstance->mRotation.z += 7.0f * dt;

	switch (mWeaponStage)
	{
		case SPAWN_MESH_WMS:
			
			mScale += 2.0f * mMaxScale * dt;			

			if (mScale >= mMaxScale)
			{
				mScale = mMaxScale;

				mWeaponStage = FIRING_MESH_WMS;
				mWeaponMeshInstance->mCollidable = true;
			}

			mWeaponMeshInstance->mScale.x = mScale;
			mWeaponMeshInstance->mScale.y = mScale;
			mWeaponMeshInstance->mScale.z = mScale;

			mWeaponMeshInstance->mHalfSizes.x = mScale * 0.5;
			mWeaponMeshInstance->mHalfSizes.y = mScale * 0.5;
			mWeaponMeshInstance->mHalfSizes.z = mScale * 0.5;

			mWeaponMeshInstance->Update(dt, true);

		break;

		case FIRING_MESH_WMS:
			
			//at this point the main demo should add the weaponmesh as an entity for colisions
			//we need to check that the collided entity is stored so it only collides once 
			//that way we aren't adding modifiers / damage several times over			

			//The engine collision method will only add to the collided entities vector
			//once per collision - hence we just need a set of flags to check if we have
			//dealt with the collision
			mHasCollided.resize(mWeaponMeshInstance->mCollidedEntities.size());

			for (int i = 0; i < mWeaponMeshInstance->mCollidedEntities.size(); i++)
			{
				if (!mHasCollided[i])
				{
					mHasCollided[i] = true;
									   					 
					if (mWeaponMeshInstance->mCollidedEntities[i]->mEntityType == ENTITY_COVER_MESH)
					{
						dmgOverallModifier *= mCoverDmg;
						//add billboard sprite 'sparks' - this could change for each type
						mBillboards.push_back(new DPhoenix::BillboardSprite());
						mBillboards.back()->LoadBS("Textures\\Impact\\Bang.png", mTexMgr, 15.0f, 15.0f, md3dDevice);
						mBillboards.back()->mPosition = mWeaponMeshInstance->mCollidedEntities[i]->mPosition;
						mBillboards.back()->mPosition.y = mWeaponMeshInstance->mPosition.y;
						//correct positioning for visibility????????
						mBillboards.back()->mPosition.x -= (mWeaponMeshInstance->mForwardVector.x *
											(mWeaponMeshInstance->mCollidedEntities[i]->mHalfSizes.x + 5.0f));
						mBillboards.back()->mPosition.z -= (mWeaponMeshInstance->mForwardVector.z *
											(mWeaponMeshInstance->mCollidedEntities[i]->mHalfSizes.z + 5.0f));
						//add FX
					
					}

					if (mTeamType == PLAYER_TEAM && mWeaponMeshInstance->mCollidedEntities[i]->mEntityType == ENTITY_ENEMY_MESH)
					{
						CharacterClass* victim;

						for (int j = 0; j < mEnemyTeam->mTeamMembers.size(); j++)
						{
							if (mEnemyTeam->mTeamMembers[j]->mModelInstance->mId ==
								mWeaponMeshInstance->mCollidedEntities[i]->mId)
							{
								victim = mEnemyTeam->mTeamMembers[j];
							}
						}

						mDmgValues.push_back(mAttacker->MagicAttack(victim, mWeaponModifier, dmgOverallModifier));

						if (mDmgValues.back() > -1)
						{
							victim->TakeDamage(mDmgValues.back());
							//add billboard sprite 'bang'
							mBillboards.push_back(new DPhoenix::BillboardSprite());
							mBillboards.back()->LoadBS("Textures\\Impact\\Bang.png", mTexMgr, 15.0f, 15.0f, md3dDevice);
							//correct positioning for visibility????????
							mBillboards.back()->mPosition = mWeaponMeshInstance->mCollidedEntities[i]->mPosition;
							mBillboards.back()->mPosition.y = mWeaponMeshInstance->mPosition.y;
							//correct positioning for visibility????????
							mBillboards.back()->mPosition.x -= (mWeaponMeshInstance->mForwardVector.x *
								(mWeaponMeshInstance->mCollidedEntities[i]->mHalfSizes.x + 5.0f));
							mBillboards.back()->mPosition.z -= (mWeaponMeshInstance->mForwardVector.z *
								(mWeaponMeshInstance->mCollidedEntities[i]->mHalfSizes.z + 5.0f));
							
						}
						else
						{
							//miss / smoke animation
						}


						mDmgPositions.push_back(mWeaponMeshInstance->mCollidedEntities[i]->mPosition);

						if (mWeaponType == WEAPON_WT)
						{
							mWeaponStage = FADE_OUT_WMS;
							mWeaponMeshInstance->mCollidable = false;
						}

					}

					if (mTeamType == ENEMY_TEAM && mWeaponMeshInstance->mCollidedEntities[i]->mEntityType == ENTITY_PLAYER_MESH)
					{
						CharacterClass* victim;

						for (int j = 0; j < mPlayerTeam->mTeamMembers.size(); j++)
						{
							if (mPlayerTeam->mTeamMembers[j]->mModelInstance->mId ==
								mWeaponMeshInstance->mCollidedEntities[i]->mId)
							{
								victim = mPlayerTeam->mTeamMembers[j];
							}
						}

						mDmgValues.push_back(mAttacker->MagicAttack(victim, mWeaponModifier, dmgOverallModifier));

						if (mDmgValues.back() > -1)
						{
							victim->TakeDamage(mDmgValues.back());
							//add billboard sprite 'bang'
							mBillboards.push_back(new DPhoenix::BillboardSprite());
							mBillboards.back()->LoadBS("Textures\\Impact\\Bang.png", mTexMgr, 15.0f, 15.0f, md3dDevice);
							//correct positioning for visibility????????
							mBillboards.back()->mPosition = mWeaponMeshInstance->mCollidedEntities[i]->mPosition;
							mBillboards.back()->mPosition.y = mWeaponMeshInstance->mPosition.y;
							//correct positioning for visibility????????
							mBillboards.back()->mPosition.x -= (mWeaponMeshInstance->mForwardVector.x *
								(mWeaponMeshInstance->mCollidedEntities[i]->mHalfSizes.x + 5.0f));
							mBillboards.back()->mPosition.z -= (mWeaponMeshInstance->mForwardVector.z *
								(mWeaponMeshInstance->mCollidedEntities[i]->mHalfSizes.z + 5.0f));

						}
						else
						{
							//miss / smoke animation
						}

						mDmgPositions.push_back(mWeaponMeshInstance->mCollidedEntities[i]->mPosition);

						if (mWeaponType == WEAPON_WT)
						{
							mWeaponStage = FADE_OUT_WMS;
							mWeaponMeshInstance->mCollidable = false;
						}
					}

					if (mWeaponMeshInstance->mCollidedEntities[i]->mEntityType == ENTITY_WALL_MESH)
					{
						//add billboard sprite 'bang'
						mBillboards.push_back(new DPhoenix::BillboardSprite());
						mBillboards.back()->LoadBS("Textures\\Impact\\Bang.png", mTexMgr, 15.0f, 15.0f, md3dDevice);
						//correct positioning for visibility????????
						mBillboards.back()->mPosition = mWeaponMeshInstance->mCollidedEntities[i]->mPosition;
						mBillboards.back()->mPosition.y = mWeaponMeshInstance->mPosition.y;
						//correct positioning for visibility????????
						mBillboards.back()->mPosition.x -= (mWeaponMeshInstance->mForwardVector.x *
							(mWeaponMeshInstance->mCollidedEntities[i]->mHalfSizes.x + 5.0f));
						mBillboards.back()->mPosition.z -= (mWeaponMeshInstance->mForwardVector.z *
							(mWeaponMeshInstance->mCollidedEntities[i]->mHalfSizes.z + 5.0f));

						mWeaponStage = FADE_OUT_WMS;
						mWeaponMeshInstance->mCollidable = false;
					}

				}

			}

			mDistanceTravelled += 100.0f * dt;

			if (mDistanceTravelled >= mMaxDistance)
			{
				mDistanceTravelled = mMaxDistance;
				mWeaponStage = FADE_OUT_WMS;
				mWeaponMeshInstance->mCollidable = false;
			}

			mWeaponMeshInstance->mRelativeVelocity.z = 50.0f;
			mWeaponMeshInstance->Update(dt, true);

			for (int i = 0; i < mBillboards.size(); i++)
			{
				mBillboards[i]->UpdateBS(dt);
			}

			mRadiance->Position = mWeaponMeshInstance->mPosition;

			for (int i = 0; i < mDmgPositions.size(); i++)
			{
				mDmgPositions[i].y += 1.0f * dt;
			}

		break;

		case FADE_OUT_WMS:
			
			mScale -= 4.0f * mMaxScale * dt;

			mWeaponMeshInstance->mScale.x = mScale;
			mWeaponMeshInstance->mScale.y = mScale;
			mWeaponMeshInstance->mScale.z = mScale;

			mWeaponMeshInstance->mHalfSizes.x = mScale * 0.5;
			mWeaponMeshInstance->mHalfSizes.y = mScale * 0.5;
			mWeaponMeshInstance->mHalfSizes.z = mScale * 0.5;

			mWeaponMeshInstance->mRelativeVelocity.z = 0.0f;
			mWeaponMeshInstance->Update(dt, true);

			for (int i = 0; i < mBillboards.size(); i++)
			{
				mBillboards[i]->UpdateBS(dt);
			}

			if (mScale <= 1.0f)
			{
				mScale = 1.0f;

				mWeaponStage = COMPLETE_WMS;
			}

			break;

		case COMPLETE_WMS:
			
		break;

	}


}