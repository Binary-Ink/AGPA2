#include "Team.h"

DPhoenix::Team::Team()
{
	mTeamState = TM_OK_TEAMSTATE;

	mTPPool = 0;
	mMaxTPPool = 0;
	mSaves = 3;
	mTeamType = PLAYER_TEAM;

	mCurrentMember = 0;
	mTurnsLeft = 4;

	//ono, magic number...
	mActionstext.resize(13);

	mActionstext[HIT_ACTION] = "HIT (1 TP)";
	mActionstext[PISTOL_ACTION] = "PISTOL (2 TP)";
	mActionstext[SHOTGUN_ACTION] = "SHOTGUN (3 TP)";
	mActionstext[SCALE_THROW_ACTION] = "SCALE THROW (4 TP)";
	mActionstext[CLAW_THROW_ACTION] = "CLAW THROW (4 TP)";
	mActionstext[ROCKET_LAUNCHER_ACTION] = "ROCKET LAUNCHER (10 TP)";
	mActionstext[FIRE_ACTION] = "FIRE (2 TP) / (25 MP)";
	mActionstext[ICE_ACTION] = "ICE (3 TP) / (25 MP)";
	mActionstext[LIGHTNING_ACTION] = "LIGHTNING (4 TP) / (50 MP)";
	mActionstext[DARKNESS_ACTION] = "DARKNESS (5 TP) / (100 MP)";
	mActionstext[FURBALL_ARCANA_ACTION] = "FURBALL (10 TP) / (50 MP)";
	mActionstext[HOLD_STILL_ACTION] = "HOLD STILL";
	mActionstext[LIGHT_BEACON_ACTION] = "LIGHT BEACON";
		
	mStatustext = "";

	mNewActionSelectFlag = false;

	lastLeaderIndexAI = 0;
}

DPhoenix::Team::~Team()
{
}

void DPhoenix::Team::CalculateTPPool()
{
	for (int i = 0; i < mTeamMembers.size(); i++)
	{
		mMaxTPPool += mTeamMembers[i]->mTP;
	}
	mTPPool = mTPTurnSnapshot = mMaxTPPool;

}

void DPhoenix::Team::ResetTeamTurn()
{
	for (int i = 0; i < mTeamMembers.size(); i++)
	{
		mTeamMembers[i]->mTurnState = CH_WAIT_TURNSTATE;
		mTeamMembers[i]->mMoveState = CH_PICKRT_MOVESTATE;
	}
	
	mCurrentMember = 0;
	mTPPool = mTPTurnSnapshot = mMaxTPPool;

	mTurnsLeft = 0;

	//check for death!
	for (int i = mTeamMembers.size() - 1; i > -1; i--)
	{
		if (mTeamMembers[i]->mLifeState != CH_DEAD_LIFESTATE)
		{
			mTurnsLeft++;
			mCurrentMember = i;
		}
	}

	mTeamMembers[mCurrentMember]->mTurnState = CH_ACTIVE_TURNSTATE;
}

void DPhoenix::Team::NextMember()
{
	if ((mTeamMembers[mCurrentMember]->mTurnState == CH_ACTIVE_TURNSTATE &&
		mTeamMembers[mCurrentMember]->mMoveState == CH_PICKRT_MOVESTATE) ||
		mTeamMembers[mCurrentMember]->mTurnState == CH_COMP_TURNSTATE)
	{
		mTPPool = mTPTurnSnapshot;

		bool selected = false;
		if(mTeamMembers[mCurrentMember]->mTurnState != CH_COMP_TURNSTATE)
			mTeamMembers[mCurrentMember]->mTurnState = CH_WAIT_TURNSTATE;

		while (!selected)
		{
			mCurrentMember++;
			if (mCurrentMember > mTeamMembers.size() - 1)
				mCurrentMember = 0;

			if (mTeamMembers[mCurrentMember]->mTurnState == CH_WAIT_TURNSTATE &&
				mTeamMembers[mCurrentMember]->mLifeState != CH_DEAD_LIFESTATE)
			{
				selected = true;
				mTeamMembers[mCurrentMember]->mTurnState = CH_ACTIVE_TURNSTATE;
				mTeamMembers[mCurrentMember]->mMoveState = CH_PICKRT_MOVESTATE;
			}
		}
	}
}

void DPhoenix::Team::PrevMember()
{
	if ((mTeamMembers[mCurrentMember]->mTurnState == CH_ACTIVE_TURNSTATE &&
		mTeamMembers[mCurrentMember]->mMoveState == CH_PICKRT_MOVESTATE) ||
		mTeamMembers[mCurrentMember]->mTurnState == CH_COMP_TURNSTATE)
	{
		mTPPool = mTPTurnSnapshot;

		bool selected = false;
		if (mTeamMembers[mCurrentMember]->mTurnState != CH_COMP_TURNSTATE)
			mTeamMembers[mCurrentMember]->mTurnState = CH_WAIT_TURNSTATE;

		while (!selected)
		{
			mCurrentMember--;
			if (mCurrentMember < 0)
				mCurrentMember = mTeamMembers.size() - 1;

			if (mTeamMembers[mCurrentMember]->mTurnState == CH_WAIT_TURNSTATE &&
				mTeamMembers[mCurrentMember]->mLifeState != CH_DEAD_LIFESTATE)
			{
				selected = true;
				mTeamMembers[mCurrentMember]->mTurnState = CH_ACTIVE_TURNSTATE;
			}
		}
	}
}

void DPhoenix::Team::SelectMember(int index)
{
	if (mCurrentMember == index)
		return;

	if (mTeamMembers[index]->mTurnState == CH_COMP_TURNSTATE)
		return;

	mTPPool = mTPTurnSnapshot;

	if (mTeamMembers[index]->mLifeState != CH_DEAD_LIFESTATE)
	{
		mTeamMembers[index]->mTurnState = CH_ACTIVE_TURNSTATE;
		mCurrentMember = index;
	}

	for (int i = 0; i < mTeamMembers.size(); i++)
	{
		if (i != index)
		{
			if (mTeamMembers[i]->mTurnState == CH_ACTIVE_TURNSTATE)
				mTeamMembers[i]->mTurnState = CH_WAIT_TURNSTATE;
		}
	}

}

