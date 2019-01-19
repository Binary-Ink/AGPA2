#pragma once
#include "D3DUtil.h"
#include "CharacterClass.h"
#include "BTLTMap.h"
#include "Beacon.h"


namespace DPhoenix
{
	enum TeamStates
	{
		TM_OK_TEAMSTATE,
		TM_DYING_TEAMSTATE,
		TM_DEAD_TEAMSTATE
	};

	enum TeamTypes
	{
		PLAYER_TEAM,
		ENEMY_TEAM
	};

	class Beacon;

	class Team
	{
		//look at me breaking encapsulation again
		//if you want to fix this, points make prizes
		public:
			
			//member vars -------------------------------------
			std::vector<CharacterClass*> mTeamMembers;
			int mTPPool;
			int mMaxTPPool;
			int mTPTurnSnapshot;
			TeamStates mTeamState;
			int mSaves;
			TeamTypes mTeamType;
			int mCurrentMember;
			int mTurnsLeft;
			std::vector<AvailableActions> mAvailableActions;
			std::vector<std::string> mActionstext;
			std::string mStatustext;
			bool mNewActionSelectFlag;

			int lastLeaderIndexAI;

			//there might be more for this class yet
			//right now it's a useful place for other mechanics

			//member methods -----------------------------------
			//constructor / destructor
			Team();
			~Team();

			void CalculateTPPool();
			void ResetTeamTurn();

			void NextMember();
			void PrevMember();
			void SelectMember(int index);

			void CheckAvailableActions(std::vector<std::vector<MapBlock*>>& _map, int currentCol, int currentRow);
			void CheckSelectedAction(int index);
			
			void EndCurrentTurn();

			bool CheckTeamState();

			//standard methods for game loop
			//we might not use these
			void Update(float dt, BTLTMap* map);
			//void Draw();


			//AI Stuff --------------------------------------
			void DetermineAIStatus(BTLTMap* map, std::vector<Beacon*>& beaconVec);
			void DetermineMovement(BTLTMap* map, std::vector<Beacon*>& beaconVec, Team* opposingteam, std::vector<PrimitiveInstance*>& happyPath,
										PrimitiveInstance* selectionBox);
			void DetermineAction(BTLTMap* map, std::vector<Beacon*>& beaconVec, Team* opposingteam, 
									std::vector<PrimitiveInstance*>& targets, PrimitiveInstance* targetHighlight);
	};
}

