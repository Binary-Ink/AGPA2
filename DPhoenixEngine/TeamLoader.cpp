#include "TeamLoader.h"

void DPhoenix::TeamLoader::Init(std::string xmlFileName)
{

	mXMLFilename = xmlFileName;

	mXMLTree = NULL;
	mXMLTree = new tinyxml2::XMLDocument();

	//could add error checking here

	mXMLTree->LoadFile(xmlFileName.c_str());

	//empty vector before repopulating
	mTeams.clear();

	//add error checking here
	tinyxml2::XMLElement* rootElement = mXMLTree->FirstChildElement("memorycard");

	for (
		//initialises to the first save element (for each team)
		tinyxml2::XMLElement* parent = rootElement->FirstChildElement("save");
		//continue to loop as long as pos then go to next sibling
		parent != NULL;
		//update pointer to next sibling each iteration
		parent = parent->NextSiblingElement()
		)
	{

		//populate struct with details from file
		TeamSave* tempTeam = new TeamSave();
		tempTeam->teamName = parent->FirstChildElement("teamname")->GetText();

		//we need to have a nested loop to go through the associated characters in the team
		for (
			//initialise to the first save element (for each team)
			tinyxml2::XMLElement* characterparent = parent->FirstChildElement("character");
			//cont to loop  as long as pos, go to the next sibling
			characterparent != NULL;
			//update pointer to next sibling each iteration
			characterparent = characterparent->NextSiblingElement()

			)
		{
			//populate structs for each charadcter then add to the team
			CharSave* tempCharacter = new CharSave();

			tempCharacter->name = characterparent->FirstChildElement("name")->GetText();

			tempCharacter->characterclass = characterparent->FirstChildElement("class")->GetText();

			//these include stoi which converts string to int
			tempCharacter->level = std::stoi(
				characterparent->FirstChildElement("level")->GetText()
			);
			tempCharacter->xp = std::stoi(
				characterparent->FirstChildElement("xp")->GetText()
			);

			//add character to team
			tempTeam->characters.push_back(tempCharacter);
		}

		//update total and update vectors of characters
		mNumTeams++;
		mTeams.push_back(tempTeam);
	}
}

void DPhoenix::TeamLoader::Create(TeamSave* teamSave)
{
	//set up nodes to insert
	tinyxml2::XMLNode* saveNode = mXMLTree->NewElement("save"); 
	mXMLTree->FirstChildElement("memorycard")->InsertEndChild(saveNode); 

	//get pointer to last save node (which inserted above)
	tinyxml2::XMLNode* lastSaveNode =
		mXMLTree->FirstChildElement("memorycard")->LastChildElement("save");

	//create name node and get C style string (const char*) - this is to be inserted at the end
	tinyxml2::XMLNode* nameNode = mXMLTree->NewElement("teamname"); 
	const char* nameText = teamSave->teamName.c_str(); 

	//insert the node and update
	lastSaveNode->InsertEndChild(nameNode); 
	lastSaveNode->FirstChildElement("teamname")->SetText(nameText); 

	for (int i = 0; i < teamSave->characters.size(); i++)
	{
		//add new character element
		tinyxml2::XMLNode* characterNode = mXMLTree->NewElement("character"); 
		lastSaveNode->InsertEndChild(characterNode); 

		//get pointer to last char node (inserted above)
		tinyxml2::XMLNode* lastCharNode = 
		lastSaveNode->LastChildElement("character"); 

		//name
		tinyxml2::XMLNode* nameNode = mXMLTree->NewElement("name");
		const char* nameText = teamSave->characters[i]->name.c_str();
		lastCharNode->InsertEndChild(nameNode); 
		lastCharNode->FirstChildElement("name")->SetText(nameText); 

		//class
		tinyxml2::XMLNode* classNode = mXMLTree->NewElement("class");
		const char* classText = teamSave->characters[i]->characterclass.c_str();
		lastCharNode->InsertEndChild(classNode);
		lastCharNode->FirstChildElement("class")->SetText(classText);

		//level
		tinyxml2::XMLNode* levelNode = mXMLTree->NewElement("level");
		std::string levelText = std::to_string(teamSave->characters[i]->level); 
		lastCharNode->InsertEndChild(levelNode);
		lastCharNode->FirstChildElement("level")->SetText(levelText.c_str());

		//xp
		tinyxml2::XMLNode* xpNode = mXMLTree->NewElement("xp");
		std::string xpText = std::to_string(teamSave->characters[i]->xp);
		lastCharNode->InsertEndChild(xpNode);
		lastCharNode->FirstChildElement("xp")->SetText(xpText.c_str());
	}

	//finally save the xl file (and then reload it)
	mXMLTree->SaveFile(mXMLFilename.c_str()); 
	Init(mXMLFilename);

}

void DPhoenix::TeamLoader::Delete(int index)
{
	tinyxml2::XMLElement* rootElement = mXMLTree->FirstChildElement("memorycard"); 

	//this is to initialise and will be overwritten
	tinyxml2::XMLElement* elementToDelete = rootElement->FirstChildElement("save"); 

	int i = 0; 

	for (
		//initialise to the first save elment for each team
		tinyxml2::XMLElement* parent = rootElement->FirstChildElement("save");
		//continue loop as long as can go to the next sibling
		parent != NULL;
		//update pointer to next sibling each iteration
		parent = parent->NextSiblingElement()
		)
	{
		//if element matches index given, this is to be deleted
		if (i = index)
			elementToDelete = parent; 

		i++;
	}

	//delete element
	rootElement->DeleteChild(elementToDelete); 

	//finally save XML file and reload
	mXMLTree->SaveFile(mXMLFilename.c_str());
	Init(mXMLFilename); 
}