void DPhoenix::Team::CheckAvailableActions(std::vector<std::vector<MapBlock*>>& _map, int currentCol, int currentRow)
{
	mAvailableActions.clear();

	bool canHit = false;

	if (mTeamType == PLAYER_TEAM)
	{
		if (currentCol - 1 > 0)
		{
			if(_map[currentCol - 1][currentRow]->mSpecialFlag == ENEMY_SP_FLAG)
				canHit = true;
		}

		if (currentCol + 1 < _map.size())
		{
			if (_map[currentCol + 1][currentRow]->mSpecialFlag == ENEMY_SP_FLAG)
				canHit = true;
		}

		if (currentRow - 1 > 0)
		{
			if (_map[currentCol][currentRow - 1]->mSpecialFlag == ENEMY_SP_FLAG)
				canHit = true;
		}

		if (currentRow + 1 < _map[0].size())
		{
			if (_map[currentCol][currentRow + 1]->mSpecialFlag == ENEMY_SP_FLAG)
				canHit = true;
		}

	}
	else if (mTeamType == ENEMY_TEAM)
	{
		//check each adjacent square to see if adversary available to hit
		if (currentCol - 1 > 0)
		{
			if (_map[currentCol - 1][currentRow]->mSpecialFlag == PLAYER_SP_FLAG)
				canHit = true;
		}

		if (currentCol + 1 < _map.size())
		{
			if (_map[currentCol + 1][currentRow]->mSpecialFlag == PLAYER_SP_FLAG)
				canHit = true;
		}

		if (currentRow - 1 > 0)
		{
			if (_map[currentCol][currentRow - 1]->mSpecialFlag == PLAYER_SP_FLAG)
				canHit = true;
		}

		if (currentRow + 1 < _map[0].size())
		{
			if (_map[currentCol][currentRow + 1]->mSpecialFlag == PLAYER_SP_FLAG)
				canHit = true;
		}
	}

	if (canHit && mTPPool >= 1)
	{
		mAvailableActions.push_back(HIT_ACTION);
	}


	switch (mTeamMembers[mCurrentMember]->mClass)
	{
		case TOY_SOLDIER_CLASS:

			if(mTPPool >= 2)
				mAvailableActions.push_back(PISTOL_ACTION);
			if (mTPPool >= 3)
				mAvailableActions.push_back(SHOTGUN_ACTION);
			if (mTPPool >= 10)
				mAvailableActions.push_back(ROCKET_LAUNCHER_ACTION);

		break;
		case DARK_ANGEL_CLASS:

			if (mTPPool >= 2 && mTeamMembers[mCurrentMember]->mMP >= 25)
				mAvailableActions.push_back(FIRE_ACTION);
			if (mTPPool >= 3 && mTeamMembers[mCurrentMember]->mMP >= 25)
				mAvailableActions.push_back(ICE_ACTION);
			if (mTPPool >= 4 && mTeamMembers[mCurrentMember]->mMP >= 50)
				mAvailableActions.push_back(LIGHTNING_ACTION);
			if (mTPPool >= 5 && mTeamMembers[mCurrentMember]->mMP >= 100)
				mAvailableActions.push_back(DARKNESS_ACTION);

		break;
		case DRAGON_CLASS:

			if (mTPPool >= 2 && mTeamMembers[mCurrentMember]->mMP >= 25)
				mAvailableActions.push_back(FIRE_ACTION);
			if (mTPPool >= 3 && mTeamMembers[mCurrentMember]->mMP >= 25)
				mAvailableActions.push_back(ICE_ACTION);
			if (mTPPool >= 4)
				mAvailableActions.push_back(SCALE_THROW_ACTION);

		break;
		case BIG_CAT_CLASS:

			if (mTPPool >= 10 && mTeamMembers[mCurrentMember]->mMP >= 50)
				mAvailableActions.push_back(FURBALL_ARCANA_ACTION);
			if (mTPPool >= 5)
				mAvailableActions.push_back(CLAW_THROW_ACTION);

		break;
	}

	mAvailableActions.push_back(HOLD_STILL_ACTION);

	if (currentCol - 1 > 0)
	{
		if (_map[currentCol - 1][currentRow]->mMapBlockType == BEACON_MAPBLOCK)
			mAvailableActions.push_back(LIGHT_BEACON_ACTION);
	}

	if (currentCol + 1 < _map.size())
	{
		if (_map[currentCol + 1][currentRow]->mMapBlockType == BEACON_MAPBLOCK)
			mAvailableActions.push_back(LIGHT_BEACON_ACTION);
	}

	if (currentRow - 1 > 0)
	{
		if (_map[currentCol][currentRow - 1]->mMapBlockType == BEACON_MAPBLOCK)
			mAvailableActions.push_back(LIGHT_BEACON_ACTION);
	}

	if (currentRow + 1 < _map[0].size())
	{
		if (_map[currentCol][currentRow + 1]->mMapBlockType == BEACON_MAPBLOCK)
			mAvailableActions.push_back(LIGHT_BEACON_ACTION);
	}

}

void DPhoenix::Team::CheckSelectedAction(int index)
{
	switch (mAvailableActions[index])
	{
		case HIT_ACTION:
		case PISTOL_ACTION:
		case SHOTGUN_ACTION:
		case SCALE_THROW_ACTION:
		case CLAW_THROW_ACTION:
		case ROCKET_LAUNCHER_ACTION:
		case FIRE_ACTION:
		case ICE_ACTION:
		case LIGHTNING_ACTION:
		case DARKNESS_ACTION:
		case FURBALL_ARCANA_ACTION:
		case LIGHT_BEACON_ACTION:
			mTeamMembers[mCurrentMember]->mSelectedAction = mAvailableActions[index];
			mNewActionSelectFlag = true;
			break;
		case HOLD_STILL_ACTION:
			mTPTurnSnapshot = mTPPool;
			EndCurrentTurn();			
			break;
	}
}

void DPhoenix::Team::EndCurrentTurn()
{
	mTeamMembers[mCurrentMember]->mTurnState = CH_COMP_TURNSTATE;
	mTurnsLeft--;
	if (mTurnsLeft > 0)
	{
		NextMember();
	}
}

bool DPhoenix::Team::CheckTeamState()
{
	bool isAlive = true;
	int countTheDead = 0;

	for (int i = 0; i < mTeamMembers.size(); i++)
	{
		if (mTeamMembers[i]->mLifeState == CH_DEAD_LIFESTATE)
		{
			countTheDead++;
		}
	}

	if (mTeamMembers.size() == countTheDead)
	{
		isAlive = false;
		mTeamState = TM_DEAD_TEAMSTATE;
	}

	return isAlive;
}

