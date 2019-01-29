#include "CharacterClass.h"

DPhoenix::CharacterClass::CharacterClass(CharacterClasses _class, TextureMgr * _texMgr, 
										ID3D11Device * _md3dDevice, AudioMgr * _audioMgr,
										std::vector<PrimitiveInstance*> _happyPath)
{
	mClass = _class;
	mTexMgr = _texMgr;
	md3dDevice = _md3dDevice;
	mAudioMgr = _audioMgr;
	mHappyPath = _happyPath;

	mGeoGen = new GeometryGenerator();
	mBox = new DPhoenix::GeometryGenerator::MeshData();
	mGeoGen->CreateBox(8.0f, 8.0f, 8.0f, *mBox);

	//AI STUFF -------------------------------------------------------
	mAIType = ATTACKER_AI;
	mTargetBeaconId = 0;
	mIsClockwiseAI = true;

	//different material types for effects
	Material* materialStandard = new Material();

	materialStandard->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	materialStandard->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	materialStandard->Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f);
	materialStandard->Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);


	switch (mClass)
	{
		case SOLDIER_CLASS:
			mModelInstance = new PrimitiveInstance();
			mModelInstance->LoadLitTexInstance(md3dDevice, "Textures\\Temp\\SoldierTemp_cm.png", mBox, mTexMgr);
			mModelInstance->mMaterial = materialStandard;
			mModelInstance->mNormalMap = mTexMgr->CreateTexture("Textures\\Temp\\SoldierTemp_cm.png");
			mModelInstance->mHalfSizes = XMFLOAT3(4.0f, 4.0f, 4.0f);
		break;
		case MAGE_CLASS:
			mModelInstance = new PrimitiveInstance();
			mModelInstance->LoadLitTexInstance(md3dDevice, "Textures\\Temp\\AngelTemp_cm.png", mBox, mTexMgr);
			mModelInstance->mMaterial = materialStandard;
			mModelInstance->mNormalMap = mTexMgr->CreateTexture("Textures\\Temp\\AngelTemp_nm.png");
			mModelInstance->mHalfSizes = XMFLOAT3(4.0f, 4.0f, 4.0f);
		break;
		case DWARF_CLASS:
			mModelInstance = new PrimitiveInstance();
			mModelInstance->LoadLitTexInstance(md3dDevice, "Textures\\Temp\\DragonTemp_cm.png", mBox, mTexMgr);
			mModelInstance->mMaterial = materialStandard;
			mModelInstance->mNormalMap = mTexMgr->CreateTexture("Textures\\Temp\\DragonTemp_nm.png");
			mModelInstance->mHalfSizes = XMFLOAT3(4.0f, 4.0f, 4.0f);
		break;
		case ARCHER_CLASS:
			mModelInstance = new PrimitiveInstance();
			mModelInstance->LoadLitTexInstance(md3dDevice, "Textures\\Temp\\CatTemp_cm.png", mBox, mTexMgr);
			mModelInstance->mMaterial = materialStandard;
			mModelInstance->mNormalMap = mTexMgr->CreateTexture("Textures\\Temp\\CatTemp_nm.png");
			mModelInstance->mHalfSizes = XMFLOAT3(4.0f, 4.0f, 4.0f);
		break;
	}


	mLifeState = CH_OK_LIFESTATE;
	mTurnState = CH_WAIT_TURNSTATE;
	mMoveState = CH_PICKRT_MOVESTATE;
	mAnimation = IDLE_ANIMATION;
	mSelectedAction = NO_ACTION;

	SetBaseStats();
	CalculateStats(1);

}

