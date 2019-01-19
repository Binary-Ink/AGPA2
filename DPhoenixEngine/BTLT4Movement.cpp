//#include "Engine.h"
//#include "BillboardSprite.h"
//#include "ParticleEmitter.h"
//
////BTLT classes
//#include "Team.h"
//
////govern control flow
//enum GameStates
//{
//	GAME_TITLE_STATE,
//	GAME_MENU_STATE,
//	GAME_PLAY_STATE,
//	GAME_OVER_STATE
//};
//
//enum MenuStates
//{
//	CHOOSE_MENUSTATE,
//	CONFIRM_MENUSTATE,
//	ENEMY_1_PICKED_MENUSTATE,
//	ENEMY_2_PICKED_MENUSTATE,
//	ENEMY_3_PICKED_MENUSTATE,
//	ENEMY_4_PICKED_MENUSTATE
//};
//
//
////initial class definition inheriting fron Engine
//class BTLTMovementDemo : public DPhoenix::Engine
//{
//private:
//	//depth stencil (to handle multiple overlayed sprites)
//	ID3D11DepthStencilState* mDepthStencilState;
//	ID3D11DepthStencilState* mDepthDisabledStencilState;
//	D3D11_DEPTH_STENCIL_DESC mDepthStencilDesc;
//	D3D11_DEPTH_STENCIL_DESC mDepthDisabledStencilDesc;
//
//	//texture manager
//	DPhoenix::TextureMgr mTexMgr;
//
//	//audio manager
//	DPhoenix::AudioMgr mAudioMgr;
//
//	//font pointer - debug text
//	DPhoenix::Font* mDebugFont;
//
//	//screen scale (for window stretching)
//	XMFLOAT2 mScreenScale;
//
//	//view and projection matrices
//	XMMATRIX mView;
//	XMMATRIX mProjection;
//
//	//for geometry generation
//	DPhoenix::GeometryGenerator* mGeoGen;
//
//	DPhoenix::GeometryGenerator::MeshData* mBox;
//	DPhoenix::GeometryGenerator::MeshData* mSphere;
//	DPhoenix::GeometryGenerator::MeshData* mGrid;
//
//	//camera object
//	DPhoenix::Camera* mCamera;
//	DPhoenix::EAngle mCamEAngle;
//
//	//ambient color
//	XMFLOAT4 mAmbientColor;
//	float mAmbIntensity;
//
//	//skybox
//	DPhoenix::Sky* mSkybox;
//
//
//	//lighting ------------------------------------------------
//	//directional lights
//	std::vector<DirectionalLight*> mDirLights;
//	DirectionalLight mDirLightsArray[1];
//	int mNumDirLights;
//
//	//fog values
//	bool mFogEnabled;
//	float  mFogStart;
//	float  mFogRange;
//	XMFLOAT4 mFogColor;
//
//	//point lights
//	std::vector<PointLight*> mPointLights;
//	int mNumPointLights;
//	PointLight mPointLightsArray[10];
//	float mRange;
//	float mPointRotationValue;
//	float mPointY;
//
//#pragma region BTLTMembers
//
//	GameStates mGameState;
//	MenuStates mMenuState;
//
//	DPhoenix::Team* mPlayerTeam;
//	DPhoenix::Team* mEnemyTeam;
//
//	DPhoenix::CharacterClass* mToySoldierMenu;
//	DPhoenix::CharacterClass* mDarkAngelMenu;
//	DPhoenix::CharacterClass* mDragonMenu;
//	DPhoenix::CharacterClass* mBigCatMenu;
//
//	//UI Elements -------------------------
//	int mMouseX;
//	int mMouseY;
//
//	DPhoenix::Sprite* mTitleScreen;
//	DPhoenix::Sprite* mTeamMenuScreen;
//
//	DPhoenix::Sprite* mSelectedClassMenu;
//
//	int mSelectedLevel;
//
//	//fonts -------------------------------
//	DPhoenix::Font* mUIFont16pt;
//	DPhoenix::Font* mUIFont18pt;
//	DPhoenix::Font* mUIFont36pt;
//	DPhoenix::Font* mUIFont30pt;
//	DPhoenix::Font* mUIFont48pt;
//	DPhoenix::Font* mUIFont90pt;
//
//	//action timers
//	DPhoenix::GameTimer mMouseClickTimer;
//	DPhoenix::GameTimer mKeyPressTmer;
//	DPhoenix::GameTimer mWaitTimer;
//
//#pragma region BTLTMapMembers
//
//	DPhoenix::BTLTMap* mMap;
//	std::vector<XMFLOAT3> mPlayerSpawnVec;
//	std::vector<XMFLOAT3> mEnemySpawnVec;
//	std::vector<XMFLOAT3> mBeaconSpawnVec;
//
//	std::vector<std::string> mFloorColorMaps;
//	std::vector<std::string> mFloorNormalMaps;
//	std::vector<std::string> mWallColorMaps;
//	std::vector<std::string> mWallNormalMaps;
//	std::vector<std::string> mCoverColorMaps;
//	std::vector<std::string> mCoverNormalMaps;
//
//	float mCameraDistanceOffset;
//	float mCamToFocusAngle;
//
//#pragma endregion BTLTMapMembers
//
//#pragma region BTLTUIMembers
//
//	DPhoenix::Sprite* mActionsUISprite;
//	DPhoenix::Sprite* mBeaconsUISprite;
//	DPhoenix::Sprite* mEnemyUISprite;
//	DPhoenix::Sprite* mHoveredTurnUISprite;
//	DPhoenix::Sprite* mPlayerTeamUISprite;
//	DPhoenix::Sprite* mSavesUISprite;
//	DPhoenix::Sprite* mSelectedActionUISprite;
//	DPhoenix::Sprite* mSelectedTurnUISprite;
//	DPhoenix::Sprite* mStatusUISprite;
//	DPhoenix::Sprite* mTakenTurnUISprite;
//
//	//avatars
//	DPhoenix::Sprite* mToySoldierAvatar;
//	DPhoenix::Sprite* mDarkAngelAvatar;
//	DPhoenix::Sprite* mDragonAvatar;
//	DPhoenix::Sprite* mBigCatAvatar;
//
//	//teams
//	DPhoenix::TeamTypes mCurrentTeam;
//
//	//camera testing -------
//	bool mCameraBehindFlag;
//	bool mClickFlag;
//#pragma region BTLTMovement
//
//	//need to update shaders to include opacity value
//	//this would work for camera collisions----------
//	//as well as for the ovement overlay-------------
//	//though the movement overlay should also change color-------
//	//SOOOOOOO.... texture switch???????
//	//needs 'happy path' vector to then function as milestones for
//	//character movement
//
//	DPhoenix::GeometryGenerator::MeshData* mSelectionBox;
//	std::vector<DPhoenix::PrimitiveInstance*> mHappyPath;
//	std::vector<DPhoenix::PrimitiveInstance*> mSelectionBoxes;
//	DPhoenix::Line* mCameraLine;
//
//	//templates to copy over
//	DPhoenix::PrimitiveInstance* mSelectionHighlight;
//	DPhoenix::PrimitiveInstance* mBadSelectionHighlight;
//	DPhoenix::PrimitiveInstance* mTargetHighlight;
//
//#pragma endregion BTLTMovement
//	   
//#pragma endregion BTLTUIMembers
//
//#pragma endregion BTLTMembers
//
//public:
//	BTLTMovementDemo(HINSTANCE hInstance);
//	~BTLTMovementDemo();
//
//	bool Init(bool fullScreen);
//	void OnResize();
//	void UpdateScene(float dt);
//	void DrawScene();
//
//	//this builds depth stencil and blend states
//	void BuildGeometryBuffers();
//
//	//new method / implements event handling
//	void BTLTMovementDemo::HandleEvents(DPhoenix::IEvent* e);
//
//	//new method / initialises sounds to be used in the app
//	void InitAudio();
//
//	//new method / specifically handles individual sprite rendering
//	void RenderSprite(DPhoenix::Sprite* sprite,
//		ID3DX11EffectTechnique* tech, bool isHUD = false);
//
//	//new method / specifically handles individual color primitives rendering
//	void RenderColorPrimitive(DPhoenix::PrimitiveInstance* model,
//		ID3DX11EffectTechnique* tech);
//
//	//new method / specifically handles individual textured primitives rendering
//	void BTLTMovementDemo::RenderBasicTexPrimitive(DPhoenix::PrimitiveInstance * model,
//		ID3DX11EffectTechnique * tech);
//
//	//new method / specifically handles textured primitives with directional lights
//	//and fog
//	void BTLTMovementDemo::RenderLitTexPrimitive(DPhoenix::PrimitiveInstance * model,
//		ID3DX11EffectTechnique * tech);
//
//	//new method / now with normal mapping!
//	void BTLTMovementDemo::RenderLitTexNormPrimitive(DPhoenix::PrimitiveInstance * model,
//		ID3DX11EffectTechnique * tech);
//
//	//new method / now with normal mapping!
//	void RenderSkybox(DPhoenix::Sky* skybox,
//		ID3D11DeviceContext* dc);
//
//	//new method / render billboard sprtes
//	void BTLTMovementDemo::RenderBillboardSprite(DPhoenix::BillboardSprite * model,
//		ID3DX11EffectTechnique * tech);
//
//	//new method / render static mesh with subsets
//	void BTLTMovementDemo::RenderBasicModel(DPhoenix::BasicModelInstance * model,
//		ID3DX11EffectTechnique * tech);
//
//	//new method / render static mesh with subsets
//	void BTLTMovementDemo::RenderBasicModelNormal(DPhoenix::BasicModelInstance * model,
//		ID3DX11EffectTechnique * tech);
//
//	//new method / render animated lit mesh with subsets
//	void BTLTMovementDemo::RenderSkinnedModel(DPhoenix::SkinnedModelInstance * model,
//		ID3DX11EffectTechnique * tech);
//
//	//new method / render animated lit normal mapped mesh with subsets
//	void BTLTMovementDemo::RenderSkinnedModelNormal(DPhoenix::SkinnedModelInstance * model,
//		ID3DX11EffectTechnique * tech);
//
//#pragma region BTLTMethods
//
//	bool BTLTMovementDemo::CheckPointInRect(int pointX, int pointY, int left, int top, int right, int bottom);
//	void BTLTMovementDemo::CheckMouseHoverMenu();
//	void BTLTMovementDemo::CheckMouseClickMenu();
//
//	void BTLTMovementDemo::CheckMouseClickRoute();
//	void BTLTMovementDemo::CheckSelectableRoute();
//	void BTLTMovementDemo::ResetPath();
//
//#pragma endregion BTLTMethods
//
//
//};
//
////our epplication entry point - WinMain
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
//	PSTR cmdLine, int showCmd)
//{
//	// Enable run-time memory check for debug builds.
//#if defined(DEBUG) | defined(_DEBUG)
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif
//
//	BTLTMovementDemo theApp(hInstance);
//
//	if (!theApp.Init(false))
//		return 0;
//
//	return theApp.Run();
//}
//
////constructor - don't really need to add much here
//BTLTMovementDemo::BTLTMovementDemo(HINSTANCE hInstance)
//	: Engine(hInstance)
//{
//	mMainWndCaption = "Big Trouble in Little Toyland";
//	//create camera here to prevent
//	//issues with matrix generation
//	mCamera = new DPhoenix::Camera();
//}
//
////destructor (release any memory as necessary)
//BTLTMovementDemo::~BTLTMovementDemo()
//{
//	DPhoenix::Effects::DestroyAll();
//	DPhoenix::InputLayouts::DestroyAll();
//}
//
////any additional initialisation (we generally initiaise game assets here)
//bool BTLTMovementDemo::Init(bool fullScreen)
//{
//	if (!Engine::Init(fullScreen))
//		return false;
//
//	//initialise the texture manager
//	mTexMgr.Init(md3dDevice);
//
//	// Must init Effects first since InputLayouts depend on shader signatures.
//	DPhoenix::Effects::InitAll(md3dDevice);
//	DPhoenix::InputLayouts::InitAll(md3dDevice);
//
//	//must init soundlayer to then buffer audio
//	DPhoenix::SoundLayer::Create(mhMainWnd);
//	InitAudio();
//
//	//build render / blend modes etc.
//	BuildGeometryBuffers();
//
//	//create geometry
//	mGeoGen = new DPhoenix::GeometryGenerator();
//
//	mBox = new DPhoenix::GeometryGenerator::MeshData();
//
//	mGeoGen->CreateBox(10.0f, 10.0f, 10.0f, *mBox);
//
//	//different material types for effects
//	Material* materialStandard = new Material();
//	Material* materialShiny = new Material();
//
//	materialStandard->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	materialStandard->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	materialStandard->Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f);
//	materialStandard->Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
//
//	materialShiny->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	materialShiny->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	materialShiny->Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
//	materialShiny->Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
//
//	//ambient lighting values
//	mAmbientColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	mAmbIntensity = 1.0f;
//
//#pragma region SetupLights
//
//	//direct above - white light (ish)
//	mDirLights.push_back(new DirectionalLight());
//	mDirLights.back()->Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
//	mDirLights.back()->Diffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
//	mDirLights.back()->Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
//	mDirLights.back()->Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
//
//	mNumDirLights = 1;
//
//	//fog values
//	mFogEnabled = true;
//	mFogStart = 100.0f;
//	mFogRange = 200.0f;
//	mFogColor = XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f);
//
//	//for point lights
//	mRange = 50.0f;
//	mPointRotationValue = 0.0f;
//	mNumPointLights = 0;
//	mPointY = 25.0f;
//
//
//#pragma endregion SetupLights
//
//	//camera and EAngle initialisation (cam in constructor due to use in OnResize)
//	mCamEAngle.r = 0.0f; mCamEAngle.p = 0.0f; mCamEAngle.y = 0.0f;
//
//	//debug font
//	//(Arial size 20 sending window, initial brush, writefactory)
//	mDebugFont = new DPhoenix::Font(mhMainWnd, mBlackBrush, mDWriteFactory, "Arial", 20.0f);
//
//#pragma region BTLTInit
//
//	mGameState = GAME_TITLE_STATE;
//	mMenuState = CHOOSE_MENUSTATE;
//
//	mPlayerTeam = new DPhoenix::Team();
//	mEnemyTeam = new DPhoenix::Team();
//
//	mToySoldierMenu = new DPhoenix::CharacterClass(DPhoenix::TOY_SOLDIER_CLASS,
//		&mTexMgr, md3dDevice, &mAudioMgr, mHappyPath);
//	mToySoldierMenu->mModelInstance->mPosition.x = 50.0f;
//	mToySoldierMenu->mModelInstance->mPosition.z = 37.5f;
//
//	mDarkAngelMenu = new DPhoenix::CharacterClass(DPhoenix::DARK_ANGEL_CLASS,
//		&mTexMgr, md3dDevice, &mAudioMgr, mHappyPath);
//	mDarkAngelMenu->mModelInstance->mPosition.x = 50.0f;
//	mDarkAngelMenu->mModelInstance->mPosition.z = mToySoldierMenu->mModelInstance->mPosition.z - 12.5f;
//
//	mDragonMenu = new DPhoenix::CharacterClass(DPhoenix::DRAGON_CLASS,
//		&mTexMgr, md3dDevice, &mAudioMgr, mHappyPath);
//	mDragonMenu->mModelInstance->mPosition.x = 50.0f;
//	mDragonMenu->mModelInstance->mPosition.z = mDarkAngelMenu->mModelInstance->mPosition.z - 12.5f;
//
//	mBigCatMenu = new DPhoenix::CharacterClass(DPhoenix::BIG_CAT_CLASS,
//		&mTexMgr, md3dDevice, &mAudioMgr, mHappyPath);
//	mBigCatMenu->mModelInstance->mPosition.x = 50.0f;
//	mBigCatMenu->mModelInstance->mPosition.z = mDragonMenu->mModelInstance->mPosition.z - 12.5f;
//
//	mSelectedLevel = 1;
//
//	//UI Elements -------------------------
//	mMouseX = 0;
//	mMouseY = 0;
//
//	mTitleScreen = new DPhoenix::Sprite();
//	mTitleScreen->Load("Textures\\Screens\\Screen_Title.png", &mTexMgr, 1600.0f, 900.0f, md3dDevice);
//
//	mTeamMenuScreen = new DPhoenix::Sprite();
//	mTeamMenuScreen->Load("Textures\\Screens\\Screen_Team.png", &mTexMgr, 1600.0f, 900.0f, md3dDevice);
//
//	mSelectedClassMenu = new DPhoenix::Sprite();
//	mSelectedClassMenu->Load("Textures\\UI\\SelectedClass.png", &mTexMgr, 300.0f, 350.0f, md3dDevice);
//	mSelectedClassMenu->mOpacityValue = 0.0f;
//
//	//fonts -------------------------------
//	mUIFont16pt = new DPhoenix::Font(mhMainWnd, mBlackBrush, mDWriteFactory, "Showcard Gothic", 16.0f);
//	mUIFont18pt = new DPhoenix::Font(mhMainWnd, mBlackBrush, mDWriteFactory, "Showcard Gothic", 18.0f);
//	mUIFont30pt = new DPhoenix::Font(mhMainWnd, mBlackBrush, mDWriteFactory, "Showcard Gothic", 30.0f);
//	mUIFont36pt = new DPhoenix::Font(mhMainWnd, mBlackBrush, mDWriteFactory, "Showcard Gothic", 36.0f);
//	mUIFont48pt = new DPhoenix::Font(mhMainWnd, mBlackBrush, mDWriteFactory, "Showcard Gothic", 48.0f);
//	mUIFont90pt = new DPhoenix::Font(mhMainWnd, mBlackBrush, mDWriteFactory, "Showcard Gothic", 90.0f);
//
//
//#pragma region BTLTMapInit
//
//	mSkybox = new DPhoenix::Sky(md3dDevice, "textures\\Skybox\\bedroomSkybox.dds", 5000.0f);
//
//	mFloorColorMaps.insert(mFloorColorMaps.end(),
//		{ "Textures\\Floor\\Floor_B_cm.png", "Textures\\Floor\\Floor_C_cm.png",  "Textures\\Floor\\Floor_D_cm.png",
//		"Textures\\Floor\\Floor_F_cm.png",  "Textures\\Floor\\Floor_G_cm.png",  "Textures\\Floor\\Floor_H_cm.png",
//		"Textures\\Floor\\Floor_J_cm.png", "Textures\\Floor\\Floor_K_cm.png", "Textures\\Floor\\Floor_L_cm.png",
//		"Textures\\Floor\\Floor_M_cm.png", "Textures\\Floor\\Floor_N_cm.png", "Textures\\Floor\\Floor_P_cm.png",
//		"Textures\\Floor\\Floor_Q_cm.png", "Textures\\Floor\\Floor_R_cm.png", "Textures\\Floor\\Floor_S_cm.png",
//		"Textures\\Floor\\Floor_T_cm.png", "Textures\\Floor\\Floor_V_cm.png", "Textures\\Floor\\Floor_W_cm.png",
//		"Textures\\Floor\\Floor_X_cm.png", "Textures\\Floor\\Floor_Y_cm.png", "Textures\\Floor\\Floor_Z_cm.png" });
//	mFloorNormalMaps.insert(mFloorNormalMaps.end(),
//		{ "Textures\\Floor\\Floor_B_nm.png", "Textures\\Floor\\Floor_C_nm.png",  "Textures\\Floor\\Floor_D_nm.png",
//		"Textures\\Floor\\Floor_F_nm.png",  "Textures\\Floor\\Floor_G_nm.png",  "Textures\\Floor\\Floor_H_nm.png",
//		"Textures\\Floor\\Floor_J_nm.png", "Textures\\Floor\\Floor_K_nm.png", "Textures\\Floor\\Floor_L_nm.png",
//		"Textures\\Floor\\Floor_M_nm.png", "Textures\\Floor\\Floor_N_nm.png", "Textures\\Floor\\Floor_P_nm.png",
//		"Textures\\Floor\\Floor_Q_nm.png", "Textures\\Floor\\Floor_R_nm.png", "Textures\\Floor\\Floor_S_nm.png",
//		"Textures\\Floor\\Floor_T_nm.png", "Textures\\Floor\\Floor_V_nm.png", "Textures\\Floor\\Floor_W_nm.png",
//		"Textures\\Floor\\Floor_X_nm.png", "Textures\\Floor\\Floor_Y_nm.png", "Textures\\Floor\\Floor_Z_nm.png" });
//	mWallColorMaps.insert(mWallColorMaps.end(),
//		{ "Textures\\Wall\\Wall_A_cm.png", "Textures\\Wall\\Wall_E_cm.png", "Textures\\Wall\\Wall_I_cm.png",
//			"Textures\\Wall\\Wall_O_cm.png", "Textures\\Wall\\Wall_U_cm.png" });
//	mWallNormalMaps.insert(mWallNormalMaps.end(),
//		{ "Textures\\Wall\\Wall_A_nm.png", "Textures\\Wall\\Wall_E_nm.png", "Textures\\Wall\\Wall_I_nm.png",
//				"Textures\\Wall\\Wall_O_nm.png", "Textures\\Wall\\Wall_U_nm.png" });
//	mCoverColorMaps.insert(mCoverColorMaps.end(),
//		{ "Textures\\Cover\\Cover_DeanyP_cm.png", "Textures\\Cover\\Cover_Humpty_cm.png", "Textures\\Cover\\Cover_Ninja_cm.png",
//			"Textures\\Cover\\Cover_SS_cm.png" });
//	mCoverNormalMaps.insert(mCoverNormalMaps.end(),
//		{ "Textures\\Cover\\Cover_DeanyP_nm.png", "Textures\\Cover\\Cover_Humpty_nm.png", "Textures\\Cover\\Cover_Ninja_nm.png",
//			"Textures\\Cover\\Cover_SS_nm.png" });
//
//	/*mMap = new DPhoenix::BTLTMap("Data\\Levels\\Level1.csv", &mTexMgr, md3dDevice,
//								mEnemySpawnVec, mPlayerSpawnVec, mBeaconSpawnVec,
//								&mBox, mFloorColorMaps, mFloorNormalMaps, mWallColorMaps, mWallNormalMaps,
//								mCoverColorMaps, mCoverNormalMaps, 50, 50, 10.0f);*/
//
//	mGeoGen->CreateBox(20.0f, 20.0f, 20.0f, *mBox);
//
//	mMap = new DPhoenix::BTLTMap("Data\\Levels\\Level1.csv", &mTexMgr, md3dDevice,
//		mEnemySpawnVec, mPlayerSpawnVec, mBeaconSpawnVec,
//		mBox, mFloorColorMaps, mFloorNormalMaps, mWallColorMaps, mWallNormalMaps,
//		mCoverColorMaps, mCoverNormalMaps, 30, 30, 20.0f);
//
//
//	mCameraDistanceOffset = 50.0f;
//	//x / z positioning on unit circle
//	mCamToFocusAngle = 45.0f;
//
//	mCameraBehindFlag = false;
//
//#pragma endregion BTLTMapInit
//
//#pragma region BTLTUIInit
//
//	//positioning calcs are hard - needed to take into account origin of 0,0 of screen and sprite
//	//yet all the Photoshopping catalogiuing was top-left
//	//either way, this needed to be done... hence the adding / subtracting necessary for x / y pos
//
//	mActionsUISprite = new DPhoenix::Sprite();
//	mActionsUISprite->Load("Textures\\UI\\ActionsUI.png", &mTexMgr, 250.0f, 250.0f, md3dDevice);
//	mActionsUISprite->mPosition = XMFLOAT3(-750.0f + 125.0f, 350.0f - 125.0f, 0.0f);
//
//	mBeaconsUISprite = new DPhoenix::Sprite();
//	mBeaconsUISprite->Load("Textures\\UI\\BeaconsUI.png", &mTexMgr, 250.0f, 250.0f, md3dDevice);
//	mBeaconsUISprite->mPosition = XMFLOAT3(500.0f + 125.0f, -100.0f - 125.0f, 0.0f);
//
//	mEnemyUISprite = new DPhoenix::Sprite();
//	mEnemyUISprite->Load("Textures\\UI\\EnemyUI.png", &mTexMgr, 250.0f, 250.0f, md3dDevice);
//	mEnemyUISprite->mPosition = XMFLOAT3(500.0f + 125.0f, 200.0f - 125.0f, 0.0f);
//
//	mStatusUISprite = new DPhoenix::Sprite();
//	mStatusUISprite->Load("Textures\\UI\\StatusUI.png", &mTexMgr, 1000.0f, 50.0f, md3dDevice);
//	mStatusUISprite->mPosition = XMFLOAT3(-500.0f + 500.0f, 400.0f - 25.0f, 0.0f);
//
//	mPlayerTeamUISprite = new DPhoenix::Sprite();
//	mPlayerTeamUISprite->Load("Textures\\UI\\PlayerTeam.png", &mTexMgr, 250.0f, 300.0f, md3dDevice);
//	mPlayerTeamUISprite->mPosition = XMFLOAT3(-750.0f + 125.0f, -50.0f - 150.0f, 0.0f);
//
//	mSavesUISprite = new DPhoenix::Sprite();
//	mSavesUISprite->Load("Textures\\UI\\SavesUI.png", &mTexMgr, 320.0f, 50.0f, md3dDevice);
//	mSavesUISprite->mPosition = XMFLOAT3(180.0f + 160.0f, -350.0f - 25.0f, 0.0f);
//
//
//	//position to be set dynamically
//	mHoveredTurnUISprite = new DPhoenix::Sprite();
//	mHoveredTurnUISprite->Load("Textures\\UI\\HoveredTurn.png", &mTexMgr, 250.0f, 50.0f, md3dDevice);
//
//	mSelectedActionUISprite = new DPhoenix::Sprite();
//	mSelectedActionUISprite->Load("Textures\\UI\\SelectedAction.png", &mTexMgr, 250.0f, 25.0f, md3dDevice);
//
//	mSelectedTurnUISprite = new DPhoenix::Sprite();
//	mSelectedTurnUISprite->Load("Textures\\UI\\SelectedTurn.png", &mTexMgr, 250.0f, 50.0f, md3dDevice);
//
//	mTakenTurnUISprite = new DPhoenix::Sprite();
//	mTakenTurnUISprite->Load("Textures\\UI\\TakenTurn.png", &mTexMgr, 250.0f, 50.0f, md3dDevice);
//
//	//avatars
//	mToySoldierAvatar = new DPhoenix::Sprite();
//	mToySoldierAvatar->Load("Textures\\UI\\SoldierAvatar.png", &mTexMgr, 25.0f, 25.0f, md3dDevice);
//	mDarkAngelAvatar = new DPhoenix::Sprite();
//	mDarkAngelAvatar->Load("Textures\\UI\\AngelAvatar.png", &mTexMgr, 25.0f, 25.0f, md3dDevice);
//	mDragonAvatar = new DPhoenix::Sprite();
//	mDragonAvatar->Load("Textures\\UI\\DragonAvatar.png", &mTexMgr, 25.0f, 25.0f, md3dDevice);
//	mBigCatAvatar = new DPhoenix::Sprite();
//	mBigCatAvatar->Load("Textures\\UI\\CatAvatar.png", &mTexMgr, 25.0f, 25.0f, md3dDevice);
//
//	//set early but not used until in game state
//	mCurrentTeam = DPhoenix::PLAYER_TEAM;
//
//#pragma endregion BTLTUIInit
//
//#pragma region BTLTMovementInit
//
//	mCameraLine = NULL;
//	mSelectionBox = new DPhoenix::GeometryGenerator::MeshData();
//	mGeoGen->CreateGrid(20.0f, 20.0f, 2, 2, *mSelectionBox);
//
//	//set up standard 'templates' to copy as new instances as needed
//	mSelectionHighlight = new DPhoenix::PrimitiveInstance();
//	mSelectionHighlight->LoadLitTexInstance(md3dDevice, "Textures\\Select\\SelectTileGreen_cm.png", mSelectionBox, &mTexMgr);
//	mSelectionHighlight->mMaterial = materialShiny;
//	mSelectionHighlight->mNormalMap = mTexMgr.CreateTexture("Textures\\Select\\SelectTileGreen_nm.png");
//
//	mBadSelectionHighlight = new DPhoenix::PrimitiveInstance();
//	mBadSelectionHighlight->LoadLitTexInstance(md3dDevice, "Textures\\Select\\SelectTileRed_cm.png", mSelectionBox, &mTexMgr);
//	mBadSelectionHighlight->mMaterial = materialShiny;
//	mBadSelectionHighlight->mNormalMap = mTexMgr.CreateTexture("Textures\\Select\\SelectTileRed_nm.png");
//
//	mTargetHighlight = new DPhoenix::PrimitiveInstance();
//	mTargetHighlight->LoadLitTexInstance(md3dDevice, "Textures\\Select\\SelectTileBlue_cm.png", mSelectionBox, &mTexMgr);
//	mTargetHighlight->mMaterial = materialShiny;
//	mTargetHighlight->mNormalMap = mTexMgr.CreateTexture("Textures\\Select\\SelectTileBlue_nm.png");
//	
//	//COLLISIONS!!!!!!
//	mGlobalCollisionOn = true;
//	mClickFlag = false;
//#pragma endregion BTLTMovementInit
//
//#pragma endregion BTLTInit
//
//	return true;
//}
//
//bool BTLTMovementDemo::CheckPointInRect(int pointX, int pointY, int left, int top, int right, int bottom)
//{
//	if (pointX < left)
//		return false;
//	if (pointX > right)
//		return false;
//	if (pointY > top)
//		return false;
//	if (pointY < bottom)
//		return false;
//
//	return true;
//}
//
//void BTLTMovementDemo::CheckMouseHoverMenu()
//{
//	//lots of constants / magic numbers here - this could be fixed to add more meaning
//	//adjustments to be made to positions as DX11 has origin as 0,0 in centre
//
//	int mouseX = mMouseX - 800;
//	int mouseY = 450 - mMouseY;
//
//	//toy soldier area
//	int tsLeft = -800; int tsTop = 150; int tsRight = tsLeft + 300; int tsBottom = tsTop - 350;
//	//dark angel area
//	int daLeft = -500; int daTop = 150; int daRight = daLeft + 300; int daBottom = daTop - 350;
//	//dragon area
//	int dLeft = -200; int dTop = 150; int dRight = dLeft + 300; int dBottom = dTop - 350;
//	//big cat area
//	int bcLeft = 100; int bcTop = 150; int bcRight = bcLeft + 300; int bcBottom = bcTop - 350;
//
//	if (CheckPointInRect(mouseX, mouseY, tsLeft, tsTop, tsRight, tsBottom))
//	{
//		mSelectedClassMenu->mPosition.x = tsLeft + 150;
//		mSelectedClassMenu->mPosition.y = tsTop - 175;
//		mSelectedClassMenu->mOpacityValue = 1.0f;
//	}
//	else if (CheckPointInRect(mouseX, mouseY, daLeft, daTop, daRight, daBottom))
//	{
//		mSelectedClassMenu->mPosition.x = daLeft + 150;
//		mSelectedClassMenu->mPosition.y = daTop - 175;
//		mSelectedClassMenu->mOpacityValue = 1.0f;
//	}
//	else if (CheckPointInRect(mouseX, mouseY, dLeft, dTop, dRight, dBottom))
//	{
//		mSelectedClassMenu->mPosition.x = dLeft + 150;
//		mSelectedClassMenu->mPosition.y = dTop - 175;
//		mSelectedClassMenu->mOpacityValue = 1.0f;
//	}
//	else if (CheckPointInRect(mouseX, mouseY, bcLeft, bcTop, bcRight, bcBottom))
//	{
//		mSelectedClassMenu->mPosition.x = bcLeft + 150;
//		mSelectedClassMenu->mPosition.y = bcTop - 175;
//		mSelectedClassMenu->mOpacityValue = 1.0f;
//	}
//	else
//	{
//		mSelectedClassMenu->mOpacityValue = 0.0f;
//	}
//
//}
//
//void BTLTMovementDemo::CheckMouseClickMenu()
//{
//	//lots of constants / magic numbers here - this could be fixed to add more meaning
//	//adjustments to be made to positions as DX11 has origin as 0,0 in centre
//
//	int mouseX = mMouseX - 800;
//	int mouseY = 450 - mMouseY;
//
//	//toy soldier area
//	int tsLeft = -800; int tsTop = 150; int tsRight = tsLeft + 300; int tsBottom = tsTop - 350;
//	//dark angel area
//	int daLeft = -500; int daTop = 150; int daRight = daLeft + 300; int daBottom = daTop - 350;
//	//dragon area
//	int dLeft = -200; int dTop = 150; int dRight = dLeft + 300; int dBottom = dTop - 350;
//	//big cat area
//	int bcLeft = 100; int bcTop = 150; int bcRight = bcLeft + 300; int bcBottom = bcTop - 350;
//	//up level arrow
//	int upLeft = 700; int upTop = 250; int upRight = upLeft + 50; int upBottom = upTop - 50;
//	//down level arrow
//	int downLeft = 700; int downTop = 200; int downRight = downLeft + 50; int downBottom = downTop - 50;
//	//remove last icon
//	int remLeft = 700; int remTop = -100; int remRight = remLeft + 50; int remBottom = remTop - 50;
//
//	//class selection
//	if (CheckPointInRect(mouseX, mouseY, tsLeft, tsTop, tsRight, tsBottom))
//	{
//		if (mPlayerTeam->mTeamMembers.size() < 4)
//		{
//			mPlayerTeam->mTeamMembers.push_back(new DPhoenix::CharacterClass(*mToySoldierMenu));
//		}
//	}
//	else if (CheckPointInRect(mouseX, mouseY, daLeft, daTop, daRight, daBottom))
//	{
//		if (mPlayerTeam->mTeamMembers.size() < 4)
//		{
//			mPlayerTeam->mTeamMembers.push_back(new DPhoenix::CharacterClass(*mDarkAngelMenu));
//		}
//	}
//	else if (CheckPointInRect(mouseX, mouseY, dLeft, dTop, dRight, dBottom))
//	{
//		if (mPlayerTeam->mTeamMembers.size() < 4)
//		{
//			mPlayerTeam->mTeamMembers.push_back(new DPhoenix::CharacterClass(*mDragonMenu));
//		}
//	}
//	else if (CheckPointInRect(mouseX, mouseY, bcLeft, bcTop, bcRight, bcBottom))
//	{
//		if (mPlayerTeam->mTeamMembers.size() < 4)
//		{
//			mPlayerTeam->mTeamMembers.push_back(new DPhoenix::CharacterClass(*mBigCatMenu));
//		}
//	}
//	//arrows and icons
//	else if (CheckPointInRect(mouseX, mouseY, upLeft, upTop, upRight, upBottom))
//	{
//		if (mSelectedLevel < 20)
//		{
//			mSelectedLevel++;
//			mToySoldierMenu->CalculateStats(mSelectedLevel);
//			mDarkAngelMenu->CalculateStats(mSelectedLevel);
//			mDragonMenu->CalculateStats(mSelectedLevel);
//			mBigCatMenu->CalculateStats(mSelectedLevel);
//
//			for (int i = 0; i < mPlayerTeam->mTeamMembers.size(); i++)
//			{
//				mPlayerTeam->mTeamMembers[i]->CalculateStats(mSelectedLevel);
//			}
//
//		}
//	}
//	else if (CheckPointInRect(mouseX, mouseY, downLeft, downTop, downRight, downBottom))
//	{
//		if (mSelectedLevel > 1)
//		{
//			mSelectedLevel--;
//			mToySoldierMenu->CalculateStats(mSelectedLevel);
//			mDarkAngelMenu->CalculateStats(mSelectedLevel);
//			mDragonMenu->CalculateStats(mSelectedLevel);
//			mBigCatMenu->CalculateStats(mSelectedLevel);
//
//			for (int i = 0; i < mPlayerTeam->mTeamMembers.size(); i++)
//			{
//				mPlayerTeam->mTeamMembers[i]->CalculateStats(mSelectedLevel);
//			}
//
//		}
//	}
//	else if (CheckPointInRect(mouseX, mouseY, remLeft, remTop, remRight, remBottom))
//	{
//		if (mPlayerTeam->mTeamMembers.size() > 0)
//		{
//			mPlayerTeam->mTeamMembers.pop_back();
//		}
//	}
//
//}
//
//void BTLTMovementDemo::CheckMouseClickRoute()
//{
//	
//	for (int i = 0; i < mSelectionBoxes.size(); i++)
//	{
//		if (mSelectionBoxes[i]->mCollided)
//		{
//			int col = mMap->GetColFromPosition(mSelectionBoxes[i]->mPosition);
//			int row = mMap->GetRowFromPosition(mSelectionBoxes[i]->mPosition);
//
//			if (mMap->mTiles[col][row]->mSpecialFlag == DPhoenix::NO_SP_FLAG &&
//				mMap->mTiles[col][row]->mMapBlockType == DPhoenix::FLOOR_MAPBLOCK)
//			{
//
//				mHappyPath.push_back(new DPhoenix::PrimitiveInstance(*mSelectionBoxes[i]));
//				mHappyPath.back()->mOpacity = 0.95f;
//				mPlayerTeam->mTPPool--;
//				mMap->mTiles[col][row]->mSpecialFlag = DPhoenix::HAPPY_PATH_SP_FLAG;
//			}
//		}
//	}
//
//	for (int i = 0; i < mSelectionBoxes.size(); i++)
//	{
//		mSelectionBoxes[i]->mAlive = false;				
//	}
//
//	mSelectionBoxes.clear();
//	CheckSelectableRoute();
//}
//
//void BTLTMovementDemo::CheckSelectableRoute()
//{
//	//check tiles surrounding selection --------------------------
//	//might be a bit big lol - prob a way of doing an elaborate loop on it...
//	int currentTileCol = mMap->GetColFromPosition(mHappyPath.back()->mPosition);
//	int currentTileRow = mMap->GetRowFromPosition(mHappyPath.back()->mPosition);
//	XMFLOAT3 spawnPosition;
//
//	//check enough TP left for 2TP each of remaining members
//	if (mPlayerTeam->mTPPool > mPlayerTeam->mTurnsLeft * 2)
//	{
//		if (currentTileCol - 1 > -1)
//		{
//			if (mMap->mTiles[currentTileCol - 1][currentTileRow]->mMapBlockType == DPhoenix::FLOOR_MAPBLOCK &&
//				mMap->mTiles[currentTileCol - 1][currentTileRow]->mSpecialFlag == DPhoenix::NO_SP_FLAG)
//			{
//				spawnPosition = mMap->GetPositionAboveFromMapRef(currentTileCol - 1, currentTileRow);
//				mSelectionBoxes.push_back(new DPhoenix::PrimitiveInstance(*mSelectionHighlight));
//				mSelectionBoxes.back()->mPosition = spawnPosition;
//				mSelectionBoxes.back()->mPrevPosition = mSelectionBoxes.back()->mPosition;
//				mSelectionBoxes.back()->mOpacity = 0.25f;
//				mSelectionBoxes.back()->mEntityType = DPhoenix::ENTITY_SELECTION;
//				mSelectionBoxes.back()->mHalfSizes.x = 10.0f; mSelectionBoxes.back()->mHalfSizes.z = 10.0f;
//				mSelectionBoxes.back()->mCollidable = true;
//				AddEntity(mSelectionBoxes.back());
//			}
//			if (mMap->mTiles[currentTileCol - 1][currentTileRow]->mMapBlockType == DPhoenix::COVER_MAPBLOCK ||
//				mMap->mTiles[currentTileCol - 1][currentTileRow]->mMapBlockType == DPhoenix::WALL_MAPBLOCK)
//			{
//				spawnPosition = mMap->GetPositionAboveFromMapRef(currentTileCol - 1, currentTileRow);
//				mSelectionBoxes.push_back(new DPhoenix::PrimitiveInstance(*mBadSelectionHighlight));
//				mSelectionBoxes.back()->mPosition = spawnPosition;
//				mSelectionBoxes.back()->mPrevPosition = mSelectionBoxes.back()->mPosition;
//				mSelectionBoxes.back()->mOpacity = 0.25f;
//				mSelectionBoxes.back()->mEntityType = DPhoenix::ENTITY_SELECTION;
//				mSelectionBoxes.back()->mHalfSizes.x = 10.0f; mSelectionBoxes.back()->mHalfSizes.z = 10.0f;
//				mSelectionBoxes.back()->mCollidable = true;
//				AddEntity(mSelectionBoxes.back());
//			}
//		}
//
//		if (currentTileCol + 1 < mMap->mWidth)
//		{
//			if (mMap->mTiles[currentTileCol + 1][currentTileRow]->mMapBlockType == DPhoenix::FLOOR_MAPBLOCK &&
//				mMap->mTiles[currentTileCol + 1][currentTileRow]->mSpecialFlag == DPhoenix::NO_SP_FLAG)
//			{
//				spawnPosition = mMap->GetPositionAboveFromMapRef(currentTileCol + 1, currentTileRow);
//				mSelectionBoxes.push_back(new DPhoenix::PrimitiveInstance(*mSelectionHighlight));
//				mSelectionBoxes.back()->mPosition = spawnPosition;
//				mSelectionBoxes.back()->mPrevPosition = mSelectionBoxes.back()->mPosition;
//				mSelectionBoxes.back()->mOpacity = 0.25f;
//				mSelectionBoxes.back()->mEntityType = DPhoenix::ENTITY_SELECTION;
//				mSelectionBoxes.back()->mHalfSizes.x = 10.0f; mSelectionBoxes.back()->mHalfSizes.z = 10.0f;
//				mSelectionBoxes.back()->mCollidable = true;
//				AddEntity(mSelectionBoxes.back());
//			}
//			if (mMap->mTiles[currentTileCol + 1][currentTileRow]->mMapBlockType == DPhoenix::COVER_MAPBLOCK ||
//				mMap->mTiles[currentTileCol + 1][currentTileRow]->mMapBlockType == DPhoenix::WALL_MAPBLOCK)
//			{
//				spawnPosition = mMap->GetPositionAboveFromMapRef(currentTileCol + 1, currentTileRow);
//				mSelectionBoxes.push_back(new DPhoenix::PrimitiveInstance(*mBadSelectionHighlight));
//				mSelectionBoxes.back()->mPosition = spawnPosition;
//				mSelectionBoxes.back()->mPrevPosition = mSelectionBoxes.back()->mPosition;
//				mSelectionBoxes.back()->mOpacity = 0.25f;
//				mSelectionBoxes.back()->mEntityType = DPhoenix::ENTITY_SELECTION;
//				mSelectionBoxes.back()->mHalfSizes.x = 10.0f; mSelectionBoxes.back()->mHalfSizes.z = 10.0f;
//				mSelectionBoxes.back()->mCollidable = true;
//				AddEntity(mSelectionBoxes.back());
//			}
//		}
//		if (currentTileRow - 1 > -1)
//		{
//			if (mMap->mTiles[currentTileCol][currentTileRow - 1]->mMapBlockType == DPhoenix::FLOOR_MAPBLOCK &&
//				mMap->mTiles[currentTileCol][currentTileRow - 1]->mSpecialFlag == DPhoenix::NO_SP_FLAG)
//			{
//				spawnPosition = mMap->GetPositionAboveFromMapRef(currentTileCol, currentTileRow - 1);
//				mSelectionBoxes.push_back(new DPhoenix::PrimitiveInstance(*mSelectionHighlight));
//				mSelectionBoxes.back()->mPosition = spawnPosition;
//				mSelectionBoxes.back()->mPrevPosition = mSelectionBoxes.back()->mPosition;
//				mSelectionBoxes.back()->mOpacity = 0.25f;
//				mSelectionBoxes.back()->mEntityType = DPhoenix::ENTITY_SELECTION;
//				mSelectionBoxes.back()->mHalfSizes.x = 10.0f; mSelectionBoxes.back()->mHalfSizes.z = 10.0f;
//				mSelectionBoxes.back()->mCollidable = true;
//				AddEntity(mSelectionBoxes.back());
//			}
//			if (mMap->mTiles[currentTileCol][currentTileRow - 1]->mMapBlockType == DPhoenix::COVER_MAPBLOCK ||
//				mMap->mTiles[currentTileCol][currentTileRow - 1]->mMapBlockType == DPhoenix::WALL_MAPBLOCK)
//			{
//				spawnPosition = mMap->GetPositionAboveFromMapRef(currentTileCol, currentTileRow - 1);
//				mSelectionBoxes.push_back(new DPhoenix::PrimitiveInstance(*mBadSelectionHighlight));
//				mSelectionBoxes.back()->mPosition = spawnPosition;
//				mSelectionBoxes.back()->mPrevPosition = mSelectionBoxes.back()->mPosition;
//				mSelectionBoxes.back()->mOpacity = 0.25f;
//				mSelectionBoxes.back()->mEntityType = DPhoenix::ENTITY_SELECTION;
//				mSelectionBoxes.back()->mHalfSizes.x = 10.0f; mSelectionBoxes.back()->mHalfSizes.z = 10.0f;
//				mSelectionBoxes.back()->mCollidable = true;
//				AddEntity(mSelectionBoxes.back());
//			}
//		}
//		if (currentTileRow + 1 < mMap->mLength)
//		{
//			if (mMap->mTiles[currentTileCol][currentTileRow + 1]->mMapBlockType == DPhoenix::FLOOR_MAPBLOCK &&
//				mMap->mTiles[currentTileCol][currentTileRow + 1]->mSpecialFlag == DPhoenix::NO_SP_FLAG)
//			{
//				spawnPosition = mMap->GetPositionAboveFromMapRef(currentTileCol, currentTileRow + 1);
//				mSelectionBoxes.push_back(new DPhoenix::PrimitiveInstance(*mSelectionHighlight));
//				mSelectionBoxes.back()->mPosition = spawnPosition;
//				mSelectionBoxes.back()->mPrevPosition = mSelectionBoxes.back()->mPosition;
//				mSelectionBoxes.back()->mOpacity = 0.25f;
//				mSelectionBoxes.back()->mEntityType = DPhoenix::ENTITY_SELECTION;
//				mSelectionBoxes.back()->mHalfSizes.x = 10.0f; mSelectionBoxes.back()->mHalfSizes.z = 10.0f;
//				mSelectionBoxes.back()->mCollidable = true;
//				AddEntity(mSelectionBoxes.back());
//			}
//			if (mMap->mTiles[currentTileCol][currentTileRow + 1]->mMapBlockType == DPhoenix::COVER_MAPBLOCK ||
//				mMap->mTiles[currentTileCol][currentTileRow + 1]->mMapBlockType == DPhoenix::WALL_MAPBLOCK)
//			{
//				spawnPosition = mMap->GetPositionAboveFromMapRef(currentTileCol, currentTileRow + 1);
//				mSelectionBoxes.push_back(new DPhoenix::PrimitiveInstance(*mBadSelectionHighlight));
//				mSelectionBoxes.back()->mPosition = spawnPosition;
//				mSelectionBoxes.back()->mPrevPosition = mSelectionBoxes.back()->mPosition;
//				mSelectionBoxes.back()->mOpacity = 0.25f;
//				mSelectionBoxes.back()->mEntityType = DPhoenix::ENTITY_SELECTION;
//				mSelectionBoxes.back()->mHalfSizes.x = 10.0f; mSelectionBoxes.back()->mHalfSizes.z = 10.0f;
//				mSelectionBoxes.back()->mCollidable = true;
//				AddEntity(mSelectionBoxes.back());
//			}
//		}
//	}
//
//
//}
//
//void BTLTMovementDemo::ResetPath()
//{
//	mHappyPath.clear();
//
//	for (int i = 0; i < mSelectionBoxes.size(); i++)
//	{
//		mSelectionBoxes[i]->mAlive = false;
//	}
//	
//	mSelectionBoxes.clear();
//
//	mHappyPath.push_back(new DPhoenix::PrimitiveInstance(*mSelectionHighlight));
//	mHappyPath.back()->mPosition = mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mModelInstance->mPosition;
//	mHappyPath.back()->mPosition.y = 10.5f;
//	mHappyPath.back()->mOpacity = 0.75f;
//
//	for (int col = 0; col < mMap->mWidth; col++)
//	{
//		for (int row = 0; row < mMap->mLength; row++)
//		{
//			if (mMap->mTiles[col][row]->mSpecialFlag == DPhoenix::HAPPY_PATH_SP_FLAG)
//				mMap->mTiles[col][row]->mSpecialFlag = DPhoenix::NO_SP_FLAG;
//		}
//	}
//
//	CheckSelectableRoute();
//
//}
//
////what to do on resize - mainly we'lll just call the parent / engine's method
//void BTLTMovementDemo::OnResize()
//{
//	Engine::OnResize();
//
//	//the window resized so the aspect ratio needs
//	//to be calculated again, and the projection matrix
//	mCamera->SetPerspective(0.25f * XM_PI, AspectRatio(), 1.0f, 100000.0f);
//}
//
////update scene - here is where our update logic for the game loop goes
//void BTLTMovementDemo::UpdateScene(float dt)
//{
//	//calculate scaling based on window dimensions
//	mScreenScale.x = (float)mClientWidth / 1440;
//	mScreenScale.y = (float)mClientHeight / 900;
//
//	//camera updating - get facing direction but no y component (horz)
//	//XMFLOAT3 facingDir = mCamEAngle.ToFloat3();
//	//facingDir.y = 0.0f;
//	//XMVECTOR facingVec = XMLoadFloat3(&facingDir);
//
//
//	switch (mGameState)
//	{
//	case GAME_MENU_STATE:
//
//		mDirLights.back()->Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
//		mDirLights.back()->Direction = XMFLOAT3(1.0f, 0.0f, 0.0f);
//
//		mToySoldierMenu->mModelInstance->mRotation.y += 1.0f * dt;
//		mDarkAngelMenu->mModelInstance->mRotation.y += 1.0f * dt;
//		mDragonMenu->mModelInstance->mRotation.y += 1.0f * dt;
//		mBigCatMenu->mModelInstance->mRotation.y += 1.0f * dt;
//
//		switch (mMenuState)
//		{
//		case CONFIRM_MENUSTATE:
//		case ENEMY_1_PICKED_MENUSTATE:
//		case ENEMY_2_PICKED_MENUSTATE:
//		case ENEMY_3_PICKED_MENUSTATE:
//
//			if (mWaitTimer.TotalTime() > 1.0f)
//			{
//				srand(time(0));
//				//random number between 0 and 3 to get class
//				int rng = rand() % +4;
//
//				switch (rng)
//				{
//				case 0:
//					mEnemyTeam->mTeamMembers.push_back(new DPhoenix::CharacterClass(*mToySoldierMenu));
//					break;
//				case 1:
//					mEnemyTeam->mTeamMembers.push_back(new DPhoenix::CharacterClass(*mDarkAngelMenu));
//					break;
//				case 2:
//					mEnemyTeam->mTeamMembers.push_back(new DPhoenix::CharacterClass(*mDragonMenu));
//					break;
//				case 3:
//					mEnemyTeam->mTeamMembers.push_back(new DPhoenix::CharacterClass(*mBigCatMenu));
//					break;
//				}
//
//				switch (mMenuState)
//				{
//				case CONFIRM_MENUSTATE: mMenuState = ENEMY_1_PICKED_MENUSTATE; break;
//				case ENEMY_1_PICKED_MENUSTATE: mMenuState = ENEMY_2_PICKED_MENUSTATE; break;
//				case ENEMY_2_PICKED_MENUSTATE: mMenuState = ENEMY_3_PICKED_MENUSTATE; break;
//				case ENEMY_3_PICKED_MENUSTATE: mMenuState = ENEMY_4_PICKED_MENUSTATE; break;
//				}
//
//
//				mWaitTimer.Reset();
//			}
//
//			break;
//		case ENEMY_4_PICKED_MENUSTATE:
//
//			if (mWaitTimer.TotalTime() > 2.0f)
//			{
//				mGameState = GAME_PLAY_STATE;
//
//				mPlayerTeam->mTeamMembers[0]->mModelInstance->mPosition = mPlayerSpawnVec[0];
//				mPlayerTeam->mTeamMembers[0]->mModelInstance->mRotation.y = 0;
//				mPlayerTeam->mTeamMembers[1]->mModelInstance->mPosition = mPlayerSpawnVec[1];
//				mPlayerTeam->mTeamMembers[1]->mModelInstance->mRotation.y = 0;
//				mPlayerTeam->mTeamMembers[2]->mModelInstance->mPosition = mPlayerSpawnVec[2];
//				mPlayerTeam->mTeamMembers[2]->mModelInstance->mRotation.y = 0;
//				mPlayerTeam->mTeamMembers[3]->mModelInstance->mPosition = mPlayerSpawnVec[3];
//				mPlayerTeam->mTeamMembers[3]->mModelInstance->mRotation.y = 0;
//
//				mPlayerTeam->mTeamType = DPhoenix::PLAYER_TEAM;
//				mPlayerTeam->CalculateTPPool();
//				mPlayerTeam->ResetTeamTurn();
//				mPlayerTeam->mStatustext = "Some test text for status bar";
//
//
//				//initial camera position --------------------------------------------------
//				float zBase = sin(XMConvertToRadians(mCamToFocusAngle));
//				float xBase = cos(XMConvertToRadians(mCamToFocusAngle));
//				mCamera->mPosition.x = mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mModelInstance->mPosition.x - (xBase * mCameraDistanceOffset);
//				mCamera->mPosition.y = mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mModelInstance->mPosition.y + mCameraDistanceOffset;
//				mCamera->mPosition.z = mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mModelInstance->mPosition.z - (zBase * mCameraDistanceOffset);
//				mCamera->mGoalPosition = mCamera->mPosition;
//
//				mCamera->mTargetPos = mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mModelInstance->mPosition;
//				mCamera->mGoalTarget = mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mModelInstance->mPosition;
//
//				//need initial line object an first piece of happy path --------------------------
//				mCameraLine = new::DPhoenix::Line();
//				AddEntity(mCameraLine);
//
//				mHappyPath.push_back(new DPhoenix::PrimitiveInstance(*mSelectionHighlight));
//				mHappyPath.back()->mPosition = mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mModelInstance->mPosition;
//				mHappyPath.back()->mPosition.y = 10.5f;
//				mHappyPath.back()->mOpacity = 0.75f;
//				CheckSelectableRoute();
//
//				mEnemyTeam->mTeamMembers[0]->mModelInstance->mPosition = mEnemySpawnVec[0];
//				mEnemyTeam->mTeamMembers[0]->mModelInstance->mRotation.y = 0;
//				mEnemyTeam->mTeamMembers[1]->mModelInstance->mPosition = mEnemySpawnVec[1];
//				mEnemyTeam->mTeamMembers[1]->mModelInstance->mRotation.y = 0;
//				mEnemyTeam->mTeamMembers[2]->mModelInstance->mPosition = mEnemySpawnVec[2];
//				mEnemyTeam->mTeamMembers[2]->mModelInstance->mRotation.y = 0;
//				mEnemyTeam->mTeamMembers[3]->mModelInstance->mPosition = mEnemySpawnVec[3];
//				mEnemyTeam->mTeamMembers[3]->mModelInstance->mRotation.y = 0;
//
//				mEnemyTeam->mTeamType = DPhoenix::ENEMY_TEAM;
//				mEnemyTeam->CalculateTPPool();
//				mEnemyTeam->ResetTeamTurn();
//
//				mDirLights[0]->Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
//			}
//
//			break;
//		}
//
//		break;
//	case GAME_PLAY_STATE:
//
//		float zBase = sin(XMConvertToRadians(mCamToFocusAngle));
//		float xBase = cos(XMConvertToRadians(mCamToFocusAngle));
//
//		DPhoenix::Team* activeTeam;
//		if (mCurrentTeam == DPhoenix::PLAYER_TEAM)
//			activeTeam = mPlayerTeam;
//		else if (mCurrentTeam == DPhoenix::ENEMY_TEAM)
//			activeTeam = mEnemyTeam;
//
//		//should set flag for 'focus' depending on different 'stuff';
//		XMFLOAT3 currentFocus;
//		if (activeTeam->mTeamMembers[activeTeam->mCurrentMember]->mTurnState == DPhoenix::CH_ACTIVE_TURNSTATE)
//		{
//			if (mCurrentTeam == DPhoenix::PLAYER_TEAM
//				&& mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mTurnState == DPhoenix::CH_ACTIVE_TURNSTATE
//				&& (mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mMoveState == DPhoenix::CH_PICKRT_MOVESTATE || 
//					mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mMoveState == DPhoenix::CH_PICKAC_MOVESTATE))
//			{
//				currentFocus = mHappyPath.back()->mPosition;
//				mCameraBehindFlag = false;
//			}
//			else
//			{
//				currentFocus = activeTeam->mTeamMembers[activeTeam->mCurrentMember]->mModelInstance->mPosition;
//				mCameraBehindFlag = true;
//			}
//		}
//
//		if (!mCameraBehindFlag)
//		{
//			mCamera->mGoalPosition.x = currentFocus.x - (xBase * mCameraDistanceOffset);
//			mCamera->mGoalPosition.y = currentFocus.y + mCameraDistanceOffset;
//			mCamera->mGoalPosition.z = currentFocus.z - (zBase * mCameraDistanceOffset);
//		}
//		else
//		{
//			//probably want to get the forward vector of the object being tracked for this to
//			//go fully behind the object
//					   			 		  		  
//			mCamera->mGoalPosition.x = currentFocus.x - activeTeam->mTeamMembers[activeTeam->mCurrentMember]->mModelInstance->mForwardVector.x * 15.0f;
//			mCamera->mGoalPosition.y = currentFocus.y;
//			mCamera->mGoalPosition.z = currentFocus.z - activeTeam->mTeamMembers[activeTeam->mCurrentMember]->mModelInstance->mForwardVector.z * 15.0f;
//		}
//
//#pragma region 
//
//		if (mCurrentTeam == DPhoenix::PLAYER_TEAM
//			&& mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mTurnState == DPhoenix::CH_ACTIVE_TURNSTATE
//			&& mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mMoveState == DPhoenix::CH_PICKRT_MOVESTATE)
//		{
//			//check collisions ---------------------------------
//
//			for (int i = 0; i < mSelectionBoxes.size(); i++)
//			{
//				if (mSelectionBoxes[i]->mCollided)
//				{
//					mSelectionBoxes[i]->mOpacity = 0.8f;
//				}
//				else
//				{
//					mSelectionBoxes[i]->mOpacity = 0.7f;
//				}
//			}		
//		}					
//
//#pragma endregion
//
//		mCamera->mGoalTarget = currentFocus;
//
//		mCamera->Update(dt);
//
//		//new line ------------------------------------------
//
//		// Move the mouse cursor coordinates into the -1 to +1 range.
//		//1600 x 900 is resolution of window
//
//		float mouseXbyorigin = mMouseX;
//		float mouseYbyorigin = mMouseY;
//
//
//		float pointX = ((2.0f * (float)mouseXbyorigin) / (float)1600) - 1.0f;
//		float pointY = (((2.0f * (float)mouseYbyorigin) / (float)900) - 1.0f) * -1.0f;
//
//		// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.		
//		pointX = pointX / mCamera->GetProj()._11;
//		pointY = pointY / mCamera->GetProj()._22;
//
//		// Get the inverse of the view matrix.
//		XMVECTOR det = XMMatrixDeterminant(mCamera->GetView());
//		XMMATRIX InverseView = XMMatrixInverse(&det, mCamera->GetView());
//
//		XMFLOAT3 lineDir;
//
//		// Calculate the direction of the picking ray in view space.
//		lineDir.x = (pointX * InverseView._11) + (pointY * InverseView._21) + InverseView._31;
//		lineDir.y = (pointX * InverseView._12) + (pointY * InverseView._22) + InverseView._32;
//		lineDir.z = (pointX * InverseView._13) + (pointY * InverseView._23) + InverseView._33;
//
//		//normalize
//		XMVECTOR linePointVec = XMLoadFloat3(&lineDir);
//		linePointVec = XMVector3Normalize(linePointVec);
//		XMStoreFloat3(&lineDir, linePointVec);
//
//		//update line properties
//		mCameraLine->Init(mCamera->mPosition, lineDir, 1000.0f);
//		mCameraLine->mEntityType = DPhoenix::ENTITY_CAMERA_LINE;
//		mCameraLine->mCollidable = true;
//
//		for (int i = 0; i < mPlayerTeam->mTeamMembers.size(); i++)
//		{
//			mPlayerTeam->mTeamMembers[i]->Update(dt);
//		}
//
//		for (int i = 0; i < mEnemyTeam->mTeamMembers.size(); i++)
//		{
//			mEnemyTeam->mTeamMembers[i]->Update(dt);
//		}
//
//		break;
//
//	}
//
//	//timer updates =====================================================
//	mMouseClickTimer.Tick();
//	mKeyPressTmer.Tick();
//	mWaitTimer.Tick();
//	mClickFlag = false;
//}
//
//void BTLTMovementDemo::BuildGeometryBuffers()
//{
//#pragma region GeometryCreation
//
//
//#pragma endregion GeometryCreation
//
//#pragma region BlendStates
//	//CREATE BLEND STATE ==========================================
//	//this is so transparency can be layered for our sprites
//
//	//create blend description struct and set properties
//	D3D11_BLEND_DESC blendDesc;
//	ZeroMemory(&blendDesc, sizeof(blendDesc));
//	blendDesc.RenderTarget[0].BlendEnable = TRUE;						//enable alpha blend
//	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//blend operation (add colour)
//	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			//source blend - alpha 
//	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	//dest blend - inverse source alpha
//	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;		//blend operation for alpha
//	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;			//blend based fully on source
//	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		//not blended based on destination
//																		//this can affect the blend / transparency with different hex values
//	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
//
//	//blend state (allow transparency)
//	ID3D11BlendState* alphaBlendState_;
//	//create the blend state with the properties set
//	//OM stands for output merger - final stage of shader pipeline
//	md3dDevice->CreateBlendState(&blendDesc, &alphaBlendState_);
//	md3dImmediateContext->OMSetBlendState(alphaBlendState_, 0, 0xffffffff);
//#pragma endregion BlendStates
//
//#pragma region DepthStencilStates
//
//	//DEPTH STENCIL STATES ==================================================
//	//disabled depth stenciling will enable for 2D sprites to be layered on top of each other
//	//we will use the standard depth stencilling for 3D rendering
//
//	// Initialize the description of the stencil state.
//	ZeroMemory(&mDepthStencilDesc, sizeof(mDepthStencilDesc));
//
//	// Set up the description of the stencil state.
//	//enable depth checking
//	mDepthStencilDesc.DepthEnable = true;
//	//use full buffer for depth checks
//	mDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//	//compare depth data
//	//If the source data is less than the destination data, the comparison passes.
//	mDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
//
//
//	//enable stencil testing
//	mDepthStencilDesc.StencilEnable = true;
//	//use all buffer for reading
//	mDepthStencilDesc.StencilReadMask = 0xFF;
//	//use all buffer for writing
//	mDepthStencilDesc.StencilWriteMask = 0xFF;
//
//	// Stencil operations if pixel is front-facing.
//	//keep if stencil test fails
//	mDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	//increment if stencil and depth fails
//	mDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
//	//keep if stencil test passes
//	mDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	//always compare data gainst existing
//	mDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//
//	// Stencil operations if pixel is back-facing.
//	//keep if stencil test fails
//	mDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	//decrement if stencil and depth fails
//	mDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
//	//keep if stencil test passes
//	mDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	//always compare data gainst existing
//	mDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//
//	// Create the depth stencil state.
//	HRESULT result = md3dDevice->CreateDepthStencilState(&mDepthStencilDesc, &mDepthStencilState);
//	if (FAILED(result))
//	{
//		//like oh no scoob
//		//return false;
//	}
//
//	// Set the depth stencil state.
//	md3dImmediateContext->OMSetDepthStencilState(mDepthStencilState, 1);
//
//	// Clear the second depth stencil state before setting the parameters.
//	ZeroMemory(&mDepthDisabledStencilDesc, sizeof(mDepthDisabledStencilDesc));
//
//	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
//	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
//	mDepthDisabledStencilDesc.DepthEnable = false;
//	mDepthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//	mDepthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
//	mDepthDisabledStencilDesc.StencilEnable = true;
//	mDepthDisabledStencilDesc.StencilReadMask = 0xFF;
//	mDepthDisabledStencilDesc.StencilWriteMask = 0xFF;
//	mDepthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	mDepthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
//	mDepthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	mDepthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//	mDepthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	mDepthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
//	mDepthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	mDepthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//
//	// Create the state using the device.
//	result = md3dDevice->CreateDepthStencilState(&mDepthDisabledStencilDesc, &mDepthDisabledStencilState);
//	if (FAILED(result))
//	{
//		//return false;
//	}
//
//#pragma endregion DepthStencilStates
//
//}
//
//void BTLTMovementDemo::DrawScene()
//{
//
//#pragma region InitialRenderingSetup
//	//do we have the context and swapchain?
//	assert(md3dImmediateContext);
//	assert(mSwapChain);
//
//	//set the effect techniques we wish to use
//	ID3DX11EffectTechnique* activeSpriteTech = DPhoenix::Effects::SpriteFX->SpriteTech;
//	ID3DX11EffectTechnique* activeColorTech = DPhoenix::Effects::ColorFX->ColorTech;
//	ID3DX11EffectTechnique* activeBasicTexTech = DPhoenix::Effects::BasicTexFX->BasicTexTech;
//	ID3DX11EffectTechnique* activeLitTexTech = DPhoenix::Effects::LitTexFX->LitTexTech;
//	ID3DX11EffectTechnique* activeLitTexNormalTech = DPhoenix::Effects::LitTexNormalFX->LitTexNormalTech;
//	ID3DX11EffectTechnique* activeSkyTech = DPhoenix::Effects::SkyFX->SkyTech;
//	ID3DX11EffectTechnique* activeLitBillboardTech = DPhoenix::Effects::LitBillboardFX->LitBillboardTech;
//	ID3DX11EffectTechnique* activeLitTexSkinnedTech = DPhoenix::Effects::LitTexFX->LitTexSkinnedTech;
//	ID3DX11EffectTechnique* activeLitTexNormSkinnedTech = DPhoenix::Effects::LitTexNormalFX->LitTexNormalSkinnedTech;
//
//	//clear the screen with the defined colour
//	float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
//	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, clearColor);
//	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::SpritePosTex);
//	//set primitive topology as triangle list (adjacent trinagles to render for GPU efficiency)
//	md3dImmediateContext->IASetPrimitiveTopology(
//		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	//clear all Direct2D before rendering any text
//	//(this overwrites the 3D clear color)
//	m2D1RT->BeginDraw();
//
//	m2D1RT->Clear(D2D1::ColorF(0x00000000));
//
//	m2D1RT->EndDraw();
//
//#pragma endregion InitialRenderingSetup
//
//#pragma region RenderSpritesBG
//
//	//to render sprites we need to disable depth testing (so it layers on top of each other)
//	//like a collage
//	md3dImmediateContext->OMSetDepthStencilState(mDepthDisabledStencilState, 1);
//
//	switch (mGameState)
//	{
//	case GAME_TITLE_STATE:
//		RenderSprite(mTitleScreen, activeSpriteTech, true);
//		break;
//	case GAME_MENU_STATE:
//		RenderSprite(mTeamMenuScreen, activeSpriteTech, true);
//		break;
//	}
//
//	//turn depth stencil back on for 3D rendering
//	md3dImmediateContext->OMSetDepthStencilState(mDepthStencilState, 1);
//
//#pragma endregion RenderSpritesBG
//
//#pragma region RenderColorPrimitives
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::PosColor);
//
//	//here we will render the box using our new method
//
//#pragma endregion RenderColorPrimitives
//
//#pragma region RenderBasicTexturedPrimitives
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::PosTex);
//
//#pragma endregion RenderBasicTexturedPrimitives
//
//#pragma region SetUpLights
//	//zero array memory
//	memset(mDirLightsArray, 0, sizeof(mDirLightsArray));
//
//	int i = 0;
//	//loop through the vector and dereference pointer
//	//to populate array for shader
//	for each(DirectionalLight* dirLight in mDirLights)
//	{
//		mDirLightsArray[i] = *dirLight;
//		i++;
//	}
//
//	//zero array memory
//	memset(mPointLightsArray, 0, sizeof(mPointLightsArray));
//
//	i = 0;
//	//loop through the vector and dereference pointer
//	//to populate array for shader
//	for each(PointLight* pointLight in mPointLights)
//	{
//		mPointLightsArray[i] = *pointLight;
//		mPointLightsArray[i].Range = mRange;
//		i++;
//	}
//
//#pragma endregion SetUplights
//
//#pragma region RenderLitTexturedPrimitives
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::PosNormalTexTan);
//
//
//#pragma endregion RenderLitTexturedPrimitives
//
//#pragma region RenderLitTexturedNormalPrimitives
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::PosNormalTexTan);
//
//	switch (mGameState)
//	{
//	case GAME_MENU_STATE:
//		RenderLitTexNormPrimitive(mToySoldierMenu->mModelInstance, activeLitTexNormalTech);
//		RenderLitTexNormPrimitive(mDarkAngelMenu->mModelInstance, activeLitTexNormalTech);
//		RenderLitTexNormPrimitive(mDragonMenu->mModelInstance, activeLitTexNormalTech);
//		RenderLitTexNormPrimitive(mBigCatMenu->mModelInstance, activeLitTexNormalTech);
//		break;
//	case GAME_PLAY_STATE:
//
//		for (int row = 0; row < mMap->mLength; row++)
//		{
//			for (int col = 0; col < mMap->mWidth; col++)
//			{
//
//				if (mMap->mTiles[col][row]->mMapBlockType == DPhoenix::FLOOR_MAPBLOCK ||
//					mMap->mTiles[col][row]->mMapBlockType == DPhoenix::WALL_MAPBLOCK ||
//					mMap->mTiles[col][row]->mMapBlockType == DPhoenix::COVER_MAPBLOCK ||
//					mMap->mTiles[col][row]->mMapBlockType == DPhoenix::BEACON_MAPBLOCK)
//				{
//					RenderLitTexNormPrimitive(mMap->mTiles[col][row]->mMeshInstance, activeLitTexNormalTech);
//				}
//			}
//		}
//
//		for (int i = 0; i < mPlayerTeam->mTeamMembers.size(); i++)
//		{
//			RenderLitTexNormPrimitive(mPlayerTeam->mTeamMembers[i]->mModelInstance, activeLitTexNormalTech);
//		}
//
//		for (int i = 0; i < mEnemyTeam->mTeamMembers.size(); i++)
//		{
//			RenderLitTexNormPrimitive(mEnemyTeam->mTeamMembers[i]->mModelInstance, activeLitTexNormalTech);
//		}
//
//#pragma region Selections
//
//		for (int i = 0; i < mHappyPath.size(); i++)
//		{
//			RenderLitTexNormPrimitive(mHappyPath[i], activeLitTexNormalTech);
//		}
//
//		for (int i = 0; i < mSelectionBoxes.size(); i++)
//		{
//			RenderLitTexNormPrimitive(mSelectionBoxes[i], activeLitTexNormalTech);
//		}
//
//#pragma endregion
//
//		break;
//		
//	}
//
//#pragma endregion RenderLitTexturedNormalPrimitives
//
//#pragma region RenderSkinnedMeshes
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::PosNormalTexTanSkinned);
//
//#pragma endregion RenderSkinnedMeshes
//
//#pragma region RenderSkinnedNormalMeshes
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::PosNormalTexTanSkinned);
//
//
//#pragma endregion RenderSkinnedNormalMeshes
//
//#pragma region RenderSkybox
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::Pos);
//
//	switch (mGameState)
//	{
//	case GAME_PLAY_STATE:
//		RenderSkybox(mSkybox, md3dImmediateContext);
//		break;
//	}
//
//#pragma endregion RenderSkybox
//
//#pragma region RenderBillboards
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::PosNormalTexTan);
//
//
//#pragma endregion RenderBillboards
//
//#pragma region RenderBasicMeshes
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::PosNormalTexTan);
//
//
//#pragma endregion RenderBasicMeshes
//
//#pragma region RenderBasicNormalMeshes
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::PosNormalTexTan);
//
//#pragma endregion RenderBasicNormalMeshes
//
//
//#pragma region RenderSpritesFG
//
//	//to render sprites we need to disable depth testing (so it layers on top of each other)
//	//like a collage
//	md3dImmediateContext->OMSetDepthStencilState(mDepthDisabledStencilState, 1);
//
//	switch (mGameState)
//	{
//	case GAME_MENU_STATE:
//		RenderSprite(mSelectedClassMenu, activeSpriteTech, true);
//		break;
//	case GAME_PLAY_STATE:
//
//#pragma region UIAreas
//
//		RenderSprite(mActionsUISprite, activeSpriteTech, true);
//		RenderSprite(mBeaconsUISprite, activeSpriteTech, true);
//		RenderSprite(mEnemyUISprite, activeSpriteTech, true);
//		RenderSprite(mStatusUISprite, activeSpriteTech, true);
//		RenderSprite(mPlayerTeamUISprite, activeSpriteTech, true);
//		RenderSprite(mSavesUISprite, activeSpriteTech, true);
//
//#pragma endregion UIAreas
//
//#pragma region Avatars
//		//This goes on for a bit.............
//		//LOTS of copypasta, there has to be a better way...
//		//player avatars ----------------------------
//
//		float initialX = -550.0f;
//		float initialY = -150.0f;
//
//		for (int i = 0; i < mPlayerTeam->mTeamMembers.size(); i++, initialY -= 50.0f)
//		{
//			switch (mPlayerTeam->mTeamMembers[i]->mClass)
//			{
//			case DPhoenix::TOY_SOLDIER_CLASS:
//				mToySoldierAvatar->mPosition.x = initialX + 25.0f;
//				mToySoldierAvatar->mPosition.y = initialY - 12.5f;
//				RenderSprite(mToySoldierAvatar, activeSpriteTech, true);
//				break;
//			case DPhoenix::DARK_ANGEL_CLASS:
//				mDarkAngelAvatar->mPosition.x = initialX + 25.0f;
//				mDarkAngelAvatar->mPosition.y = initialY - 12.5f;
//				RenderSprite(mDarkAngelAvatar, activeSpriteTech, true);
//				break;
//			case DPhoenix::DRAGON_CLASS:
//				mDragonAvatar->mPosition.x = initialX + 25.0f;
//				mDragonAvatar->mPosition.y = initialY - 12.5f;
//				RenderSprite(mDragonAvatar, activeSpriteTech, true);
//				break;
//			case DPhoenix::BIG_CAT_CLASS:
//				mBigCatAvatar->mPosition.x = initialX + 25.0f;
//				mBigCatAvatar->mPosition.y = initialY - 12.5f;
//				RenderSprite(mBigCatAvatar, activeSpriteTech, true);
//				break;
//			}
//		}
//
//		initialX = 525.0f;
//		initialY = 150.0f;
//
//		for (int i = 0; i < mEnemyTeam->mTeamMembers.size(); i++, initialY -= 50.0f)
//		{
//			switch (mEnemyTeam->mTeamMembers[i]->mClass)
//			{
//			case DPhoenix::TOY_SOLDIER_CLASS:
//				mToySoldierAvatar->mPosition.x = initialX + 25.0f;
//				mToySoldierAvatar->mPosition.y = initialY - 12.5f;
//				RenderSprite(mToySoldierAvatar, activeSpriteTech, true);
//				break;
//			case DPhoenix::DARK_ANGEL_CLASS:
//				mDarkAngelAvatar->mPosition.x = initialX + 25.0f;
//				mDarkAngelAvatar->mPosition.y = initialY - 12.5f;
//				RenderSprite(mDarkAngelAvatar, activeSpriteTech, true);
//				break;
//			case DPhoenix::DRAGON_CLASS:
//				mDragonAvatar->mPosition.x = initialX + 25.0f;
//				mDragonAvatar->mPosition.y = initialY - 12.5f;
//				RenderSprite(mDragonAvatar, activeSpriteTech, true);
//				break;
//			case DPhoenix::BIG_CAT_CLASS:
//				mBigCatAvatar->mPosition.x = initialX + 25.0f;
//				mBigCatAvatar->mPosition.y = initialY - 12.5f;
//				RenderSprite(mBigCatAvatar, activeSpriteTech, true);
//				break;
//			}
//		}
//
//
//
//#pragma endregion Avatars
//
//		break;
//
//	}
//
//	//turn depth stencil back on for 3D rendering
//	md3dImmediateContext->OMSetDepthStencilState(mDepthStencilState, 1);
//
//#pragma endregion RenderSpritesFG
//
//#pragma region RenderFonts
//
//	//string stream object for getting all text to display
//	std::ostringstream debugText;
//
//	//debugText << "Debug text:" << std::endl;
//	//debugText << "Mouse X:" << mMouseX << std::endl;
//	//debugText << "Mouse Y:" << mMouseY << std::endl;
//
//
//	//calculate screen scaling
//	float scaleX = (float)mClientWidth / 1440;
//	float scaleY = (float)mClientHeight / 900;
//
//	//call draw font method sending all key info
//	mDebugFont->DrawFont(m2D1RT, debugText.str(), 10.0f, 500.0f,
//		1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 0.5f));
//
//	//all stringstreams before the switch / case as intellisense doesn't like
//	//variables with scope local to cases
//	//for team selection menu -----------------------------------------------
//	std::ostringstream leveltext;
//	std::ostringstream classesText;
//	std::ostringstream tsText;
//	std::ostringstream daText;
//	std::ostringstream dText;
//	std::ostringstream bcText;
//	//for gameplay UI -------------------------------------------------------
//	std::ostringstream statusText;
//	std::ostringstream statsText;
//	std::ostringstream actionsText;
//	std::ostringstream beaconsText;
//	std::ostringstream savesText;
//
//	//offsets for easier tweaking
//	float xOffset = 0.0f;
//	float yOffset = 0.0f;
//
//
//	switch (mGameState)
//	{
//	case GAME_MENU_STATE:
//
//		leveltext << mSelectedLevel;
//
//		mUIFont90pt->DrawFont(m2D1RT, leveltext.str(), 1350.0f, 200.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//
//#pragma region SelectedClasses
//
//
//
//		if (mPlayerTeam->mTeamMembers.size() > 0)
//		{
//			switch (mPlayerTeam->mTeamMembers[0]->mClass)
//			{
//			case DPhoenix::TOY_SOLDIER_CLASS:
//				classesText << "TOY SOLDIER" << std::endl;
//				break;
//			case DPhoenix::DARK_ANGEL_CLASS:
//				classesText << "DARK ANGEL" << std::endl;
//				break;
//			case DPhoenix::DRAGON_CLASS:
//				classesText << "DRAGON" << std::endl;
//				break;
//			case DPhoenix::BIG_CAT_CLASS:
//				classesText << "BIG CAT" << std::endl;
//				break;
//			}
//		}
//
//		//call draw font method sending all key info
//		//thankfully, fonts are from top-left (but drawn from the bottom!!!!)
//		mUIFont36pt->DrawFont(m2D1RT, classesText.str(), 1250.0f, 350.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//
//		classesText.clear(); classesText.str("");
//
//
//		if (mPlayerTeam->mTeamMembers.size() > 1)
//		{
//			switch (mPlayerTeam->mTeamMembers[1]->mClass)
//			{
//			case DPhoenix::TOY_SOLDIER_CLASS:
//				classesText << "TOY SOLDIER" << std::endl;
//				break;
//			case DPhoenix::DARK_ANGEL_CLASS:
//				classesText << "DARK ANGEL" << std::endl;
//				break;
//			case DPhoenix::DRAGON_CLASS:
//				classesText << "DRAGON" << std::endl;
//				break;
//			case DPhoenix::BIG_CAT_CLASS:
//				classesText << "BIG CAT" << std::endl;
//				break;
//			}
//		}
//
//		//call draw font method sending all key info
//		//thankfully, fonts are from top-left
//		mUIFont36pt->DrawFont(m2D1RT, classesText.str(), 1250.0f, 400.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//
//		classesText.clear(); classesText.str("");
//
//		if (mPlayerTeam->mTeamMembers.size() > 2)
//		{
//			switch (mPlayerTeam->mTeamMembers[2]->mClass)
//			{
//			case DPhoenix::TOY_SOLDIER_CLASS:
//				classesText << "TOY SOLDIER" << std::endl;
//				break;
//			case DPhoenix::DARK_ANGEL_CLASS:
//				classesText << "DARK ANGEL" << std::endl;
//				break;
//			case DPhoenix::DRAGON_CLASS:
//				classesText << "DRAGON" << std::endl;
//				break;
//			case DPhoenix::BIG_CAT_CLASS:
//				classesText << "BIG CAT" << std::endl;
//				break;
//			}
//		}
//
//		//call draw font method sending all key info
//		//thankfully, fonts are from top-left
//		mUIFont36pt->DrawFont(m2D1RT, classesText.str(), 1250.0f, 450.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//
//		classesText.clear(); classesText.str("");
//
//		if (mPlayerTeam->mTeamMembers.size() > 3)
//		{
//			switch (mPlayerTeam->mTeamMembers[3]->mClass)
//			{
//			case DPhoenix::TOY_SOLDIER_CLASS:
//				classesText << "TOY SOLDIER" << std::endl;
//				break;
//			case DPhoenix::DARK_ANGEL_CLASS:
//				classesText << "DARK ANGEL" << std::endl;
//				break;
//			case DPhoenix::DRAGON_CLASS:
//				classesText << "DRAGON" << std::endl;
//				break;
//			case DPhoenix::BIG_CAT_CLASS:
//				classesText << "BIG CAT" << std::endl;
//				break;
//			}
//		}
//
//		//call draw font method sending all key info
//		//thankfully, fonts are from top-left
//		mUIFont36pt->DrawFont(m2D1RT, classesText.str(), 1250.0f, 500.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//
//#pragma endregion SelectedClasses
//
//#pragma region SelectedEnemyClasses
//
//		classesText.clear(); classesText.str("");
//
//		if (mEnemyTeam->mTeamMembers.size() > 0)
//		{
//			switch (mEnemyTeam->mTeamMembers[0]->mClass)
//			{
//			case DPhoenix::TOY_SOLDIER_CLASS:
//				classesText << "TOY SOLDIER" << std::endl;
//				break;
//			case DPhoenix::DARK_ANGEL_CLASS:
//				classesText << "DARK ANGEL" << std::endl;
//				break;
//			case DPhoenix::DRAGON_CLASS:
//				classesText << "DRAGON" << std::endl;
//				break;
//			case DPhoenix::BIG_CAT_CLASS:
//				classesText << "BIG CAT" << std::endl;
//				break;
//			}
//		}
//
//		//call draw font method sending all key info
//		//thankfully, fonts are from top-left (but drawn from the bottom!!!!)
//		mUIFont30pt->DrawFont(m2D1RT, classesText.str(), 1300.0f, 710.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//
//		classesText.clear(); classesText.str("");
//
//
//		if (mEnemyTeam->mTeamMembers.size() > 1)
//		{
//			switch (mEnemyTeam->mTeamMembers[1]->mClass)
//			{
//			case DPhoenix::TOY_SOLDIER_CLASS:
//				classesText << "TOY SOLDIER" << std::endl;
//				break;
//			case DPhoenix::DARK_ANGEL_CLASS:
//				classesText << "DARK ANGEL" << std::endl;
//				break;
//			case DPhoenix::DRAGON_CLASS:
//				classesText << "DRAGON" << std::endl;
//				break;
//			case DPhoenix::BIG_CAT_CLASS:
//				classesText << "BIG CAT" << std::endl;
//				break;
//			}
//		}
//
//		//call draw font method sending all key info
//		//thankfully, fonts are from top-left
//		mUIFont30pt->DrawFont(m2D1RT, classesText.str(), 1300.0f, 750.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//
//		classesText.clear(); classesText.str("");
//
//		if (mEnemyTeam->mTeamMembers.size() > 2)
//		{
//			switch (mEnemyTeam->mTeamMembers[2]->mClass)
//			{
//			case DPhoenix::TOY_SOLDIER_CLASS:
//				classesText << "TOY SOLDIER" << std::endl;
//				break;
//			case DPhoenix::DARK_ANGEL_CLASS:
//				classesText << "DARK ANGEL" << std::endl;
//				break;
//			case DPhoenix::DRAGON_CLASS:
//				classesText << "DRAGON" << std::endl;
//				break;
//			case DPhoenix::BIG_CAT_CLASS:
//				classesText << "BIG CAT" << std::endl;
//				break;
//			}
//		}
//
//		//call draw font method sending all key info
//		//thankfully, fonts are from top-left
//		mUIFont30pt->DrawFont(m2D1RT, classesText.str(), 1300.0f, 790.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//
//		classesText.clear(); classesText.str("");
//
//		if (mEnemyTeam->mTeamMembers.size() > 3)
//		{
//			switch (mEnemyTeam->mTeamMembers[3]->mClass)
//			{
//			case DPhoenix::TOY_SOLDIER_CLASS:
//				classesText << "TOY SOLDIER" << std::endl;
//				break;
//			case DPhoenix::DARK_ANGEL_CLASS:
//				classesText << "DARK ANGEL" << std::endl;
//				break;
//			case DPhoenix::DRAGON_CLASS:
//				classesText << "DRAGON" << std::endl;
//				break;
//			case DPhoenix::BIG_CAT_CLASS:
//				classesText << "BIG CAT" << std::endl;
//				break;
//			}
//		}
//
//		//call draw font method sending all key info
//		//thankfully, fonts are from top-left
//		mUIFont30pt->DrawFont(m2D1RT, classesText.str(), 1300.0f, 830.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//
//#pragma endregion SelectedEnemyClasses
//
//#pragma region ToySoldierStats
//
//
//
//		tsText << mToySoldierMenu->mTP << std::endl;
//		tsText << mToySoldierMenu->mHP << std::endl;
//		tsText << mToySoldierMenu->mMP << std::endl;
//		tsText << mToySoldierMenu->mPower << std::endl;
//		tsText << mToySoldierMenu->mFirepower << std::endl;
//		tsText << mToySoldierMenu->mDefense << std::endl;
//		tsText << mToySoldierMenu->mAccuracy << std::endl;
//		tsText << mToySoldierMenu->mEvasion << std::endl;
//		tsText << mToySoldierMenu->mMagic << std::endl;
//		tsText << mToySoldierMenu->mCharm << std::endl;
//
//		//call draw font method sending all key info
//		//thankfully, fonts are from top-left
//		mUIFont16pt->DrawFont(m2D1RT, tsText.str(), 200.0f, 650.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//
//#pragma endregion ToySoldierStats			
//
//#pragma region DarkAngelStats
//
//
//
//		daText << mDarkAngelMenu->mTP << std::endl;
//		daText << mDarkAngelMenu->mHP << std::endl;
//		daText << mDarkAngelMenu->mMP << std::endl;
//		daText << mDarkAngelMenu->mPower << std::endl;
//		daText << mDarkAngelMenu->mFirepower << std::endl;
//		daText << mDarkAngelMenu->mDefense << std::endl;
//		daText << mDarkAngelMenu->mAccuracy << std::endl;
//		daText << mDarkAngelMenu->mEvasion << std::endl;
//		daText << mDarkAngelMenu->mMagic << std::endl;
//		daText << mDarkAngelMenu->mCharm << std::endl;
//
//		//call draw font method sending all key info
//		//thankfully, fonts are from top-left
//		mUIFont16pt->DrawFont(m2D1RT, daText.str(), 500.0f, 650.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//
//#pragma endregion DarkAngelStats
//
//#pragma region DragonStats
//
//
//
//		dText << mDragonMenu->mTP << std::endl;
//		dText << mDragonMenu->mHP << std::endl;
//		dText << mDragonMenu->mMP << std::endl;
//		dText << mDragonMenu->mPower << std::endl;
//		dText << mDragonMenu->mFirepower << std::endl;
//		dText << mDragonMenu->mDefense << std::endl;
//		dText << mDragonMenu->mAccuracy << std::endl;
//		dText << mDragonMenu->mEvasion << std::endl;
//		dText << mDragonMenu->mMagic << std::endl;
//		dText << mDragonMenu->mCharm << std::endl;
//
//		//call draw font method sending all key info
//		//thankfully, fonts are from top-left
//		mUIFont16pt->DrawFont(m2D1RT, dText.str(), 800.0f, 650.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//
//#pragma endregion DragonStats
//
//#pragma region BigCatStats
//
//
//
//		bcText << mBigCatMenu->mTP << std::endl;
//		bcText << mBigCatMenu->mHP << std::endl;
//		bcText << mBigCatMenu->mMP << std::endl;
//		bcText << mBigCatMenu->mPower << std::endl;
//		bcText << mBigCatMenu->mFirepower << std::endl;
//		bcText << mBigCatMenu->mDefense << std::endl;
//		bcText << mBigCatMenu->mAccuracy << std::endl;
//		bcText << mBigCatMenu->mEvasion << std::endl;
//		bcText << mBigCatMenu->mMagic << std::endl;
//		bcText << mBigCatMenu->mCharm << std::endl;
//
//		//call draw font method sending all key info
//		//thankfully, fonts are from top-left
//		mUIFont16pt->DrawFont(m2D1RT, bcText.str(), 1100.0f, 650.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//
//#pragma endregion BigCatStats
//
//		break;
//	case GAME_PLAY_STATE:
//
//#pragma region PlayerStats
//
//		xOffset = 5.0f;
//		yOffset = -5.0f;
//
//		//TP Pool
//		statsText << mPlayerTeam->mTPPool;
//		mUIFont30pt->DrawFont(m2D1RT, statsText.str(), 200.0f, 550.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//
//		//first character
//		statsText << mPlayerTeam->mTeamMembers[0]->mHP << " / " << mPlayerTeam->mTeamMembers[0]->mMaxHP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 150.0f + xOffset, 600.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//
//		statsText << mPlayerTeam->mTeamMembers[0]->mMP << " / " << mPlayerTeam->mTeamMembers[0]->mMaxMP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 150.0f + xOffset, 625.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//		//2nd character
//		statsText << mPlayerTeam->mTeamMembers[1]->mHP << " / " << mPlayerTeam->mTeamMembers[1]->mMaxHP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 150.0f + xOffset, 650.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//
//		statsText << mPlayerTeam->mTeamMembers[1]->mMP << " / " << mPlayerTeam->mTeamMembers[1]->mMaxMP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 150.0f + xOffset, 675.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//		//3rd character
//		statsText << mPlayerTeam->mTeamMembers[2]->mHP << " / " << mPlayerTeam->mTeamMembers[2]->mMaxHP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 150.0f + xOffset, 700.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//
//		statsText << mPlayerTeam->mTeamMembers[2]->mMP << " / " << mPlayerTeam->mTeamMembers[2]->mMaxMP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 150.0f + xOffset, 725.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//		//4th character
//		statsText << mPlayerTeam->mTeamMembers[3]->mHP << " / " << mPlayerTeam->mTeamMembers[3]->mMaxHP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 150.0f + xOffset, 750.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//
//		statsText << mPlayerTeam->mTeamMembers[3]->mMP << " / " << mPlayerTeam->mTeamMembers[3]->mMaxMP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 150.0f + xOffset, 775.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//
//#pragma endregion PlayerStats
//
//#pragma region EnemyStats
//		//first character
//		statsText << mEnemyTeam->mTeamMembers[0]->mHP << " / " << mEnemyTeam->mTeamMembers[0]->mMaxHP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 1450.0f + xOffset, 305.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//
//		statsText << mEnemyTeam->mTeamMembers[0]->mMP << " / " << mEnemyTeam->mTeamMembers[0]->mMaxMP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 1450.0f + xOffset, 330.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//		//2nd character
//		statsText << mEnemyTeam->mTeamMembers[1]->mHP << " / " << mEnemyTeam->mTeamMembers[1]->mMaxHP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 1450.0f + xOffset, 355.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//
//		statsText << mEnemyTeam->mTeamMembers[1]->mMP << " / " << mEnemyTeam->mTeamMembers[1]->mMaxMP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 1450.0f + xOffset, 380.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//		//3rd character
//		statsText << mEnemyTeam->mTeamMembers[2]->mHP << " / " << mEnemyTeam->mTeamMembers[2]->mMaxHP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 1450.0f + xOffset, 405.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//
//		statsText << mEnemyTeam->mTeamMembers[2]->mMP << " / " << mEnemyTeam->mTeamMembers[2]->mMaxMP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 1450.0f + xOffset, 430.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//		//4th character
//		statsText << mEnemyTeam->mTeamMembers[3]->mHP << " / " << mEnemyTeam->mTeamMembers[3]->mMaxHP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 1450.0f + xOffset, 455.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//
//		statsText << mEnemyTeam->mTeamMembers[3]->mMP << " / " << mEnemyTeam->mTeamMembers[3]->mMaxMP;
//		mUIFont16pt->DrawFont(m2D1RT, statsText.str(), 1450.0f + xOffset, 480.0f + yOffset,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statsText.clear(); statsText.str("");
//
//#pragma endregion EnemyStats
//
//#pragma region StatusBar
//
//		if (mCurrentTeam == DPhoenix::PLAYER_TEAM)
//		{
//			statusText << mPlayerTeam->mStatustext;
//		}
//		else if (mCurrentTeam == DPhoenix::ENEMY_TEAM)
//		{
//			statusText << mEnemyTeam->mStatustext;
//		}
//
//		mUIFont36pt->DrawFont(m2D1RT, statusText.str(), 510.0f, 50.0f,
//			1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 1.0f));
//		statusText.clear(); statusText.str("");
//
//
//#pragma endregion StatusBar
//
//
//
//		break;
//
//	}
//
//
//
//
//#pragma endregion RenderFonts
//
//	//present to screen
//	HR(mSwapChain->Present(0, 0));
//
//}
//
//void BTLTMovementDemo::HandleEvents(DPhoenix::IEvent* e)
//{
//	switch (e->GetID())
//	{
//	case DPhoenix::EVENT_KEYPRESS:
//	{
//		DPhoenix::KeyPressEvent* kpe = (DPhoenix::KeyPressEvent*)e;
//
//		switch (kpe->mKeycode)
//		{
//		case DIK_UP:
//		case DIK_W:
//			if (mKeyPressTmer.TotalTime() > 0.1)
//			{
//				if (mCurrentTeam == DPhoenix::PLAYER_TEAM)
//				{
//					mPlayerTeam->NextMember();
//					ResetPath();
//				}
//				mKeyPressTmer.Reset();
//			}
//			break;
//		case DIK_DOWN:
//		case DIK_S:
//			if (mKeyPressTmer.TotalTime() > 0.1)
//			{
//				if (mCurrentTeam == DPhoenix::PLAYER_TEAM)
//				{
//					mPlayerTeam->PrevMember();
//					ResetPath();
//				}
//				mKeyPressTmer.Reset();
//			}
//			break;
//			break;
//		case DIK_LEFT:
//		case DIK_A:
//
//			if (mGameState == GAME_PLAY_STATE)
//			{
//				mCamToFocusAngle -= 1.0f;
//			}
//
//			break;
//		case DIK_RIGHT:
//		case DIK_D:
//			if (mGameState == GAME_PLAY_STATE)
//			{
//				mCamToFocusAngle += 1.0f;
//			}
//			break;
//		case DIK_ESCAPE:
//			//shutdown here
//			break;
//		case DIK_RETURN:
//			if (mKeyPressTmer.TotalTime() > 0.2)
//			{
//				if (mGameState == GAME_TITLE_STATE)
//				{
//					mGameState = GAME_MENU_STATE;
//				}
//				else if (mGameState == GAME_MENU_STATE)
//				{
//					if (mMenuState == CHOOSE_MENUSTATE &&
//						mPlayerTeam->mTeamMembers.size() == 4)
//					{
//						mMenuState = CONFIRM_MENUSTATE;
//						mWaitTimer.Reset();
//					}
//				}
//				else if (mGameState == GAME_PLAY_STATE)
//				{
//					if (mCurrentTeam == DPhoenix::PLAYER_TEAM &&
//						mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mMoveState == DPhoenix::CH_PICKRT_MOVESTATE)
//					{
//						mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mHappyPath = mHappyPath;
//						mPlayerTeam->mTeamMembers[mPlayerTeam->mCurrentMember]->mMoveState = DPhoenix::CH_MOVERT_MOVESTATE;
//					}
//
//				}
//
//				break;
//
//				mKeyPressTmer.Reset();
//			}
//		}
//
//	}
//	break;
//	case DPhoenix::EVENT_KEYRELEASE:
//	{
//		DPhoenix::KeyReleaseEvent* kre = (DPhoenix::KeyReleaseEvent*)e;
//		switch (kre->mKeycode)
//		{
//		case DIK_Q:
//			mCameraBehindFlag = true;
//			break;
//		case DIK_E:
//			mCameraBehindFlag = false;
//			break;
//		}
//	}
//	break;
//	case DPhoenix::EVENT_MOUSEMOTION:
//	{
//		DPhoenix::MouseMotionEvent* mme = (DPhoenix::MouseMotionEvent*)e;
//		float movex = mme->mDeltaX;		//difference in horizontal movement
//		float movey = mme->mDeltaY;		//difference in vertical movement
//
//	}
//	break;
//	case DPhoenix::EVENT_MOUSEMOVE:
//	{
//		DPhoenix::MouseMoveEvent* mme = (DPhoenix::MouseMoveEvent*)e;
//		mMouseX = mme->mPosX;
//		mMouseY = mme->mPosY;
//
//		switch (mGameState)
//		{
//		case GAME_MENU_STATE:
//			if (mMenuState == CHOOSE_MENUSTATE)
//				CheckMouseHoverMenu();
//			break;
//		}
//	}
//	break;
//	case DPhoenix::EVENT_MOUSECLICK:
//	{
//		DPhoenix::MouseClickEvent* mc = (DPhoenix::MouseClickEvent*)e;
//		
//		if (mMouseClickTimer.TotalTime() > 0.2)
//		{
//			switch (mGameState)
//			{
//			case GAME_MENU_STATE:
//				if (mc->mButton == 0 && mMenuState == CHOOSE_MENUSTATE)
//					CheckMouseClickMenu();
//				break;
//			case GAME_PLAY_STATE:
//				if (mc->mButton == 0)
//					CheckMouseClickRoute();
//				break;
//			}
//
//			mMouseClickTimer.Reset();
//		}
//
//	}
//	break;
//	case DPhoenix::EVENT_MOUSEWHEEL:
//	{
//		DPhoenix::MouseWheelEvent* mwe = (DPhoenix::MouseWheelEvent*)e;
//
//		switch (mGameState)
//		{
//		case GAME_PLAY_STATE:
//
//			mCameraDistanceOffset -= mwe->mWheel * 0.1f;
//
//			if (mCameraDistanceOffset < 10.0f)
//				mCameraDistanceOffset = 10.0f;
//			if (mCameraDistanceOffset > 100.0f)
//				mCameraDistanceOffset = 100.0f;
//
//		}
//
//	}
//	break;
//	}
//}
//
//void BTLTMovementDemo::InitAudio()
//{
//
//}
//
//void BTLTMovementDemo::RenderSprite(DPhoenix::Sprite * sprite, ID3DX11EffectTechnique * tech, bool isHUD)
//{
//	//here we calculate the scale on X and Y based on the client width / height
//	//vs. our original resolution 
//	//float scaleX = mScreenScale.x;
//	//float scaleY = mScreenScale.y;
//
//	float scaleX = 1.0f;
//	float scaleY = 1.0f;
//
//	//add to a vector to use in matrices
//	XMFLOAT3 scaleVec; scaleVec.x = scaleX;
//	scaleVec.y = scaleY; scaleVec.z = 1.0f;
//
//	//create view / projection matrix
//	XMMATRIX viewProj;
//
//	//HUD objects not affected by cameras
//	if (!isHUD)
//	{
//		//here you would add any 2D camera control
//	}
//	else
//	{
//		//an identity matrix is standard and used as a basis
//		//for other calculations
//		XMMATRIX view = XMMatrixIdentity();
//		//orthographic, left handed co-ordinates
//		XMMATRIX projection = XMMatrixOrthographicLH(
//			(float)mClientWidth,	//width of viewport
//			(float)mClientHeight,	//height of viewport
//			0.1f,					//nearplane (Z)
//			100.0f					//far plane (Z)
//		);
//
//		//create view / projection matrix (multiply / concatenate)
//		//these represent the 'camera'
//		viewProj = XMMatrixMultiply(view, projection);
//	}
//
//	//concatenate world / view / projection matrices for rendering
//	XMMATRIX world = sprite->CalculateTransforms(scaleVec, isHUD);
//	XMMATRIX worldViewProj = XMMatrixMultiply(world, viewProj);
//	DPhoenix::Effects::SpriteFX->SetWorldViewProj(worldViewProj);
//
//	//now we get all the matching Sprite properties to send to the shader
//	DPhoenix::Effects::SpriteFX->SetDiffuseMap(sprite->GetShaderResourceView());			//texture
//	DPhoenix::Effects::SpriteFX->SetColumn(sprite->GetCurrentFrame());						//col / frame
//	DPhoenix::Effects::SpriteFX->SetNumCols(sprite->mAnimationColumns);						//num frames
//	DPhoenix::Effects::SpriteFX->SetFlipValue(sprite->mFlipValue);							//flip value
//	DPhoenix::Effects::SpriteFX->SetLerpValue(sprite->mColorLerpValue);						//lerp value
//	DPhoenix::Effects::SpriteFX->SetOpacityValue(sprite->mOpacityValue);					//opacity
//	DPhoenix::Effects::SpriteFX->SetAltColorValue(XMLoadFloat4(&sprite->mAltColorValue));	//alt color
//
//																							//render using effect shader technique
//	D3DX11_TECHNIQUE_DESC techDesc;
//	tech->GetDesc(&techDesc);
//	for (UINT p = 0; p < techDesc.Passes; ++p)
//	{
//		//use current context and get current rendering pass
//		tech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
//
//		//call Render method of Sprite object
//		sprite->Render(md3dImmediateContext);
//	}
//
//	//reset the lerp value on the shader
//	DPhoenix::Effects::SpriteFX->SetLerpValue(0.0f);
//}
//
//void BTLTMovementDemo::RenderColorPrimitive(DPhoenix::PrimitiveInstance * model,
//	ID3DX11EffectTechnique * tech)
//{
//	//here we calculate the scale on X and Y based on the client width / height
//	//vs. our original resolution 
//	float scaleX = mScreenScale.x;
//	float scaleY = mScreenScale.y;
//
//	//add to a vector to use in matrices
//	XMFLOAT3 scaleVec; scaleVec.x = scaleX;
//	scaleVec.y = scaleY; scaleVec.z = 1.0f;
//
//	//create view / projection matrix
//	XMMATRIX view = mCamera->GetView();
//	XMMATRIX proj = mCamera->GetProj();
//
//	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
//
//	//concatenate world / view / projection matrices for rendering
//	XMMATRIX world = model->CalculateTransforms();
//	//XMMATRIX world = Identity;
//	XMMATRIX worldViewProj = XMMatrixMultiply(world, viewProj);
//	DPhoenix::Effects::ColorFX->SetWorldViewProj(worldViewProj);
//
//	DPhoenix::Effects::ColorFX->SetOpacityValue(model->mOpacity);
//
//	//render using effect shader technique
//	D3DX11_TECHNIQUE_DESC techDesc;
//	tech->GetDesc(&techDesc);
//	for (UINT p = 0; p < techDesc.Passes; ++p)
//	{
//		//use current context and get current rendering pass
//		tech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
//
//		//call Render method of Sprite object
//		model->Render(md3dImmediateContext);
//	}
//}
//
//void BTLTMovementDemo::RenderBasicTexPrimitive(DPhoenix::PrimitiveInstance * model,
//	ID3DX11EffectTechnique * tech)
//{
//	//here we calculate the scale on X and Y based on the client width / height
//	//vs. our original resolution 
//	float scaleX = mScreenScale.x;
//	float scaleY = mScreenScale.y;
//
//	//add to a vector to use in matrices
//	XMFLOAT3 scaleVec; scaleVec.x = scaleX;
//	scaleVec.y = scaleY; scaleVec.z = 1.0f;
//
//	//create view / projection matrix
//	XMMATRIX view = mCamera->GetView();
//	XMMATRIX proj = mCamera->GetProj();
//
//	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
//
//	//concatenate world / view / projection matrices for rendering
//	XMMATRIX world = model->CalculateTransforms();
//	//XMMATRIX world = Identity;
//	XMMATRIX worldViewProj = XMMatrixMultiply(world, viewProj);
//	DPhoenix::Effects::BasicTexFX->SetWorldViewProj(worldViewProj);
//	DPhoenix::Effects::BasicTexFX->SetAmbLight(XMLoadFloat4(&mAmbientColor));
//	DPhoenix::Effects::BasicTexFX->SetAmbIntensity(mAmbIntensity);
//	DPhoenix::Effects::BasicTexFX->SetDiffuseMap(model->mDiffuseMap);
//	DPhoenix::Effects::BasicTexFX->SetTexScaleU(model->mTexScaleU);
//	DPhoenix::Effects::BasicTexFX->SetTexScaleV(model->mTexScaleV);
//
//	DPhoenix::Effects::BasicTexFX->SetOpacityValue(model->mOpacity);
//
//	//render using effect shader technique
//	D3DX11_TECHNIQUE_DESC techDesc;
//	tech->GetDesc(&techDesc);
//	for (UINT p = 0; p < techDesc.Passes; ++p)
//	{
//		//use current context and get current rendering pass
//		tech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
//
//		//call Render method of Sprite object
//		model->Render(md3dImmediateContext);
//	}
//}
//
//void BTLTMovementDemo::RenderLitTexPrimitive(DPhoenix::PrimitiveInstance * model,
//	ID3DX11EffectTechnique * tech)
//{
//
//	//create view / projection matrix
//	XMMATRIX view = mCamera->GetView();
//	XMMATRIX proj = mCamera->GetProj();
//
//	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
//
//	//concatenate world / view / projection matrices for rendering
//	XMMATRIX world = model->CalculateTransforms();
//
//	DPhoenix::Effects::LitTexFX->SetWorld(
//		world
//	);
//
//	XMMATRIX worldViewProj = XMMatrixMultiply(world, viewProj);
//	DPhoenix::Effects::LitTexFX->SetWorldViewProj(worldViewProj);
//
//	// Inverse-transpose is just applied to normals.  So zero out 
//	// translation row so that it doesn't get into our inverse-transpose
//	// calculation--we don't want the inverse-transpose of the translation.
//	XMMATRIX A = world;
//	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//
//	XMVECTOR det = XMMatrixDeterminant(A);
//
//	DPhoenix::Effects::LitTexFX->SetWorldInvTranspose(
//		XMMatrixTranspose(XMMatrixInverse(&det, A))
//	);
//
//	DPhoenix::Effects::LitTexFX->SetAmbLight(XMLoadFloat4(&mAmbientColor));
//	DPhoenix::Effects::LitTexFX->SetAmbIntensity(mAmbIntensity);
//	DPhoenix::Effects::LitTexFX->SetDiffuseMap(model->mDiffuseMap);
//
//	DPhoenix::Effects::LitTexFX->SetEyePosW(mCamera->mPosition);
//	DPhoenix::Effects::LitTexFX->SetFogEnabled(mFogEnabled);
//	DPhoenix::Effects::LitTexFX->SetFogColor(XMLoadFloat4(&mFogColor));
//	DPhoenix::Effects::LitTexFX->SetFogStart(mFogStart);
//	DPhoenix::Effects::LitTexFX->SetFogRange(mFogRange);
//
//	DPhoenix::Effects::LitTexFX->SetOpacityValue(model->mOpacity);
//
//	DPhoenix::Effects::LitTexFX->SetDirLights(mDirLightsArray);
//	DPhoenix::Effects::LitTexFX->SetMaterial(*model->mMaterial);
//
//	DPhoenix::Effects::LitTexFX->SetDirLightCount(mNumDirLights);
//
//	DPhoenix::Effects::LitTexFX->SetPointLights(mPointLightsArray);
//	DPhoenix::Effects::LitTexFX->SetPointLightCount(mNumPointLights);
//
//	//render using effect shader technique
//	D3DX11_TECHNIQUE_DESC techDesc;
//	tech->GetDesc(&techDesc);
//	for (UINT p = 0; p < techDesc.Passes; ++p)
//	{
//		//use current context and get current rendering pass
//		tech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
//
//		//call Render method of Sprite object
//		model->Render(md3dImmediateContext);
//	}
//}
//
//void BTLTMovementDemo::RenderLitTexNormPrimitive(DPhoenix::PrimitiveInstance * model,
//	ID3DX11EffectTechnique * tech)
//{
//
//	//create view / projection matrix
//	XMMATRIX view = mCamera->GetView();
//	XMMATRIX proj = mCamera->GetProj();
//
//	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
//
//	//concatenate world / view / projection matrices for rendering
//	XMMATRIX world = model->CalculateTransforms();
//
//	DPhoenix::Effects::LitTexNormalFX->SetWorld(
//		world
//	);
//
//	XMMATRIX worldViewProj = XMMatrixMultiply(world, viewProj);
//	DPhoenix::Effects::LitTexNormalFX->SetWorldViewProj(worldViewProj);
//
//	// Inverse-transpose is just applied to normals.  So zero out 
//	// translation row so that it doesn't get into our inverse-transpose
//	// calculation--we don't want the inverse-transpose of the translation.
//	XMMATRIX A = world;
//	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//
//	XMVECTOR det = XMMatrixDeterminant(A);
//
//	DPhoenix::Effects::LitTexNormalFX->SetWorldInvTranspose(
//		XMMatrixTranspose(XMMatrixInverse(&det, A))
//	);
//
//	DPhoenix::Effects::LitTexNormalFX->SetAmbLight(XMLoadFloat4(&mAmbientColor));
//	DPhoenix::Effects::LitTexNormalFX->SetAmbIntensity(mAmbIntensity);
//	DPhoenix::Effects::LitTexNormalFX->SetDiffuseMap(model->mDiffuseMap);
//
//	DPhoenix::Effects::LitTexNormalFX->SetNormalMap(model->mNormalMap);
//
//	DPhoenix::Effects::LitTexNormalFX->SetEyePosW(mCamera->mPosition);
//	DPhoenix::Effects::LitTexNormalFX->SetFogEnabled(mFogEnabled);
//	DPhoenix::Effects::LitTexNormalFX->SetFogColor(XMLoadFloat4(&mFogColor));
//	DPhoenix::Effects::LitTexNormalFX->SetFogStart(mFogStart);
//	DPhoenix::Effects::LitTexNormalFX->SetFogRange(mFogRange);
//
//	DPhoenix::Effects::LitTexNormalFX->SetOpacityValue(model->mOpacity);
//
//	DPhoenix::Effects::LitTexNormalFX->SetDirLights(mDirLightsArray);
//	DPhoenix::Effects::LitTexNormalFX->SetMaterial(*model->mMaterial);
//
//	DPhoenix::Effects::LitTexNormalFX->SetDirLightCount(mNumDirLights);
//
//	DPhoenix::Effects::LitTexNormalFX->SetPointLights(mPointLightsArray);
//	DPhoenix::Effects::LitTexNormalFX->SetPointLightCount(mNumPointLights);
//
//	//render using effect shader technique
//	D3DX11_TECHNIQUE_DESC techDesc;
//	tech->GetDesc(&techDesc);
//	for (UINT p = 0; p < techDesc.Passes; ++p)
//	{
//		//use current context and get current rendering pass
//		tech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
//
//		//call Render method of Sprite object
//		model->Render(md3dImmediateContext);
//	}
//}
//
////call skybox draw call
//void BTLTMovementDemo::RenderSkybox(DPhoenix::Sky* skybox,
//	ID3D11DeviceContext* dc)
//{
//	skybox->Draw(dc, mCamera);
//}
//
//void BTLTMovementDemo::RenderBillboardSprite(DPhoenix::BillboardSprite * sprite,
//	ID3DX11EffectTechnique * tech)
//{
//	//create view / projection matrix
//	XMMATRIX view = mCamera->GetView();
//	XMMATRIX proj = mCamera->GetProj();
//
//	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
//
//	//concatenate world / view / projection matrices for rendering
//	XMMATRIX world = sprite->GetWorldBS(mCamera->mPosition);
//
//	DPhoenix::Effects::LitBillboardFX->SetWorld(
//		world
//	);
//
//	//XMMATRIX world = Identity;
//	XMMATRIX worldViewProj = XMMatrixMultiply(world, viewProj);
//	DPhoenix::Effects::LitBillboardFX->SetWorldViewProj(worldViewProj);
//
//	// Inverse-transpose is just applied to normals.  So zero out 
//	// translation row so that it doesn't get into our inverse-transpose
//	// calculation--we don't want the inverse-transpose of the translation.
//	XMMATRIX A = world;
//	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//
//	XMVECTOR det = XMMatrixDeterminant(A);
//
//	DPhoenix::Effects::LitBillboardFX->SetWorldInvTranspose(
//		XMMatrixTranspose(XMMatrixInverse(&det, A))
//	);
//
//	DPhoenix::Effects::LitBillboardFX->SetAmbLight(XMLoadFloat4(&mAmbientColor));
//	DPhoenix::Effects::LitBillboardFX->SetAmbIntensity(mAmbIntensity);
//	DPhoenix::Effects::LitBillboardFX->SetDiffuseMap(sprite->GetShaderResourceView());
//
//	DPhoenix::Effects::LitBillboardFX->SetEyePosW(mCamera->mPosition);
//	DPhoenix::Effects::LitBillboardFX->SetFogColor(XMLoadFloat4(&mFogColor));
//	DPhoenix::Effects::LitBillboardFX->SetFogStart(mFogStart);
//	DPhoenix::Effects::LitBillboardFX->SetFogRange(mFogRange);
//
//	Material* materialStandard = new Material();
//
//	materialStandard->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	materialStandard->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	materialStandard->Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f);
//	materialStandard->Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
//
//	DPhoenix::Effects::LitBillboardFX->SetMaterial(*materialStandard);
//
//	DPhoenix::Effects::LitBillboardFX->SetDirLights(mDirLightsArray);
//	DPhoenix::Effects::LitBillboardFX->SetDirLightCount(mNumDirLights);
//
//	DPhoenix::Effects::LitBillboardFX->SetPointLights(mPointLightsArray);
//	DPhoenix::Effects::LitBillboardFX->SetPointLightCount(mNumPointLights);
//
//	DPhoenix::Effects::LitBillboardFX->SetColumn(sprite->GetCurrentFrame());					//col / frame
//	DPhoenix::Effects::LitBillboardFX->SetNumCols(sprite->mAnimationColumns);					//num frames
//	DPhoenix::Effects::LitBillboardFX->SetFlipValue(sprite->mFlipValue);						//flip value
//	DPhoenix::Effects::LitBillboardFX->SetLerpValue(sprite->mColorLerpValue);					//lerp value
//	DPhoenix::Effects::LitBillboardFX->SetOpacityValue(sprite->mOpacityValue);					//opacity
//	DPhoenix::Effects::LitBillboardFX->SetAltColorValue(XMLoadFloat4(&sprite->mAltColorValue));	//alt color
//
//																								//render using effect shader technique
//	D3DX11_TECHNIQUE_DESC techDesc;
//	tech->GetDesc(&techDesc);
//	for (UINT p = 0; p < techDesc.Passes; ++p)
//	{
//		//use current context and get current rendering pass
//		tech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
//
//		//call Render method of Sprite object
//		sprite->RenderBS(md3dImmediateContext);
//	}
//}
//
//
//void BTLTMovementDemo::RenderBasicModel(DPhoenix::BasicModelInstance * model,
//	ID3DX11EffectTechnique * tech)
//{
//
//	//create view / projection matrix
//	XMMATRIX view = mCamera->GetView();
//	XMMATRIX proj = mCamera->GetProj();
//
//	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
//
//	//concatenate world / view / projection matrices for rendering
//	XMMATRIX world = model->CalculateTransforms();
//
//	DPhoenix::Effects::LitTexFX->SetWorld(
//		world
//	);
//
//	XMMATRIX worldViewProj = XMMatrixMultiply(world, viewProj);
//	DPhoenix::Effects::LitTexFX->SetWorldViewProj(worldViewProj);
//
//	// Inverse-transpose is just applied to normals.  So zero out 
//	// translation row so that it doesn't get into our inverse-transpose
//	// calculation--we don't want the inverse-transpose of the translation.
//	XMMATRIX A = world;
//	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//
//	XMVECTOR det = XMMatrixDeterminant(A);
//
//	DPhoenix::Effects::LitTexFX->SetWorldInvTranspose(
//		XMMatrixTranspose(XMMatrixInverse(&det, A))
//	);
//
//	DPhoenix::Effects::LitTexFX->SetAmbLight(XMLoadFloat4(&mAmbientColor));
//	DPhoenix::Effects::LitTexFX->SetAmbIntensity(mAmbIntensity);
//
//	DPhoenix::Effects::LitTexFX->SetEyePosW(mCamera->mPosition);
//	DPhoenix::Effects::LitTexFX->SetFogEnabled(mFogEnabled);
//	DPhoenix::Effects::LitTexFX->SetFogColor(XMLoadFloat4(&mFogColor));
//	DPhoenix::Effects::LitTexFX->SetFogStart(mFogStart);
//	DPhoenix::Effects::LitTexFX->SetFogRange(mFogRange);
//
//	DPhoenix::Effects::LitTexFX->SetOpacityValue(model->mOpacity);
//
//	DPhoenix::Effects::LitTexFX->SetDirLights(mDirLightsArray);
//
//	DPhoenix::Effects::LitTexFX->SetDirLightCount(mNumDirLights);
//
//	DPhoenix::Effects::LitTexFX->SetPointLights(mPointLightsArray);
//	DPhoenix::Effects::LitTexFX->SetPointLightCount(mNumPointLights);
//
//	//render using effect shader technique
//	D3DX11_TECHNIQUE_DESC techDesc;
//	tech->GetDesc(&techDesc);
//	for (UINT p = 0; p < techDesc.Passes; ++p)
//	{
//		//loop through subsets
//		for (UINT subset = 0; subset < model->Model->SubsetCount; ++subset)
//		{
//			//get pointer to the model / mesh data
//			DPhoenix::BasicModel* bmPtr = model->Model;
//			//get the material index for this subset
//			UINT materialIndex = bmPtr->Subsets[subset].materialIndex;
//			//set the material 
//			DPhoenix::Effects::LitTexFX->SetMaterial(bmPtr->Mat[materialIndex]);
//			//if we have a diffuse map then set it for this subset
//			if (bmPtr->DiffuseMapSRV[materialIndex] != NULL)
//			{
//				DPhoenix::Effects::LitTexFX->SetDiffuseMap(bmPtr->DiffuseMapSRV[materialIndex]);
//			}
//			else
//			{
//				DPhoenix::Effects::LitTexFX->SetDiffuseMap(NULL);
//			}
//			//call the draw method with the given subset
//			tech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
//			bmPtr->ModelMesh.Draw(md3dImmediateContext, subset);
//		}
//	}
//}
//
//void BTLTMovementDemo::RenderBasicModelNormal(DPhoenix::BasicModelInstance * model,
//	ID3DX11EffectTechnique * tech)
//{
//
//	//create view / projection matrix
//	XMMATRIX view = mCamera->GetView();
//	XMMATRIX proj = mCamera->GetProj();
//
//	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
//
//	//concatenate world / view / projection matrices for rendering
//	XMMATRIX world = model->CalculateTransforms();
//
//	DPhoenix::Effects::LitTexNormalFX->SetWorld(
//		world
//	);
//
//	XMMATRIX worldViewProj = XMMatrixMultiply(world, viewProj);
//	DPhoenix::Effects::LitTexNormalFX->SetWorldViewProj(worldViewProj);
//
//	// Inverse-transpose is just applied to normals.  So zero out 
//	// translation row so that it doesn't get into our inverse-transpose
//	// calculation--we don't want the inverse-transpose of the translation.
//	XMMATRIX A = world;
//	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//
//	XMVECTOR det = XMMatrixDeterminant(A);
//
//	DPhoenix::Effects::LitTexNormalFX->SetWorldInvTranspose(
//		XMMatrixTranspose(XMMatrixInverse(&det, A))
//	);
//
//	DPhoenix::Effects::LitTexNormalFX->SetAmbLight(XMLoadFloat4(&mAmbientColor));
//	DPhoenix::Effects::LitTexNormalFX->SetAmbIntensity(mAmbIntensity);
//
//	DPhoenix::Effects::LitTexNormalFX->SetEyePosW(mCamera->mPosition);
//	DPhoenix::Effects::LitTexNormalFX->SetFogEnabled(mFogEnabled);
//	DPhoenix::Effects::LitTexNormalFX->SetFogColor(XMLoadFloat4(&mFogColor));
//	DPhoenix::Effects::LitTexNormalFX->SetFogStart(mFogStart);
//	DPhoenix::Effects::LitTexNormalFX->SetFogRange(mFogRange);
//
//	DPhoenix::Effects::LitTexNormalFX->SetOpacityValue(model->mOpacity);
//
//	DPhoenix::Effects::LitTexNormalFX->SetDirLights(mDirLightsArray);
//
//	DPhoenix::Effects::LitTexNormalFX->SetDirLightCount(mNumDirLights);
//
//	DPhoenix::Effects::LitTexNormalFX->SetPointLights(mPointLightsArray);
//	DPhoenix::Effects::LitTexNormalFX->SetPointLightCount(mNumPointLights);
//
//	//render using effect shader technique
//	D3DX11_TECHNIQUE_DESC techDesc;
//	tech->GetDesc(&techDesc);
//	for (UINT p = 0; p < techDesc.Passes; ++p)
//	{
//		//loop through subsets
//		for (UINT subset = 0; subset < model->Model->SubsetCount; ++subset)
//		{
//			//get pointer to the model / mesh data
//			DPhoenix::BasicModel* bmPtr = model->Model;
//			//get the material index for this subset
//			UINT materialIndex = bmPtr->Subsets[subset].materialIndex;
//			//set the material 
//			DPhoenix::Effects::LitTexNormalFX->SetMaterial(bmPtr->Mat[materialIndex]);
//			//if we have a diffuse map then set it for this subset
//			if (bmPtr->DiffuseMapSRV[materialIndex] != NULL)
//			{
//				DPhoenix::Effects::LitTexNormalFX->SetDiffuseMap(bmPtr->DiffuseMapSRV[materialIndex]);
//			}
//
//			if (bmPtr->NormalMapSRV[materialIndex] != NULL)
//			{
//				DPhoenix::Effects::LitTexNormalFX->SetNormalMap(bmPtr->NormalMapSRV[materialIndex]);
//			}
//			//call the draw method with the given subset
//			tech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
//			bmPtr->ModelMesh.Draw(md3dImmediateContext, subset);
//		}
//	}
//}
//
//void BTLTMovementDemo::RenderSkinnedModel(DPhoenix::SkinnedModelInstance * model,
//	ID3DX11EffectTechnique * tech)
//{
//
//	//create view / projection matrix
//	XMMATRIX view = mCamera->GetView();
//	XMMATRIX proj = mCamera->GetProj();
//
//	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
//
//	//concatenate world / view / projection matrices for rendering
//	XMMATRIX world = model->CalculateTransforms();
//
//	DPhoenix::Effects::LitTexFX->SetWorld(
//		world
//	);
//
//	XMMATRIX worldViewProj = XMMatrixMultiply(world, viewProj);
//	DPhoenix::Effects::LitTexFX->SetWorldViewProj(worldViewProj);
//
//	// Inverse-transpose is just applied to normals.  So zero out 
//	// translation row so that it doesn't get into our inverse-transpose
//	// calculation--we don't want the inverse-transpose of the translation.
//	XMMATRIX A = world;
//	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//
//	XMVECTOR det = XMMatrixDeterminant(A);
//
//	DPhoenix::Effects::LitTexFX->SetWorldInvTranspose(
//		XMMatrixTranspose(XMMatrixInverse(&det, A))
//	);
//
//	std::vector<XMFLOAT4X4> finalTransforms = model->GetFinalTransforms();
//	/*std::vector<XMFLOAT4X4> finalTransforms = model->GetBoneTransforms();*/
//	DPhoenix::Effects::LitTexFX->SetBoneTransforms(
//		&finalTransforms[0],
//		finalTransforms.size());
//
//	DPhoenix::Effects::LitTexFX->SetAmbLight(XMLoadFloat4(&mAmbientColor));
//	DPhoenix::Effects::LitTexFX->SetAmbIntensity(mAmbIntensity);
//
//	DPhoenix::Effects::LitTexFX->SetEyePosW(mCamera->mPosition);
//	DPhoenix::Effects::LitTexFX->SetFogEnabled(mFogEnabled);
//	DPhoenix::Effects::LitTexFX->SetFogColor(XMLoadFloat4(&mFogColor));
//	DPhoenix::Effects::LitTexFX->SetFogStart(mFogStart);
//	DPhoenix::Effects::LitTexFX->SetFogRange(mFogRange);
//
//	DPhoenix::Effects::LitTexFX->SetOpacityValue(model->mOpacity);
//
//	DPhoenix::Effects::LitTexFX->SetDirLights(mDirLightsArray);
//
//	DPhoenix::Effects::LitTexFX->SetDirLightCount(mNumDirLights);
//
//	DPhoenix::Effects::LitTexFX->SetPointLights(mPointLightsArray);
//	DPhoenix::Effects::LitTexFX->SetPointLightCount(mNumPointLights);
//
//	//render using effect shader technique
//	D3DX11_TECHNIQUE_DESC techDesc;
//	tech->GetDesc(&techDesc);
//	for (UINT p = 0; p < techDesc.Passes; ++p)
//	{
//		//loop through subsets
//		for (UINT subset = 0; subset < model->Model->SubsetCount; ++subset)
//		{
//			//get pointer to the model / mesh data
//			DPhoenix::SkinnedModel* bmPtr = model->Model;
//			//get the material index for this subset
//			UINT materialIndex = bmPtr->Subsets[subset].materialIndex;
//			//set the material 
//			DPhoenix::Effects::LitTexFX->SetMaterial(bmPtr->Mat[materialIndex]);
//			//if we have a diffuse map then set it for this subset
//			if (bmPtr->DiffuseMapSRV[materialIndex] != NULL)
//			{
//				DPhoenix::Effects::LitTexFX->SetDiffuseMap(bmPtr->DiffuseMapSRV[materialIndex]);
//			}
//			else
//			{
//				ID3D11ShaderResourceView* tempTex = mTexMgr.CreateTexture("Models\\wood.png");
//				DPhoenix::Effects::LitTexFX->SetDiffuseMap(tempTex);
//			}
//			//call the draw method with the given subset
//			tech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
//			bmPtr->ModelMesh.Draw(md3dImmediateContext, subset);
//		}
//	}
//}
//
//void BTLTMovementDemo::RenderSkinnedModelNormal(DPhoenix::SkinnedModelInstance * model,
//	ID3DX11EffectTechnique * tech)
//{
//
//	//create view / projection matrix
//	XMMATRIX view = mCamera->GetView();
//	XMMATRIX proj = mCamera->GetProj();
//
//	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
//
//	//concatenate world / view / projection matrices for rendering
//	XMMATRIX world = model->CalculateTransforms();
//
//	DPhoenix::Effects::LitTexNormalFX->SetWorld(
//		world
//	);
//
//	XMMATRIX worldViewProj = XMMatrixMultiply(world, viewProj);
//	DPhoenix::Effects::LitTexNormalFX->SetWorldViewProj(worldViewProj);
//
//	// Inverse-transpose is just applied to normals.  So zero out 
//	// translation row so that it doesn't get into our inverse-transpose
//	// calculation--we don't want the inverse-transpose of the translation.
//	XMMATRIX A = world;
//	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//
//	XMVECTOR det = XMMatrixDeterminant(A);
//
//	DPhoenix::Effects::LitTexNormalFX->SetWorldInvTranspose(
//		XMMatrixTranspose(XMMatrixInverse(&det, A))
//	);
//
//	std::vector<XMFLOAT4X4> finalTransforms = model->GetFinalTransforms();
//	/*std::vector<XMFLOAT4X4> finalTransforms = model->GetBoneTransforms();*/
//	DPhoenix::Effects::LitTexNormalFX->SetBoneTransforms(
//		&finalTransforms[0],
//		finalTransforms.size());
//
//	DPhoenix::Effects::LitTexNormalFX->SetAmbLight(XMLoadFloat4(&mAmbientColor));
//	DPhoenix::Effects::LitTexNormalFX->SetAmbIntensity(mAmbIntensity);
//
//	DPhoenix::Effects::LitTexNormalFX->SetEyePosW(mCamera->mPosition);
//	DPhoenix::Effects::LitTexNormalFX->SetFogEnabled(mFogEnabled);
//	DPhoenix::Effects::LitTexNormalFX->SetFogColor(XMLoadFloat4(&mFogColor));
//	DPhoenix::Effects::LitTexNormalFX->SetFogStart(mFogStart);
//	DPhoenix::Effects::LitTexNormalFX->SetFogRange(mFogRange);
//
//	DPhoenix::Effects::LitTexNormalFX->SetOpacityValue(model->mOpacity);
//
//
//	DPhoenix::Effects::LitTexNormalFX->SetDirLights(mDirLightsArray);
//
//	DPhoenix::Effects::LitTexNormalFX->SetDirLightCount(mNumDirLights);
//
//	DPhoenix::Effects::LitTexNormalFX->SetPointLights(mPointLightsArray);
//	DPhoenix::Effects::LitTexNormalFX->SetPointLightCount(mNumPointLights);
//
//	//render using effect shader technique
//	D3DX11_TECHNIQUE_DESC techDesc;
//	tech->GetDesc(&techDesc);
//	for (UINT p = 0; p < techDesc.Passes; ++p)
//	{
//		//loop through subsets
//		for (UINT subset = 0; subset < model->Model->SubsetCount; ++subset)
//		{
//			//get pointer to the model / mesh data
//			DPhoenix::SkinnedModel* bmPtr = model->Model;
//			//get the material index for this subset
//			UINT materialIndex = bmPtr->Subsets[subset].materialIndex;
//			//set the material 
//			DPhoenix::Effects::LitTexNormalFX->SetMaterial(bmPtr->Mat[materialIndex]);
//			//if we have a diffuse map then set it for this subset
//			if (bmPtr->DiffuseMapSRV[materialIndex] != NULL)
//			{
//				DPhoenix::Effects::LitTexNormalFX->SetDiffuseMap(bmPtr->DiffuseMapSRV[materialIndex]);
//			}
//			else
//			{
//				ID3D11ShaderResourceView* tempTex = mTexMgr.CreateTexture("Models\\wood.png");
//				DPhoenix::Effects::LitTexNormalFX->SetDiffuseMap(tempTex);
//			}
//
//			if (bmPtr->NormalMapSRV[materialIndex] != NULL)
//			{
//				DPhoenix::Effects::LitTexNormalFX->SetNormalMap(bmPtr->NormalMapSRV[materialIndex]);
//			}
//			else
//			{
//				ID3D11ShaderResourceView* tempTex = mTexMgr.CreateTexture("Models\\woodNM.png");
//				DPhoenix::Effects::LitTexNormalFX->SetNormalMap(tempTex);
//
//			}
//			//call the draw method with the given subset
//			tech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
//			bmPtr->ModelMesh.Draw(md3dImmediateContext, subset);
//		}
//	}
//}
//