void DPhoenix::Team::Update(float dt, BTLTMap* map)
{	
	//need to update SP flags where necessary
	for (int i = 0; i < mTeamMembers.size(); i++)
	{		
		int col = map->GetColFromPosition(mTeamMembers[i]->mModelInstance->mPosition);
		int row = map->GetRowFromPosition(mTeamMembers[i]->mModelInstance->mPosition);
		map->mTiles[col][row]->mSpecialFlag = NO_SP_FLAG;

		mTeamMembers[i]->Update(dt);

		col = map->GetColFromPosition(mTeamMembers[i]->mModelInstance->mPosition);
		row = map->GetRowFromPosition(mTeamMembers[i]->mModelInstance->mPosition);
		
		if(mTeamType == PLAYER_TEAM && mTeamMembers[i]->mLifeState != CH_DEAD_LIFESTATE)
			map->mTiles[col][row]->mSpecialFlag = PLAYER_SP_FLAG;

		if (mTeamType == ENEMY_TEAM && mTeamMembers[i]->mLifeState != CH_DEAD_LIFESTATE)
			map->mTiles[col][row]->mSpecialFlag = ENEMY_SP_FLAG;

		switch (mTeamMembers[mCurrentMember]->mMoveState)
		{
			case CH_PICKRT_MOVESTATE:
				mStatustext = "Pick route";
			break;
			case CH_MOVERT_MOVESTATE:
				mStatustext = "Move route";
			break;
			case CH_PICKAC_MOVESTATE:
				mStatustext = "Pick action";
			break;
			case CH_DOAC_MOVESTATE:
				mStatustext = "Do action";
			break;
		}


	}
}

void DPhoenix::Team::DetermineAIStatus(BTLTMap * map, std::vector<Beacon*>& beaconVec)
{
	//AI is deternmined by how many beacons lit - enemy players will go for the beacons
	//first and the go for the player
	//this is determined on a team-turn by team-turn basis

	//AI not very bright! Will divvy up beacons between themselves
	//head for them without attacking player until beacons lit
	//only in attack mode wil they consider actually attacking
	//this will mean that the first turn will move a lot and the others won't 
	//hence a flag of the last 'leader' will be kept to move first

	//if next to beacon (and defender), light it
	//if next to player, melee attack it
	//if attacking but not near player, attempt to fire at it
	//(some randomness involved here)

	lastLeaderIndexAI++;

	//assert statements! There shoud be more of these.............
	//*STRONG HINT* --------------------------------------------
	//anyway, we need 4 of each to make this loop work as expected
	assert(beaconVec.size() == 4);
	assert(mTeamMembers.size() == 4);

	for (int i = 0; i < beaconVec.size(); i++)
	{
		if (beaconVec[i]->mBeaconState != BEACON_ENEMY_LIT)
		{
			mTeamMembers[i]->mAIType = DEFENDER_AI;
			mTeamMembers[i]->mTargetBeaconId = i;
		}
		else
		{
			mTeamMembers[i]->mAIType = ATTACKER_AI;
			mTeamMembers[i]->mTargetBeaconId = i;
		}
	}

}