DPhoenix::CharacterClass::CharacterClass(const CharacterClass & character)
{
	//stats --------------------------------------------------
	mLevel = character.mLevel;
	mBaseExp = character.mBaseExp; mExp = character.mExp;
	mBaseTP = character.mBaseTP; mTP = character.mTP;
	mBaseHP = character.mBaseHP; mHP = character.mHP; mMaxHP = character.mMaxHP;
	mBaseMP = character.mBaseMP; mMP = character.mMP; mMaxMP = character.mMaxMP;
	mBasePower = character.mBasePower; mPower = character.mPower;
	mBaseFirepower = character.mBaseFirepower; mFirepower = character.mFirepower;
	mBaseDefense = character.mBaseDefense; mDefense = character.mDefense;
	mBaseAccuracy = character.mBaseAccuracy; mAccuracy = character.mAccuracy;
	mBaseEvasion = character.mBaseEvasion; mEvasion = character.mEvasion;
	mBaseMagic = character.mBaseMagic; mMagic = character.mMagic;
	mBaseCharm = character.mBaseCharm; mCharm = character.mCharm;

	//character traits ---------------------------------------
	mClass = character.mClass;
	//actions should go here when determined
	//models should go here when determined

	//TEMP MODELS TO BEGIN WITH --------------------------------------
	mGeoGen = character.mGeoGen;
	mBox = character.mBox;
	mModelInstance = new PrimitiveInstance(*character.mModelInstance);
	
	mLifeState = character.mLifeState;
	mTurnState = character.mTurnState;
	mMoveState = character.mMoveState;
	mAnimation = character.mAnimation;

	//pointers to main singletons / devices
	mTexMgr = character.mTexMgr;
	md3dDevice = character.md3dDevice;
	mAudioMgr = character.mAudioMgr;

	mHappyPath = character.mHappyPath;
}

DPhoenix::CharacterClass::~CharacterClass()
{
}

void DPhoenix::CharacterClass::SetBaseStats()
{
	//NEW CLASSES 
	//SEE DOCUMENTATION FOR GROWTH CHARTS
	switch(mClass)
	{
		case SOLDIER_CLASS: //NEW SOLDIER
			mBaseExp = 100;
			mBaseTP = 15;
			mBaseHP = 50;
			mBaseMP = 5;
			mBasePower = 45;
			mBaseFirepower = 35;
			mBaseDefense = 10;
			mBaseAccuracy = 50;
			mBaseEvasion = 15;
			mBaseMagic = 5;
			mBaseCharm = 10;
		break;
		case MAGE_CLASS: //NEW MAGE
			mBaseExp = 100;
			mBaseTP = 10;
			mBaseHP = 25;
			mBaseMP = 80;
			mBasePower = 10;
			mBaseFirepower = 10;
			mBaseDefense = 9;
			mBaseAccuracy = 30;
			mBaseEvasion = 1;
			mBaseMagic = 80;
			mBaseCharm = 45;
		break;
		case DWARF_CLASS: // NEW DWARF
			mBaseExp = 100;
			mBaseTP = 20;
			mBaseHP = 20;
			mBaseMP = 65;
			mBasePower = 20;
			mBaseFirepower = 25;
			mBaseDefense = 20;
			mBaseAccuracy = 30;
			mBaseEvasion = 10;
			mBaseMagic = 60;
			mBaseCharm = 30;
		break;
		case ARCHER_CLASS: // NEW ARCHER
			mBaseExp = 100;
			mBaseTP = 25;
			mBaseHP = 20;
			mBaseMP = 10;
			mBasePower = 15;
			mBaseFirepower = 45;
			mBaseDefense = 20;
			mBaseAccuracy = 70;
			mBaseEvasion = 55;
			mBaseMagic = 10;
			mBaseCharm = 30;
		break;
	}

}

