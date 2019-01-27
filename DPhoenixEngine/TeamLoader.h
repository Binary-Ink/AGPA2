#pragma once
#include "D3DUtil.h"

//here use same namespace as game for code transfer
namespace DPhoenix
{
	//struct for individual characters
	struct CharSave
	{
		std::string name; 
		std::string characterclass; 
		int level; 
		int xp; 
	};

	//struct for individual teams
	struct TeamSave
	{
		std::string teamName; 
		std::vector<CharSave*> characters; 
	};

	class TeamLoader
	{
	private:
		tinyxml2::XMLDocument* mXMLTree; //main xml doc
		int mNumTeams; //num of teams
		int mSelectedteam; //current selected team
		std::vector<TeamSave*>mTeams; //all team stats loaded
		std::string mXMLFilename; 

	public:
		TeamLoader() {}; //constructor
		~TeamLoader() {}; //destructor 

		void Init(std::string xmlFileName);
		void ReloadXML();

		TeamSave* GetTeamAt(int index) { return mTeams[index]; };
		std::vector<TeamSave*>&GetTeams() { return mTeams; };
	
#pragma region CRUDOperations
		void Create(TeamSave* teamSave);
		void Update(TeamSave* teamsave, int index);
		void Delete(int index); 

		void killChildren(); 

#pragma endregion CRUDOperations
	};
}