void DPhoenix::Team::DetermineMovement(BTLTMap * map, std::vector<Beacon*>& beaconVec, Team* opposingteam, std::vector<PrimitiveInstance*>& happyPath,
										PrimitiveInstance* selectionBox)
{
	//At this point it is a specific character's turn
	//depending on their AI type, they will either pursue the target beacon (if enough turn points are available)
	//or they will find the nearest player character as a target and pursue them

	//this method is meant to determine the happy path auto-magically 
	//the player doesn't need the selections displayed so teh happy path can just 'appear'

	//this method should determine the full happy path ready to move along it
	//it will determine the happy path on one update - it doesn't need to go one-by-one
	//against the CPU, there's no need for unnecessary delays
	//HOWEVER, it should be fair

	//This method should only be called if current ENEMY team member
	//is in PICKRT actionstate

	if (mTeamMembers[mCurrentMember]->mAIType == DEFENDER_AI)
	{
		//need to get the character's grid position relative to the beacon position
		int currentCharRow = map->GetRowFromPosition(mTeamMembers[mCurrentMember]->mModelInstance->mPosition);
		int currentCharCol = map->GetColFromPosition(mTeamMembers[mCurrentMember]->mModelInstance->mPosition);

		int targetBeaconRow = map->GetRowFromPosition(
			beaconVec[mTeamMembers[mCurrentMember]->mTargetBeaconId]->mPoleMeshInstance->mPosition
		);
		int targetBeaconCol = map->GetColFromPosition(
			beaconVec[mTeamMembers[mCurrentMember]->mTargetBeaconId]->mPoleMeshInstance->mPosition
		);

		//need to add the begining of te happy path if not there already
		if (happyPath.size() < 1)
		{
			happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
			happyPath.back()->mPosition = mTeamMembers[mCurrentMember]->mModelInstance->mPosition;
			happyPath.back()->mPosition.y = 10.5f;
			happyPath.back()->mOpacity = 0.75f;
		}

		//lovely, now if we are already next to it, brucey bonus, we casn move on teh state to then light it
		//if not then we neeed to loop through our available happy path to move as close as possible

		//here's the initial checks to set the flag
		bool isNextToBeacon = false;

		//need to separately check for adjacent row or adjacent column
		if (
			(currentCharRow == targetBeaconRow && (abs(currentCharCol - targetBeaconCol) == 1)) || 
			(currentCharCol == targetBeaconCol && (abs(currentCharRow - targetBeaconRow) == 1))
			)
		{
			isNextToBeacon = true;
		}

		if (isNextToBeacon)
		{
			mTeamMembers[mCurrentMember]->mMoveState = CH_MOVERT_MOVESTATE;
		}
		else
		{
			//here we now need to construct the happy path
			//next thing we need to do is build up the happyPath
			//sooooo ----- based upon:
				//our position
				//remaining turn points
				//available moves
				//way to the beacon
			//we then add to it
			//heeeeeere goess.........................

			//OK, start with turns left --------------
			//then see what available moves there are
			//then determine based on vector to beacon which is the best to choose
			//rotate 'clockwise' if available moves not 'great' 
			
			//UH OH!!!!!
			//NEED TO ALSO CHECK WE ARE NOT GOING BACK ON THE PATH
			//AHHHHHHHHHHHH--------------------


			int mTurnPointsToLeave = mTurnsLeft * 2;
			XMFLOAT3 leftPos, rightPos, upPos, downPos;
			

			//need to take off a TP each turn
			for (int i = mTPPool; i > mTurnPointsToLeave && !isNextToBeacon; i--, mTPPool--)
			{
				bool happyPathAddedto = false;

				//right, what's our available turns first off
				bool left, up, right, down;
				left = up = right = down = false;

				int happyPathRow = map->GetRowFromPosition(happyPath.back()->mPosition);
				int happyPathCol = map->GetColFromPosition(happyPath.back()->mPosition);

				if (happyPathCol - 1 > -1)
					if ((map->mTiles[happyPathCol - 1][happyPathRow]->mMapBlockType == DPhoenix::FLOOR_MAPBLOCK &&
						map->mTiles[happyPathCol - 1][happyPathRow]->mSpecialFlag == DPhoenix::NO_SP_FLAG))
					{
						left = true;
						leftPos = map->GetPositionAboveFromMapRef(happyPathCol - 1, happyPathRow);
					}

				if (happyPathCol + 1 < map->mWidth)
					if (map->mTiles[happyPathCol + 1][happyPathRow]->mMapBlockType == DPhoenix::FLOOR_MAPBLOCK &&
						map->mTiles[happyPathCol + 1][happyPathRow]->mSpecialFlag == DPhoenix::NO_SP_FLAG)
					{
						right = true;
						rightPos = map->GetPositionAboveFromMapRef(happyPathCol + 1, happyPathRow);
					}

				if (happyPathRow - 1 > -1)
					if (map->mTiles[happyPathCol][happyPathRow - 1]->mMapBlockType == DPhoenix::FLOOR_MAPBLOCK &&
						map->mTiles[happyPathCol][happyPathRow - 1]->mSpecialFlag == DPhoenix::NO_SP_FLAG)
					{
						up = true;
						upPos = map->GetPositionAboveFromMapRef(happyPathCol, happyPathRow - 1);
					}

				if (happyPathRow + 1 < map->mLength)
					if (map->mTiles[happyPathCol][happyPathRow + 1]->mMapBlockType == DPhoenix::FLOOR_MAPBLOCK &&
						map->mTiles[happyPathCol][happyPathRow + 1]->mSpecialFlag == DPhoenix::NO_SP_FLAG)
					{
						down = true;
						downPos = map->GetPositionAboveFromMapRef(happyPathCol, happyPathRow + 1);
					}

				//here's how we erase valid moves if our happy path already has it
				//a bit like Nokia Snake I guess
				for (int j = 0; j < happyPath.size(); j++)
				{
					int currentHappyPathRow = map->GetRowFromPosition(happyPath[j]->mPosition);
					int currentHappyPathCol = map->GetColFromPosition(happyPath[j]->mPosition);

					//oh dear, now to actually check....

					if (happyPathRow + 1 == currentHappyPathRow && happyPathCol == currentHappyPathCol)
						down = false;

					if (happyPathRow - 1 == currentHappyPathRow && happyPathCol == currentHappyPathCol)
						up = false;

					if (happyPathRow == currentHappyPathRow && happyPathCol + 1 == currentHappyPathCol)
						right = false;

					if (happyPathRow == currentHappyPathRow && happyPathCol - 1 == currentHappyPathCol)
						left = false;					
				}

				//ok, got our turns, so which is the best move for us??????
				//first we determine where our target beacon is in relation to us and try that block first
				//based on manhattan distance (ish)
				//ooorrrrrrr........
				bool isLeft, isUp, isRight, isDown;
				isLeft = isUp = isRight = isDown = false;
				
				if (happyPathCol - targetBeaconCol > 0)
					isLeft = true;
				if (happyPathCol - targetBeaconCol < 0)
					isRight = true;
				if (happyPathRow - targetBeaconRow > 0)
					isUp = true;
				if (happyPathRow - targetBeaconRow < 0)
					isDown = true;

				//ok let's go the clockwise - lets go round again!	
				//ACTUALLY, THE AI CAN GET STUCK FOR CERTAIN BEACONS 
				//HENCE EACH CHARACTER TURN THE ORDER WILL FLIP
				//THAT WILL MEAN THEY OPNLY PICK A SILLY ROUTE ONCE, RIgHT?????

				if (mTeamMembers[mCurrentMember]->mIsClockwiseAI)
				{
					if (left && isLeft)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = leftPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
					else if (up && isUp)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = upPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
					else if (right && isRight)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = rightPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
					else if (down && isDown)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = downPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
				}
				else
				{
					if (down && isDown)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = downPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
					else if (right && isRight)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = rightPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
					else if (up && isUp)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = upPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
					else if (left && isLeft)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = leftPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
				}

				//ok we've picked our best move right????
				//well, if not, it's time to try alternatives in a clockwise fashion
				//ORRRRRR ANTI-CLOCKWISE


				if (!happyPathAddedto)
				{
					if (mTeamMembers[mCurrentMember]->mIsClockwiseAI)
					{
						if (left)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = leftPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
						else if (up)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = upPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
						else if (right)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = rightPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
						else if (down)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = downPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
					}
					else
					{
						if (down)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = downPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
						else if (right)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = rightPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
						else if (up)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = upPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
						else if (left)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = leftPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
					}
				}

				//lovely, now is the time to check if we're next to a beacon
				//in which case let's quit

				if (happyPathAddedto)
				{
					happyPathRow = map->GetRowFromPosition(happyPath.back()->mPosition);
					happyPathCol = map->GetColFromPosition(happyPath.back()->mPosition);

					if (happyPathCol - 1 > -1)
						if (map->mTiles[happyPathCol - 1][happyPathRow]->mMapBlockType == DPhoenix::BEACON_MAPBLOCK)
							isNextToBeacon = true;

					if (happyPathCol + 1 < map->mWidth)
						if (map->mTiles[happyPathCol + 1][happyPathRow]->mMapBlockType == DPhoenix::BEACON_MAPBLOCK)
							isNextToBeacon = true;

					if (happyPathRow - 1 > -1)
						if (map->mTiles[happyPathCol][happyPathRow - 1]->mMapBlockType == DPhoenix::BEACON_MAPBLOCK)
							isNextToBeacon = true;

					if (happyPathRow + 1 < map->mLength)
						if (map->mTiles[happyPathCol][happyPathRow + 1]->mMapBlockType == DPhoenix::BEACON_MAPBLOCK)
							isNextToBeacon = true;
					
				}

				if (!happyPathAddedto)
					break;

			}
		}

		mTeamMembers[mCurrentMember]->mMoveState = CH_MOVERT_MOVESTATE;
		mTeamMembers[mCurrentMember]->mHappyPath = happyPath;

		if (mTeamMembers[mCurrentMember]->mIsClockwiseAI)
		{
			mTeamMembers[mCurrentMember]->mIsClockwiseAI = false;
		}
		else
		{
			mTeamMembers[mCurrentMember]->mIsClockwiseAI = true;
		}

	}

	if (mTeamMembers[mCurrentMember]->mAIType == ATTACKER_AI)
	{
		//need to get the character's grid position to check proximity to players
		int currentCharRow = map->GetRowFromPosition(mTeamMembers[mCurrentMember]->mModelInstance->mPosition);
		int currentCharCol = map->GetColFromPosition(mTeamMembers[mCurrentMember]->mModelInstance->mPosition);

		//this is for storing the id of the intended target
		int targetIndex0distance, targetIndex1distance, targetIndex2distance, targetIndex3distance;
		targetIndex0distance = targetIndex1distance = targetIndex2distance = targetIndex3distance = 0;

		int targetRow;
		int targetCol;

		//DEATH - IF A CHARACTER DIES WE ELIMINATE THEM AS A TARGET 
		//IF All DIE THIS WILL BREAK, HOWEVER Teh MAIn LOOP AT THAT POINT
		//SHOULD SWITCH TO GAME_OVER_STATE
		
		//then we need to see which player is nearest and assign them as the target
		for (int i = 0; i < opposingteam->mTeamMembers.size(); i++)
		{
			int currentTargetRow = 0;
			int currentTargetCol = 0;

			if (opposingteam->mTeamMembers[i]->mLifeState == CH_DEAD_LIFESTATE)
			{
				//set absurdly high so it never meets the minimum distance check
				currentTargetRow = 9999;
				currentTargetCol = 9999;
			}
			else
			{
				currentTargetRow = map->GetRowFromPosition(opposingteam->mTeamMembers[i]->mModelInstance->mPosition);
				currentTargetCol = map->GetColFromPosition(opposingteam->mTeamMembers[i]->mModelInstance->mPosition);
			}			

			switch (i)
			{
				case 0:
					targetIndex0distance = abs(currentCharRow - currentTargetRow) + abs(currentCharCol - currentTargetCol);
					break;
				case 1:
					targetIndex1distance = abs(currentCharRow - currentTargetRow) + abs(currentCharCol - currentTargetCol);
					break;
				case 2:
					targetIndex2distance = abs(currentCharRow - currentTargetRow) + abs(currentCharCol - currentTargetCol);
					break;
				case 3:
					targetIndex3distance = abs(currentCharRow - currentTargetRow) + abs(currentCharCol - currentTargetCol);
					break;
			}
		}

		int minDistance = min(targetIndex0distance, min(targetIndex1distance, min(targetIndex2distance, targetIndex3distance)));

		if (minDistance == targetIndex0distance)
		{
			targetRow = map->GetRowFromPosition(opposingteam->mTeamMembers[0]->mModelInstance->mPosition);
			targetCol = map->GetColFromPosition(opposingteam->mTeamMembers[0]->mModelInstance->mPosition);
		} 
		else if (minDistance == targetIndex1distance)
		{
			targetRow = map->GetRowFromPosition(opposingteam->mTeamMembers[1]->mModelInstance->mPosition);
			targetCol = map->GetColFromPosition(opposingteam->mTeamMembers[1]->mModelInstance->mPosition);
		}
		else if (minDistance == targetIndex2distance)
		{
			targetRow = map->GetRowFromPosition(opposingteam->mTeamMembers[2]->mModelInstance->mPosition);
			targetCol = map->GetColFromPosition(opposingteam->mTeamMembers[2]->mModelInstance->mPosition);
		}
		else
		{
			targetRow = map->GetRowFromPosition(opposingteam->mTeamMembers[3]->mModelInstance->mPosition);
			targetCol = map->GetColFromPosition(opposingteam->mTeamMembers[3]->mModelInstance->mPosition);
		}

		//YAYA!!!!! We HAVE OUR TARGET - NOw LET'S MOVE TOWArDS IT
		//HERE FOlLOWS A barelY edited copypasta of what we did for beacons

		//need to add the begining of te happy path if not there already
		if (happyPath.size() < 1)
		{
			happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
			happyPath.back()->mPosition = mTeamMembers[mCurrentMember]->mModelInstance->mPosition;
			happyPath.back()->mPosition.y = 10.5f;
			happyPath.back()->mOpacity = 0.75f;
		}

		//lovely, now if we are already next to it, brucey bonus, we casn move on teh state to then light it
		//if not then we neeed to loop through our available happy path to move as close as possible

		//here's the initial checks to set the flag
		bool isNextToTarget = false;

		//need to separately check for adjacent row or adjacent column
		if (
			(currentCharRow == targetRow && (abs(currentCharCol - targetCol) == 1)) ||
			(currentCharCol == targetCol && (abs(currentCharRow - targetRow) == 1))
			)
		{
			isNextToTarget = true;
		}

		if (isNextToTarget)
		{
			mTeamMembers[mCurrentMember]->mMoveState = CH_MOVERT_MOVESTATE;
		}
		else
		{

			int mTurnPointsToLeave = mTurnsLeft * 2;
			XMFLOAT3 leftPos, rightPos, upPos, downPos;


			//need to take off a TP each turn
			for (int i = mTPPool; i > mTurnPointsToLeave && !isNextToTarget; i--, mTPPool--)
			{
				bool happyPathAddedto = false;

				//right, what's our available turns first off
				bool left, up, right, down;
				left = up = right = down = false;

				int happyPathRow = map->GetRowFromPosition(happyPath.back()->mPosition);
				int happyPathCol = map->GetColFromPosition(happyPath.back()->mPosition);

				if (happyPathCol - 1 > -1)
					if ((map->mTiles[happyPathCol - 1][happyPathRow]->mMapBlockType == DPhoenix::FLOOR_MAPBLOCK &&
						map->mTiles[happyPathCol - 1][happyPathRow]->mSpecialFlag == DPhoenix::NO_SP_FLAG))
					{
						left = true;
						leftPos = map->GetPositionAboveFromMapRef(happyPathCol - 1, happyPathRow);
					}

				if (happyPathCol + 1 < map->mWidth)
					if (map->mTiles[happyPathCol + 1][happyPathRow]->mMapBlockType == DPhoenix::FLOOR_MAPBLOCK &&
						map->mTiles[happyPathCol + 1][happyPathRow]->mSpecialFlag == DPhoenix::NO_SP_FLAG)
					{
						right = true;
						rightPos = map->GetPositionAboveFromMapRef(happyPathCol + 1, happyPathRow);
					}

				if (happyPathRow - 1 > -1)
					if (map->mTiles[happyPathCol][happyPathRow - 1]->mMapBlockType == DPhoenix::FLOOR_MAPBLOCK &&
						map->mTiles[happyPathCol][happyPathRow - 1]->mSpecialFlag == DPhoenix::NO_SP_FLAG)
					{
						up = true;
						upPos = map->GetPositionAboveFromMapRef(happyPathCol, happyPathRow - 1);
					}

				if (happyPathRow + 1 < map->mLength)
					if (map->mTiles[happyPathCol][happyPathRow + 1]->mMapBlockType == DPhoenix::FLOOR_MAPBLOCK &&
						map->mTiles[happyPathCol][happyPathRow + 1]->mSpecialFlag == DPhoenix::NO_SP_FLAG)
					{
						down = true;
						downPos = map->GetPositionAboveFromMapRef(happyPathCol, happyPathRow + 1);
					}

				//here's how we erase valid moves if our happy path already has it
				//a bit like Nokia Snake I guess
				for (int j = 0; j < happyPath.size(); j++)
				{
					int currentHappyPathRow = map->GetRowFromPosition(happyPath[j]->mPosition);
					int currentHappyPathCol = map->GetColFromPosition(happyPath[j]->mPosition);

					//oh dear, now to actually check....

					if (happyPathRow + 1 == currentHappyPathRow && happyPathCol == currentHappyPathCol)
						down = false;

					if (happyPathRow - 1 == currentHappyPathRow && happyPathCol == currentHappyPathCol)
						up = false;

					if (happyPathRow == currentHappyPathRow && happyPathCol + 1 == currentHappyPathCol)
						right = false;

					if (happyPathRow == currentHappyPathRow && happyPathCol - 1 == currentHappyPathCol)
						left = false;
				}

				//ok, got our turns, so which is the best move for us??????
				//first we determine where our target beacon is in relation to us and try that block first
				//based on manhattan distance (ish)
				//ooorrrrrrr........
				bool isLeft, isUp, isRight, isDown;
				isLeft = isUp = isRight = isDown = false;

				if (happyPathCol - targetCol > 0)
					isLeft = true;
				if (happyPathCol - targetCol < 0)
					isRight = true;
				if (happyPathRow - targetRow > 0)
					isUp = true;
				if (happyPathRow - targetRow < 0)
					isDown = true;

				//ok let's go the clockwise - lets go round again!	
				//ACTUALLY, THE AI CAN GET STUCK FOR CERTAIN BEACONS 
				//HENCE EACH CHARACTER TURN THE ORDER WILL FLIP
				//THAT WILL MEAN THEY OPNLY PICK A SILLY ROUTE ONCE, RIgHT?????

				if (mTeamMembers[mCurrentMember]->mIsClockwiseAI)
				{
					if (left && isLeft)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = leftPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
					else if (up && isUp)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = upPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
					else if (right && isRight)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = rightPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
					else if (down && isDown)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = downPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
				}
				else
				{
					if (down && isDown)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = downPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
					else if (right && isRight)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = rightPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
					else if (up && isUp)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = upPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
					else if (left && isLeft)
					{
						happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
						happyPath.back()->mPosition = leftPos;
						happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
						happyPath.back()->mPosition.y = 10.5f;
						happyPath.back()->mOpacity = 0.75f;
						happyPathAddedto = true;
					}
				}

				//ok we've picked our best move right????
				//well, if not, it's time to try alternatives in a clockwise fashion
				//ORRRRRR ANTI-CLOCKWISE


				if (!happyPathAddedto)
				{
					if (mTeamMembers[mCurrentMember]->mIsClockwiseAI)
					{
						if (left)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = leftPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
						else if (up)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = upPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
						else if (right)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = rightPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
						else if (down)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = downPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
					}
					else
					{
						if (down)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = downPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
						else if (right)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = rightPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
						else if (up)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = upPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
						else if (left)
						{
							happyPath.push_back(new DPhoenix::PrimitiveInstance(*selectionBox));
							happyPath.back()->mPosition = leftPos;
							happyPath.back()->mPrevPosition = happyPath.back()->mPosition;
							happyPath.back()->mPosition.y = 10.5f;
							happyPath.back()->mOpacity = 0.75f;
							happyPathAddedto = true;
						}
					}
				}

				//lovely, now is the time to check if we're next to a beacon
				//in which case let's quit

				if (happyPathAddedto)
				{
					//WE'LL TRUN OFF THE SP FLAGS ON DEATH SO NO 'FALSE POSITIVES' ON BEInG
					//NEXT TO A PLAYER

					happyPathRow = map->GetRowFromPosition(happyPath.back()->mPosition);
					happyPathCol = map->GetColFromPosition(happyPath.back()->mPosition);

					if (happyPathCol - 1 > -1)
						if (map->mTiles[happyPathCol - 1][happyPathRow]->mSpecialFlag == DPhoenix::PLAYER_SP_FLAG)
							isNextToTarget = true;

					if (happyPathCol + 1 < map->mWidth)
						if (map->mTiles[happyPathCol + 1][happyPathRow]->mSpecialFlag == DPhoenix::PLAYER_SP_FLAG)
							isNextToTarget = true;

					if (happyPathRow - 1 > -1)
						if (map->mTiles[happyPathCol][happyPathRow - 1]->mSpecialFlag == DPhoenix::PLAYER_SP_FLAG)
							isNextToTarget = true;

					if (happyPathRow + 1 < map->mLength)
						if (map->mTiles[happyPathCol][happyPathRow + 1]->mSpecialFlag == DPhoenix::PLAYER_SP_FLAG)
							isNextToTarget = true;

				}

				if (!happyPathAddedto)
					break;

			}
		}

		mTeamMembers[mCurrentMember]->mMoveState = CH_MOVERT_MOVESTATE;
		mTeamMembers[mCurrentMember]->mHappyPath = happyPath;

		if (mTeamMembers[mCurrentMember]->mIsClockwiseAI)
		{
			mTeamMembers[mCurrentMember]->mIsClockwiseAI = false;
		}
		else
		{
			mTeamMembers[mCurrentMember]->mIsClockwiseAI = true;
		}


	}

}