void DPhoenix::CharacterClass::CalculateStats(int _level)
{
	
	mLevel = _level;
	//as defined in the spreadsheet and GDD
	switch (mClass)
	{
	case SOLDIER_CLASS:
		mExp = mBaseExp * (mLevel^2);
		mTP = floor(mBaseTP + (pow(mLevel, 2) * 0.05));
		mHP = floor(mBaseHP * (pow(mLevel,1.9)));
		mMaxHP = mHP;
		mMP = floor(mBaseMP + (pow(mLevel, 2) * 0.5));
		mMaxMP = mMP;
		mPower = min( floor(log2(mLevel) * 55 + mBasePower), 255);
		mFirepower = floor(log2(mLevel) * 45 + mBaseFirepower);
		mDefense = floor(log2(mLevel) * 40 + mBaseDefense);
		mAccuracy = floor(log2(mLevel) * 15 + mBaseAccuracy);
		mEvasion = floor(mBaseEvasion + (pow(mLevel,2) * 0.125));
		mMagic = floor(mBaseMagic + pow(mLevel,2) * 0.25);
		mCharm = floor(mBaseCharm + pow(mLevel, 2) * 0.4);
	break;
	case MAGE_CLASS:
		mExp = mBaseExp * (mLevel ^ 2);
		mTP = floor(mBaseTP + (pow(mLevel, 2) * 0.05));
		mHP = floor(mBaseHP * (pow(mLevel, 1.9)));
		mMaxHP = mHP;
		mMP = floor(log2(mLevel) * 60 + mBaseMP);
		mMaxMP = mMP;
		mPower = floor(mBasePower * (pow(mLevel, 1.1)));
		mFirepower = floor(mBaseFirepower * (pow(mLevel, 1.05)));
		mDefense = floor(mBaseDefense * (pow(mLevel, 0.8)));
		mAccuracy = floor(mBaseAccuracy * (pow(mLevel, 0.5)));
		mEvasion = floor(mBaseEvasion + (pow(mLevel,2) * 0.125));
		mMagic = min(floor(log2(mLevel) * 50 + mBaseMagic), 255);
		mCharm = min(floor(log2(mLevel) * 40 + mBaseCharm), 255);
	break;
	case DWARF_CLASS:
		mExp = mBaseExp * (mLevel ^ 2);
		mTP = floor(mBaseTP + (pow(mLevel, 2) * 0.0125));
		mHP = floor(mBaseHP * (pow(mLevel, 1.95)));
		mMaxHP = mHP;
		mMP = floor(log2(mLevel) * 40 + mBaseMP);
		mMaxMP = mMP;
		mPower = floor(mBasePower * (pow(mLevel, 0.7)));
		mFirepower = floor(mBaseFirepower * (pow(mLevel, 0.96)));
		mDefense = floor(mBaseDefense * (pow(mLevel, 0.5)));		
		mAccuracy = floor(mBaseAccuracy * (pow(mLevel, 0.3)));
		mEvasion = floor(mBaseEvasion + (pow(mLevel, 2) * 0.025));
		mMagic = min(floor(log2(mLevel) * 35 + mBaseMagic), 255);
		mCharm = min(floor(log2(mLevel) * 40 + mBaseCharm), 255);
	break;
	case ARCHER_CLASS:
		mExp = mBaseExp * (mLevel ^ 2);
		mTP = floor(mBaseTP + (pow(mLevel, 2) * 0.05));
		mHP = floor(mBaseHP * (pow(mLevel, 1.8)));
		mMaxHP = mHP;
		mMP = floor(mBaseMP + (pow(mLevel, 2) * 0.3));
		mMaxMP = mMP;
		mPower = min(floor(log2(mLevel) * 35 + mBasePower), 255);
		mFirepower = floor(log2(mLevel) * 30 + mBaseFirepower);
		mDefense = floor(log2(mLevel) * 40 + mBaseDefense);
		mAccuracy = floor(log2(mLevel) * 15 + mBaseAccuracy);
		mEvasion = floor(log2(mLevel) * 15 + mBaseEvasion);
		mMagic = floor(mBaseMagic + pow(mLevel, 2) * 0.25);
		mCharm = floor(mBaseCharm + pow(mLevel, 2) * 0.4);
	break;
	}

}

int DPhoenix::CharacterClass::MeleeAttack(CharacterClass* _target)
{
	std::srand(time(0));

	int rng = rand() % 100 + 1;

	int dmg = mPower * (rng * 0.1) -(_target->mDefense / rng);

	if (dmg < 0)
		dmg = 0;

	return dmg;
}

int DPhoenix::CharacterClass::MagicAttack(CharacterClass* _target, int _modifier, int _coverDmg)
{
	std::srand(time(0));

	int rng = rand() % 100 + 1;
	int rng2 = rand() % 100 + 1;

	int hit = mAccuracy * (rng * 0.01) - _target->mEvasion * (rng * 0.005);

	if (hit < 0)
		return -1; //to indicate miss

	int magic = mMagic * _modifier;

	int dmg = magic * (rng * 0.05) - (_target->mCharm * 0.75 / (rng * 0.1));

	if (dmg < 0)
		dmg = 0;

	return dmg;
}

