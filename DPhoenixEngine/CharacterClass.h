#pragma once
#include "D3DUtil.h"

namespace DPhoenix
{
	//this will be used to determine stat calculations, model loading
	//available actions, battle calculations etc.
	enum CharacterClasses
	{
		TOY_SOLDIER_CLASS,
		DARK_ANGEL_CLASS,
		DRAGON_CLASS,
		BIG_CAT_CLASS
	};

	//governs animations and available update logic
	enum LifeStates
	{
		CH_OK_LIFESTATE,
		CH_HURT_LIFESTATE,
		CH_DYING_LIFESTATE,
		CH_DEAD_LIFESTATE
	};

	//governs the turn-based system
	enum TurnStates
	{
		CH_WAIT_TURNSTATE,
		CH_ACTIVE_TURNSTATE,
		CH_COMP_TURNSTATE
	};

	//governs the actions perfomed during a turn
	enum MoveStates
	{
		CH_PICKRT_MOVESTATE,
		CH_MOVERT_MOVESTATE,
		CH_PICKAC_MOVESTATE,
		CH_DOAC_MOVESTATE
	};

	//animations required for control
	enum CharacterAnimations
	{
		IDLE_ANIMATION,
		DANCE_ANIMATION,
		MELEE_ANIMATION,
		MAGIC_ANIMATION,
		WEAPON_ANIMATION,
		HURT_ANIMATION,
		DYING_ANIMATION,
		DEAD_ANIMATION
	};


	enum AvailableActions
	{
		HIT_ACTION,
		PISTOL_ACTION,
		SHOTGUN_ACTION,
		SCALE_THROW_ACTION,
		CLAW_THROW_ACTION,
		ROCKET_LAUNCHER_ACTION,
		FIRE_ACTION,
		ICE_ACTION,
		LIGHTNING_ACTION,
		DARKNESS_ACTION,
		FURBALL_ARCANA_ACTION,
		HOLD_STILL_ACTION,
		LIGHT_BEACON_ACTION,
		NO_ACTION
	};

	enum AITypes {
		DEFENDER_AI,
		ATTACKER_AI

	};

	class CharacterClass
	{
		//breaking encapsulation -
		//really all these properties should have getters and setters
		//points for fixes!!!!
		public:
			//member vars --------------------------------------------

			//stats --------------------------------------------------
			int mLevel;
			int mBaseExp, mExp;
			int mBaseTP, mTP;
			int mBaseHP, mHP, mMaxHP;
			int mBaseMP, mMP, mMaxMP;
			int mBasePower, mPower;
			int mBaseFirepower, mFirepower;
			int mBaseDefense, mDefense;
			int mBaseAccuracy, mAccuracy;
			int mBaseEvasion, mEvasion;
			int mBaseMagic, mMagic;
			int mBaseCharm, mCharm;

			//character traits ---------------------------------------
			CharacterClasses mClass;
			//actions should go here when determined
			//models should go here when determined

			//TEMP MODELS TO BEGIN WITH --------------------------------------
			DPhoenix::GeometryGenerator* mGeoGen;
			DPhoenix::GeometryGenerator::MeshData* mBox;
			DPhoenix::PrimitiveInstance* mModelInstance;

			LifeStates mLifeState;
			TurnStates mTurnState;
			MoveStates mMoveState;
			CharacterAnimations mAnimation;
			AvailableActions mSelectedAction;

			GameTimer mLifeStateTimer;


			//FOR MOVEMENT
			std::vector<PrimitiveInstance*> mHappyPath;

			//pointers to main singletons / devices
			TextureMgr* mTexMgr;
			ID3D11Device* md3dDevice;
			AudioMgr* mAudioMgr;

			//FOR AI -------------------------------------------------
			AITypes mAIType;
			int mTargetBeaconId;
			bool mIsClockwiseAI;


			//methods ------------------------------------------------

			//constructor / destructor
			CharacterClass(CharacterClasses _class, TextureMgr* _texMgr, 
							ID3D11Device* _md3dDevice,	AudioMgr* _audioMgr,
							std::vector<PrimitiveInstance*> _happyPath);
			//copy constructor
			CharacterClass(const CharacterClass &character);
			~CharacterClass();

			//main methods
			//stat growth / calcs
			void SetBaseStats();
			void CalculateStats(int _level);
			//battle
			//(the modifier relates to type of magic / weapon)
			//(the coverDmg implies we'll figure out cover / wall collisions first)
			int MeleeAttack(CharacterClass* _target);
			int MagicAttack(CharacterClass* _target, int _modifier, int _coverDmg);
			int WeaponAttack(CharacterClass* _target, int _modifier, int _coverDmg);

			//logic / rendering
			void Update(float dt);
			//we're likely to instead call the draw method directly from
			//the linked model in the main demo rendering method
			//void Draw();
			void TakeDamage(int dmg);
			
	};

}