void DPhoenix::Team::DetermineAction(BTLTMap * map, std::vector<Beacon*>& beaconVec, Team* opposingteam,
									std::vector<PrimitiveInstance*>& targets, PrimitiveInstance* targetHighlight)
{
	//THe MAIn DEMo SHOULD HAVE AN ACTIvETEAM AND OPPOSING TEAM
	//RATHEr THAn POLAYEr AND ENEMY
	//THAT WAY THE ACTIoNs CAN BE 'AGNOSTIC' ANd WOrK PROPERLY As DEfINEd ALREADY
	//CHECKS:
		//NEXt To BEACON? LIGHt IT THEN!!!!!
		//ADVERSARY NEXT To YOU? WHACK EM!
		//ADVERSARY FAR???? WHAT'S THE CLOSEST SQAURE To THEM, HIt 'EM!!!!
		//WITh THE MOST POWERFUl ATtACk BASED ON CLASS AND AVAILABLe ACTIONS
		//IF NO TP LEFT, HOLd STILL - GET EM NEXT TIME


	int currentPosRow = map->GetRowFromPosition(mTeamMembers[mCurrentMember]->mModelInstance->mPosition);
	int currentPosCol = map->GetColFromPosition(mTeamMembers[mCurrentMember]->mModelInstance->mPosition);

	XMFLOAT3 targetPos;

	if (mTeamMembers[mCurrentMember]->mAIType == DEFENDER_AI)
	{
		bool isNextToBeacon = false;
		bool isBeaconLit = false;

		if (currentPosCol - 1 > -1)
			if (map->mTiles[currentPosCol - 1][currentPosRow]->mMapBlockType == DPhoenix::BEACON_MAPBLOCK)
			{
				isNextToBeacon = true;
				targetPos = map->GetPositionAboveFromMapRef(currentPosCol - 1, currentPosRow);

				//check to see if the adjacent beacon is already lit by the enemy
				for (int i = 0; i < beaconVec.size(); i++)
				{
					int beaconRow = map->GetRowFromPosition(beaconVec[i]->mPoleMeshInstance->mPosition);
					int beaconCol = map->GetColFromPosition(beaconVec[i]->mPoleMeshInstance->mPosition);

					if (beaconRow == currentPosRow && beaconCol == currentPosCol - 1)
					{
						if (beaconVec[i]->mBeaconState == BEACON_ENEMY_LIT)
							isBeaconLit = true;
					}
				}

			}

		if (currentPosCol + 1 < map->mWidth)
			if (map->mTiles[currentPosCol + 1][currentPosRow]->mMapBlockType == DPhoenix::BEACON_MAPBLOCK)
			{
				isNextToBeacon = true;
				targetPos = map->GetPositionAboveFromMapRef(currentPosCol + 1, currentPosRow);

				//check to see if the adjacent beacon is already lit by the enemy
				for (int i = 0; i < beaconVec.size(); i++)
				{
					int beaconRow = map->GetRowFromPosition(beaconVec[i]->mPoleMeshInstance->mPosition);
					int beaconCol = map->GetColFromPosition(beaconVec[i]->mPoleMeshInstance->mPosition);

					if (beaconRow == currentPosRow && beaconCol == currentPosCol + 1)
					{
						if (beaconVec[i]->mBeaconState == BEACON_ENEMY_LIT)
							isBeaconLit = true;
					}
				}

			}

		if (currentPosRow - 1 > -1)
			if (map->mTiles[currentPosCol][currentPosRow - 1]->mMapBlockType == DPhoenix::BEACON_MAPBLOCK)
			{
				isNextToBeacon = true;
				targetPos = map->GetPositionAboveFromMapRef(currentPosCol, currentPosRow - 1);

				//check to see if the adjacent beacon is already lit by the enemy
				for (int i = 0; i < beaconVec.size(); i++)
				{
					int beaconRow = map->GetRowFromPosition(beaconVec[i]->mPoleMeshInstance->mPosition);
					int beaconCol = map->GetColFromPosition(beaconVec[i]->mPoleMeshInstance->mPosition);

					if (beaconRow == currentPosRow - 1 && beaconCol == currentPosCol)
					{
						if (beaconVec[i]->mBeaconState == BEACON_ENEMY_LIT)
							isBeaconLit = true;
					}
				}
			}

		if (currentPosRow + 1 < map->mLength)
			if (map->mTiles[currentPosCol][currentPosRow + 1]->mMapBlockType == DPhoenix::BEACON_MAPBLOCK)
			{
				isNextToBeacon = true;
				targetPos = map->GetPositionAboveFromMapRef(currentPosCol, currentPosRow + 1);

				//check to see if the adjacent beacon is already lit by the enemy
				for (int i = 0; i < beaconVec.size(); i++)
				{
					int beaconRow = map->GetRowFromPosition(beaconVec[i]->mPoleMeshInstance->mPosition);
					int beaconCol = map->GetColFromPosition(beaconVec[i]->mPoleMeshInstance->mPosition);

					if (beaconRow == currentPosRow + 1 && beaconCol == currentPosCol)
					{
						if (beaconVec[i]->mBeaconState == BEACON_ENEMY_LIT)
							isBeaconLit = true;
					}
				}
			}

		if (isNextToBeacon && !isBeaconLit)
		{
			//ok, add target to vector and make the magic happen
			targets.clear();
			targets.push_back(new DPhoenix::PrimitiveInstance(*targetHighlight));
			targets.back()->mPosition = targetPos;
			targets.back()->mOpacity = 0.95f;

			mTeamMembers[mCurrentMember]->mMoveState = CH_DOAC_MOVESTATE;
			mTeamMembers[mCurrentMember]->mSelectedAction = LIGHT_BEACON_ACTION;
		}
		else
		{
			mTPTurnSnapshot = mTPPool;
			EndCurrentTurn();
		}
	}

	//ATTACKInG!!!!!!!!
	if (mTeamMembers[mCurrentMember]->mAIType == ATTACKER_AI)
	{
		//THIS NEXT BIt COPIED AND PASTEd DIRECTLY FROM ABOVE WITh SOMe EDITS -------------------------------------

		//need to get the character's grid position to check proximity to players
		int currentCharRow = currentPosRow;
		int currentCharCol = currentPosCol;

		//this is for storing the id of the intended target
		int targetIndex0distance, targetIndex1distance, targetIndex2distance, targetIndex3distance;
		targetIndex0distance = targetIndex1distance = targetIndex2distance = targetIndex3distance = 0;

		int targetRow;
		int targetCol;


		//then we need to see which player is nearest and assign them as the target
		for (int i = 0; i < opposingteam->mTeamMembers.size(); i++)
		{
			//DEATh CHECKS AGAIN - THE REST SHOULD HOLD WELL ENOUGH

			int currentTargetRow = 0;
			int currentTargetCol = 0;

			if (opposingteam->mTeamMembers[i]->mLifeState == CH_DEAD_LIFESTATE)
			{
				//set absurdly high so it never meets the minimum distance check
				currentTargetRow = 9999;
				currentTargetCol = 9999;
			}
			else
			{
				currentTargetRow = map->GetRowFromPosition(opposingteam->mTeamMembers[i]->mModelInstance->mPosition);
				currentTargetCol = map->GetColFromPosition(opposingteam->mTeamMembers[i]->mModelInstance->mPosition);
			}

			switch (i)
			{
			case 0:
				targetIndex0distance = abs(currentCharRow - currentTargetRow) + abs(currentCharCol - currentTargetCol);
				break;
			case 1:
				targetIndex1distance = abs(currentCharRow - currentTargetRow) + abs(currentCharCol - currentTargetCol);
				break;
			case 2:
				targetIndex2distance = abs(currentCharRow - currentTargetRow) + abs(currentCharCol - currentTargetCol);
				break;
			case 3:
				targetIndex3distance = abs(currentCharRow - currentTargetRow) + abs(currentCharCol - currentTargetCol);
				break;
			}
		}

		int minDistance = min(targetIndex0distance, min(targetIndex1distance, min(targetIndex2distance, targetIndex3distance)));

		if (minDistance == targetIndex0distance)
		{
			targetRow = map->GetRowFromPosition(opposingteam->mTeamMembers[0]->mModelInstance->mPosition);
			targetCol = map->GetColFromPosition(opposingteam->mTeamMembers[0]->mModelInstance->mPosition);
			targetPos = opposingteam->mTeamMembers[0]->mModelInstance->mPosition;
		}
		else if (minDistance == targetIndex1distance)
		{
			targetRow = map->GetRowFromPosition(opposingteam->mTeamMembers[1]->mModelInstance->mPosition);
			targetCol = map->GetColFromPosition(opposingteam->mTeamMembers[1]->mModelInstance->mPosition);
			targetPos = opposingteam->mTeamMembers[1]->mModelInstance->mPosition;
		}
		else if (minDistance == targetIndex2distance)
		{
			targetRow = map->GetRowFromPosition(opposingteam->mTeamMembers[2]->mModelInstance->mPosition);
			targetCol = map->GetColFromPosition(opposingteam->mTeamMembers[2]->mModelInstance->mPosition);
			targetPos = opposingteam->mTeamMembers[2]->mModelInstance->mPosition;
		}
		else
		{
			targetRow = map->GetRowFromPosition(opposingteam->mTeamMembers[3]->mModelInstance->mPosition);
			targetCol = map->GetColFromPosition(opposingteam->mTeamMembers[3]->mModelInstance->mPosition);
			targetPos = opposingteam->mTeamMembers[3]->mModelInstance->mPosition;
		}

		//excellent, we have our target

		//FIRST OFF, IF WE ARE NExt TO IT, THEn WE MELEE ATTACK!!!!!
		if ((targetRow == currentPosRow && abs(targetCol - currentPosCol) == 1) ||
			(abs(targetRow - currentPosRow) == 1 && targetCol == currentPosCol))
		{
			//ok, add target to vector and make the magic happen
			targets.clear();
			targets.push_back(new DPhoenix::PrimitiveInstance(*targetHighlight));
			targets.back()->mPosition = targetPos;
			targets.back()->mOpacity = 0.95f;

			mTeamMembers[mCurrentMember]->mMoveState = CH_DOAC_MOVESTATE;
			mTeamMembers[mCurrentMember]->mSelectedAction = HIT_ACTION;
		}
		else
		{
			//right, lets see what actions we have available
			CheckAvailableActions(map->mTiles, currentCharCol, currentCharRow);

			//vector copy for WITHOUT hold still (and without light beacon is SOMEHOw we're next to one)
			std::vector<AvailableActions> actionsNotHoldStill = mAvailableActions;

			for (int i = 0; i < actionsNotHoldStill.size(); )
			{
				if (actionsNotHoldStill[i] == HOLD_STILL_ACTION ||
					actionsNotHoldStill[i] == LIGHT_BEACON_ACTION)
				{
					actionsNotHoldStill.erase(actionsNotHoldStill.begin() + i);
				}
				else
				{
					i++;
				}
			}

			//if no actions left, well it was hold still so do that
			if (actionsNotHoldStill.size() == 0)
			{
				//hold still
				mTPTurnSnapshot = mTPPool;
				EndCurrentTurn();
			}
			else
			{
				//yay we have another action we can use to attack!!!
				//in which case randomly select it

				int action = rand() % actionsNotHoldStill.size();

				mTeamMembers[mCurrentMember]->mSelectedAction = actionsNotHoldStill[action];

				//now, all weapon / magic has 5 squares around the character to target with
				//we need first of all to build that 
				//after which we determine the first adversary we come across and target them
				//if none in the targeting area, we attempt to target the edge
				//nearest the nearest foe

				int minRow = max(currentCharRow - 5, 0);
				int maxRow = min(currentCharRow + 5, map->mWidth - 1);
				int minCol = max(currentCharCol - 5, 0);
				int maxCol = min(currentCharCol + 5, map->mLength - 1);

				XMFLOAT3 targetPos;

				bool isTargetFound = false;

				for (int col = minCol; col <= maxCol && !isTargetFound; col++)
				{
					for (int row = minRow; row <= maxRow && !isTargetFound; row++)
					{
						if (!(currentCharCol == col && currentCharRow == row))
						{

							if (map->mTiles[col][row]->mSpecialFlag == DPhoenix::PLAYER_SP_FLAG)
							{
								targetPos = map->GetPositionAboveFromMapRef(col, row);
								targets.push_back(new DPhoenix::PrimitiveInstance(*targetHighlight));
								targets.back()->mPosition = targetPos;
								targets.back()->mPrevPosition = targets.back()->mPosition;
								isTargetFound = true;
							}
						}
					}
				}

				//ok, nothing in range of target selection so let's try and fire at the nearest edge
				//to a player
				if (!isTargetFound)
				{
					//we already have the row / col of the nearest target so let's get a final 
					//target position based upon the outer edge of our targeting area
					int finalRow;
					int finalCol;
					
					
					if (targetRow <= minRow)
					{
						finalRow = minRow;
					}
					else
					{
						finalRow = maxRow;
					}

					if (targetCol <= minCol)
					{
						finalCol = minCol;
					}
					else
					{
						finalCol = maxCol;
					}

					targetPos = map->GetPositionAboveFromMapRef(finalCol, finalRow);
					targets.push_back(new DPhoenix::PrimitiveInstance(*targetHighlight));
					targets.back()->mPosition = targetPos;
					targets.back()->mPrevPosition = targets.back()->mPosition;

				}

				mTeamMembers[mCurrentMember]->mMoveState = CH_DOAC_MOVESTATE;
			}			
		}
	}
}