int DPhoenix::CharacterClass::WeaponAttack(CharacterClass*_target, int _modifier, int _coverDmg)
{
	std::srand(time(0));

	int rng = rand() % 100 + 1;
	int rng2 = rand() % 100 + 1;

	int hit = mAccuracy * (rng * 0.005) - _target->mEvasion * (rng * 0.01);

	if (hit < 0)
		return -1; //to indicate miss

	int firepower = mFirepower * _modifier;

	int dmg = firepower * (rng * 0.1) - (_target->mDefense * 0.65 / (rng * 0.1));

	if (dmg < 0)
		dmg = 0;

	return dmg;
}

void DPhoenix::CharacterClass::Update(float dt)
{
	switch (mLifeState)
	{
		case CH_OK_LIFESTATE:

			switch (mTurnState)
			{
				case CH_ACTIVE_TURNSTATE:

					switch (mMoveState)
					{
						case CH_MOVERT_MOVESTATE:

							//WHEN PLAYER MOVES, PLAY FX 
							if (!mAudioMgr->GetSound("Footsteps")->IsPlaying()) {
								mAudioMgr->GetSound("Footsteps")->Play(true); 
							}


							XMFLOAT3 goal = mHappyPath[0]->mPosition;

							goal.y = mModelInstance->mPosition.y;

							XMVECTOR goalPosVec = XMLoadFloat3(&goal);
							XMVECTOR modelPosVec = XMLoadFloat3(&mModelInstance->mPosition);
							XMVECTOR pointAtGoalVec = XMVectorSubtract(goalPosVec, modelPosVec);
							XMVECTOR goalDistanceVec = pointAtGoalVec;
							XMFLOAT3 goalDistance;							
							XMStoreFloat3(&goalDistance, XMVector3Length(goalDistanceVec));
							pointAtGoalVec = XMVector3Normalize(pointAtGoalVec);

							XMFLOAT3 moveDir;
							XMStoreFloat3(&moveDir, pointAtGoalVec);

							mModelInstance->mForwardVector = moveDir;

							mModelInstance->mPosition.x += moveDir.x * 30.0f * dt;
							mModelInstance->mPosition.y += moveDir.y * 30.0f * dt;
							mModelInstance->mPosition.z += moveDir.z * 30.0f * dt;													

							if (goalDistance.x < 5.0f)
							{
								mHappyPath.erase(mHappyPath.begin());

								//END MUSIC
								if (mHappyPath.size() == 0)
								{
									mMoveState = CH_PICKAC_MOVESTATE;
									mAudioMgr->GetSound("Footsteps")->Stop();
									mAudioMgr->GetSound("Footsteps")->SetPosition(0); 
								}

							}

						break;
					}
					break;

				break;
			}

		
		break;

		case CH_HURT_LIFESTATE:
		{
			mLifeStateTimer.Tick();

			//here we'd play a hurt animation

			if (mLifeStateTimer.TotalTime() >= 1.0f)
			{
				mLifeState = CH_OK_LIFESTATE;
				mLifeStateTimer.Reset();
			}

		}
		break;

		case CH_DYING_LIFESTATE:
		{
			//here we'd play a dying animation
			//however for now, lets just squash them

			mModelInstance->mScale.y -= 1.0f * dt;
			mModelInstance->mPosition.y -= 4.0f * dt;

			if (mModelInstance->mScale.y <= 0.1f)
			{
				mModelInstance->mScale.y = 0.1f;
				mModelInstance->mPosition.y = 1.0f;

				//COULD PLAY SOUND FX HERE

				mLifeState = CH_DEAD_LIFESTATE;
				mModelInstance->mCollidable = false;
			}
		}
		break;

	}

	mModelInstance->Update(dt, false);

}

void DPhoenix::CharacterClass::TakeDamage(int dmg)
{
	mHP -= dmg;
	
	if (mHP <= 0)
	{
		mHP = 0;
		mLifeState = CH_DYING_LIFESTATE;
	}
	else
	{
		mLifeState = CH_HURT_LIFESTATE;
		mLifeStateTimer.Reset();
	}
}

