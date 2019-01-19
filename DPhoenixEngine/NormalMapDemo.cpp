//#include "Engine.h"
//
////initial class definition inheriting fron Engine
//class NormalMapDemo : public DPhoenix::Engine
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
//	//sprite position
//	XMFLOAT3 mPosition;
//	XMFLOAT3 mVelocity;
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
//	//vector for color shaded boxes
//	std::vector<DPhoenix::PrimitiveInstance*> mColorBoxes;
//
//	//camera object
//	DPhoenix::Camera* mCamera;
//	DPhoenix::EAngle mCamEAngle;
//
//	//ambient color
//	XMFLOAT4 mAmbientColor;
//	float mAmbIntensity;
//
//	//for FPS Scene -------------------------------------------
//	DPhoenix::PrimitiveInstance* mPlayer;	//player box for movement / collision
//	DPhoenix::PrimitiveInstance* mFloor;	//floor grid
//
//	void InitCollectables();				//initialise collectable objects
//	void InitScenery();						//initialise scenery objects
//
//											//vectors to hold scenery / collectables
//	std::vector<DPhoenix::PrimitiveInstance*> mCollectableBoxes;
//	std::vector<DPhoenix::PrimitiveInstance*> mSceneryBoxes;
//
//	//tracking int for collected objects
//	int mCollected;
//
//	//lighting ------------------------------------------------
//	//directional lights
//	std::vector<DirectionalLight*> mDirLights;
//	DirectionalLight mDirLightsArray[3];
//	int mNumDirLights;
//
//	//fog values
//	bool mFogEnabled;
//	float  mFogStart;
//	float  mFogRange;
//	XMFLOAT4 mFogColor;
//
//public:
//	NormalMapDemo(HINSTANCE hInstance);
//	~NormalMapDemo();
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
//	void NormalMapDemo::HandleEvents(DPhoenix::IEvent* e);
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
//	void NormalMapDemo::RenderBasicTexPrimitive(DPhoenix::PrimitiveInstance * model,
//		ID3DX11EffectTechnique * tech);
//
//	//new method / specifically handles textured primitives with directional lights
//	//and fog
//	void NormalMapDemo::RenderLitTexPrimitive(DPhoenix::PrimitiveInstance * model,
//		ID3DX11EffectTechnique * tech);
//
//	//new method / now with normal mapping!
//	void NormalMapDemo::RenderLitTexNormPrimitive(DPhoenix::PrimitiveInstance * model,
//		ID3DX11EffectTechnique * tech);
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
//	NormalMapDemo theApp(hInstance);
//
//	if (!theApp.Init(false))
//		return 0;
//
//	return theApp.Run();
//}
//
////constructor - don't really need to add much here
//NormalMapDemo::NormalMapDemo(HINSTANCE hInstance)
//	: Engine(hInstance)
//{
//	mMainWndCaption = "Normal Map Demo";
//	//create camera here to prevent
//	//issues with matrix generation
//	mCamera = new DPhoenix::Camera();
//}
//
////destructor (release any memory as necessary)
//NormalMapDemo::~NormalMapDemo()
//{
//	DPhoenix::Effects::DestroyAll();
//	DPhoenix::InputLayouts::DestroyAll();
//}
//
////initilaise objects that can be collected to test collisions
////(in practice, you'd often read from data and use a specific class)
//void NormalMapDemo::InitCollectables()
//{
//	//material for collectables
//	Material* materialShiny = new Material();
//	materialShiny->Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
//	materialShiny->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	materialShiny->Specular = XMFLOAT4(0.25f, 0.25f, 1.0f, 16.0f);
//	materialShiny->Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
//
//	//for each collectable box, we push back a new PrimitiveInstance
//	//then we load a basic textured instance and update various properties
//	mCollectableBoxes.push_back(new DPhoenix::PrimitiveInstance());
//	mCollectableBoxes.back()->LoadLitTexInstance(md3dDevice, "Models\\bluetexture.jpg", mBox, &mTexMgr);
//	mCollectableBoxes.back()->mMaterial = materialShiny;
//	mCollectableBoxes.back()->mNormalMap = mTexMgr.CreateTexture("Models\\bluetextureNM.jpg");
//	mCollectableBoxes.back()->mScale = XMFLOAT3(0.5f, 0.5f, 0.5f);				//half the stanadrd box size
//	mCollectableBoxes.back()->mHalfSizes = XMFLOAT3(2.5f, 2.5f, 2.5f);			//5*5*5 size
//	mCollectableBoxes.back()->mPosition = XMFLOAT3(100.0f, 5.0f, 0.0f);			//up 5 on y so in air
//	mCollectableBoxes.back()->mPrevPosition = XMFLOAT3(100.0f, 5.0f, 0.0f);		//set previous as current to init
//	mCollectableBoxes.back()->mCollidable = true;								//set as collidable
//	mCollectableBoxes.back()->mEntityType = DPhoenix::ENTITY_COLLECTABLE_MESH;	//set entity type
//	AddEntity(mCollectableBoxes.back());										//add to engine
//
//																				//And more boxes - you can experiment with textures and positions
//	mCollectableBoxes.push_back(new DPhoenix::PrimitiveInstance());
//	mCollectableBoxes.back()->LoadLitTexInstance(md3dDevice, "Models\\bluetexture.jpg", mBox, &mTexMgr);
//	mCollectableBoxes.back()->mMaterial = materialShiny;
//	mCollectableBoxes.back()->mNormalMap = mTexMgr.CreateTexture("Models\\bluetextureNM.jpg");
//	mCollectableBoxes.back()->mScale = XMFLOAT3(0.5f, 0.5f, 0.5f);
//	mCollectableBoxes.back()->mHalfSizes = XMFLOAT3(2.5f, 2.5f, 2.5f);
//	mCollectableBoxes.back()->mPosition = XMFLOAT3(0.0f, 5.0f, 250.0f);
//	mCollectableBoxes.back()->mPrevPosition = XMFLOAT3(0.0f, 5.0f, 250.0f);
//	mCollectableBoxes.back()->mCollidable = true;
//	mCollectableBoxes.back()->mEntityType = DPhoenix::ENTITY_COLLECTABLE_MESH;
//	AddEntity(mCollectableBoxes.back());
//
//	mCollectableBoxes.push_back(new DPhoenix::PrimitiveInstance());
//	mCollectableBoxes.back()->LoadLitTexInstance(md3dDevice, "Models\\bluetexture.jpg", mBox, &mTexMgr);
//	mCollectableBoxes.back()->mMaterial = materialShiny;
//	mCollectableBoxes.back()->mNormalMap = mTexMgr.CreateTexture("Models\\bluetextureNM.jpg");
//	mCollectableBoxes.back()->mScale = XMFLOAT3(0.5f, 0.5f, 0.5f);
//	mCollectableBoxes.back()->mHalfSizes = XMFLOAT3(2.5f, 2.5f, 2.5f);
//	mCollectableBoxes.back()->mPosition = XMFLOAT3(150.0f, 5.0f, -100.0f);
//	mCollectableBoxes.back()->mPrevPosition = XMFLOAT3(150.0f, 5.0f, -100.0f);
//	mCollectableBoxes.back()->mCollidable = true;
//	mCollectableBoxes.back()->mEntityType = DPhoenix::ENTITY_COLLECTABLE_MESH;
//	AddEntity(mCollectableBoxes.back());
//
//	mCollectableBoxes.push_back(new DPhoenix::PrimitiveInstance());
//	mCollectableBoxes.back()->LoadLitTexInstance(md3dDevice, "Models\\bluetexture.jpg", mBox, &mTexMgr);
//	mCollectableBoxes.back()->mMaterial = materialShiny;
//	mCollectableBoxes.back()->mNormalMap = mTexMgr.CreateTexture("Models\\bluetextureNM.jpg");
//	mCollectableBoxes.back()->mScale = XMFLOAT3(0.5f, 0.5f, 0.5f);
//	mCollectableBoxes.back()->mHalfSizes = XMFLOAT3(2.5f, 2.5f, 2.5f);
//	mCollectableBoxes.back()->mPosition = XMFLOAT3(-200.0f, 5.0f, -200.0f);
//	mCollectableBoxes.back()->mPrevPosition = XMFLOAT3(-200.0f, 5.0f, -200.0f);
//	mCollectableBoxes.back()->mCollidable = true;
//	mCollectableBoxes.back()->mEntityType = DPhoenix::ENTITY_COLLECTABLE_MESH;
//	AddEntity(mCollectableBoxes.back());
//
//}
//
////initilaise objects that can be bumped into to test collisions
////(in practice, you'd often read from data and use a specific class)
//void NormalMapDemo::InitScenery()
//{
//	Material* materialStandard = new Material();
//
//	materialStandard->Ambient = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
//	materialStandard->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	materialStandard->Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
//	materialStandard->Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
//
//	//for each scenery box, we push back a new PrimitiveInstance
//	//then we load a basic textured instance and update various properties
//	mSceneryBoxes.push_back(new DPhoenix::PrimitiveInstance());
//	mSceneryBoxes.back()->LoadLitTexInstance(md3dDevice, "Models\\metal.png", mBox, &mTexMgr);
//	mSceneryBoxes.back()->mMaterial = materialStandard;
//	mSceneryBoxes.back()->mNormalMap = mTexMgr.CreateTexture("Models\\metalNM.png");
//	mSceneryBoxes.back()->mScale = XMFLOAT3(2.0f, 2.0f, 2.0f);			//double size
//	mSceneryBoxes.back()->mHalfSizes = XMFLOAT3(10.0f, 10.0f, 10.0f);	//20*20*20 collision box
//	mSceneryBoxes.back()->mPosition = XMFLOAT3(50.0f, 10.0f, 0.0f);		//10 on y so bottom touches floor
//	mSceneryBoxes.back()->mPrevPosition = XMFLOAT3(50.0f, 10.0f, 0.0f);	//init prev position
//	mSceneryBoxes.back()->mCollidable = true;							//set as collidable
//	mSceneryBoxes.back()->mEntityType = DPhoenix::ENTITY_SCENERY_MESH;	//set as scnery mesh for collision
//	AddEntity(mSceneryBoxes.back());									//add to engine
//
//	//And more boxes - you can experiment with textures and positions
//	mSceneryBoxes.push_back(new DPhoenix::PrimitiveInstance());
//	mSceneryBoxes.back()->LoadLitTexInstance(md3dDevice, "Models\\metal.png", mBox, &mTexMgr);
//	mSceneryBoxes.back()->mMaterial = materialStandard;
//	mSceneryBoxes.back()->mNormalMap = mTexMgr.CreateTexture("Models\\metalNM.png");
//	mSceneryBoxes.back()->mScale = XMFLOAT3(2.0f, 2.0f, 2.0f);
//	mSceneryBoxes.back()->mHalfSizes = XMFLOAT3(10.0f, 10.0f, 10.0f);
//	mSceneryBoxes.back()->mPosition = XMFLOAT3(0.0f, 10.0f, 100.0f);
//	mSceneryBoxes.back()->mPrevPosition = XMFLOAT3(0.0f, 10.0f, 100.0f);
//	mSceneryBoxes.back()->mCollidable = true;
//	mSceneryBoxes.back()->mEntityType = DPhoenix::ENTITY_SCENERY_MESH;
//	AddEntity(mSceneryBoxes.back());
//
//	mSceneryBoxes.push_back(new DPhoenix::PrimitiveInstance());
//	mSceneryBoxes.back()->LoadLitTexInstance(md3dDevice, "Models\\metal.png", mBox, &mTexMgr);
//	mSceneryBoxes.back()->mMaterial = materialStandard;
//	mSceneryBoxes.back()->mNormalMap = mTexMgr.CreateTexture("Models\\metalNM.png");
//	mSceneryBoxes.back()->mScale = XMFLOAT3(2.0f, 2.0f, 2.0f);
//	mSceneryBoxes.back()->mHalfSizes = XMFLOAT3(10.0f, 10.0f, 10.0f);
//	mSceneryBoxes.back()->mPosition = XMFLOAT3(100.0f, 10.0f, -75.0f);
//	mSceneryBoxes.back()->mPrevPosition = XMFLOAT3(100.0f, 10.0f, -75.0f);
//	mSceneryBoxes.back()->mCollidable = true;
//	mSceneryBoxes.back()->mEntityType = DPhoenix::ENTITY_SCENERY_MESH;
//	AddEntity(mSceneryBoxes.back());
//
//	mSceneryBoxes.push_back(new DPhoenix::PrimitiveInstance());
//	mSceneryBoxes.back()->LoadLitTexInstance(md3dDevice, "Models\\metal.png", mBox, &mTexMgr);
//	mSceneryBoxes.back()->mMaterial = materialStandard;
//	mSceneryBoxes.back()->mNormalMap = mTexMgr.CreateTexture("Models\\metalNM.png");
//	mSceneryBoxes.back()->mScale = XMFLOAT3(2.0f, 2.0f, 2.0f);
//	mSceneryBoxes.back()->mHalfSizes = XMFLOAT3(10.0f, 10.0f, 10.0f);
//	mSceneryBoxes.back()->mPosition = XMFLOAT3(-100.0f, 10.0f, -100.0f);
//	mSceneryBoxes.back()->mPrevPosition = XMFLOAT3(-100.0f, 10.0f, -100.0f);
//	mSceneryBoxes.back()->mCollidable = true;
//	mSceneryBoxes.back()->mEntityType = DPhoenix::ENTITY_SCENERY_MESH;
//	AddEntity(mSceneryBoxes.back());
//}
//
////any additional initialisation (we generally initiaise game assets here)
//bool NormalMapDemo::Init(bool fullScreen)
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
//	//right box
//	mColorBoxes.push_back(new DPhoenix::PrimitiveInstance());
//	mColorBoxes.back()->LoadLitTexInstance(md3dDevice, "Textures\\Boxes\\dpnew.jpg",
//		mBox, &mTexMgr);
//	mColorBoxes.back()->mMaterial = materialStandard;
//	mColorBoxes.back()->mPosition.x = 50.0f;
//	mColorBoxes.back()->mNormalMap = mTexMgr.CreateTexture("Models\\dpnewNM.jpg");
//	//left box
//	mColorBoxes.push_back(new DPhoenix::PrimitiveInstance());
//	mColorBoxes.back()->LoadLitTexInstance(md3dDevice, "Textures\\Boxes\\dpnew.jpg",
//		mBox, &mTexMgr);
//	mColorBoxes.back()->mMaterial = materialShiny;
//	mColorBoxes.back()->mPosition.x = -50.0f;
//	mColorBoxes.back()->mPosition.y = 50.0f;
//	mColorBoxes.back()->mNormalMap = mTexMgr.CreateTexture("Models\\dpnewNM.jpg");
//	//top box
//	mColorBoxes.push_back(new DPhoenix::PrimitiveInstance());
//	mColorBoxes.back()->LoadLitTexInstance(md3dDevice, "Textures\\Boxes\\dpnew.jpg",
//		mBox, &mTexMgr);
//	mColorBoxes.back()->mMaterial = materialStandard;
//	mColorBoxes.back()->mPosition.y = 150.0f;
//	mColorBoxes.back()->mNormalMap = mTexMgr.CreateTexture("Models\\dpnewNM.jpg");
//	//bottom box
//	mColorBoxes.push_back(new DPhoenix::PrimitiveInstance());
//	mColorBoxes.back()->LoadLitTexInstance(md3dDevice, "Textures\\Boxes\\dpnew.jpg",
//		mBox, &mTexMgr);
//	mColorBoxes.back()->mMaterial = materialShiny;
//	mColorBoxes.back()->mPosition.y = 0.0f;
//	mColorBoxes.back()->mNormalMap = mTexMgr.CreateTexture("Models\\dpnewNM.jpg");
//	//front box
//	mColorBoxes.push_back(new DPhoenix::PrimitiveInstance());
//	mColorBoxes.back()->LoadLitTexInstance(md3dDevice, "Textures\\Boxes\\dpnew.jpg",
//		mBox, &mTexMgr);
//	mColorBoxes.back()->mMaterial = materialStandard;
//	mColorBoxes.back()->mPosition.z = 50.0f;
//	mColorBoxes.back()->mPosition.y = 50.0f;
//	mColorBoxes.back()->mNormalMap = mTexMgr.CreateTexture("Models\\dpnewNM.jpg");
//	//bottom box
//	mColorBoxes.push_back(new DPhoenix::PrimitiveInstance());
//	mColorBoxes.back()->LoadLitTexInstance(md3dDevice, "Textures\\Boxes\\dpnew.jpg",
//		mBox, &mTexMgr);
//	mColorBoxes.back()->mMaterial = materialShiny;
//	mColorBoxes.back()->mPosition.z = -50.0f;
//	mColorBoxes.back()->mPosition.y = 50.0f;
//	mColorBoxes.back()->mNormalMap = mTexMgr.CreateTexture("Models\\dpnewNM.jpg");
//	//ambient lighting values
//	mAmbientColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	mAmbIntensity = 1.0f;
//
//#pragma region SetupFPSScene
//
//	//create new primitive instance to represent player (ot to be rendered, but for collision etc.)
//	mPlayer = new DPhoenix::PrimitiveInstance();
//	mPlayer->LoadColorInstance(md3dDevice, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), mBox);
//	mPlayer->mEntityType = DPhoenix::ENTITY_PLAYER_MESH;	//set as player type
//	mPlayer->mHalfSizes = XMFLOAT3(5.0f, 5.0f, 5.0f);		//10*10*10 collision box
//	mPlayer->mCollidable = true;							//set colidable
//	mPlayer->mPosition.y = 5.0f;							//init position
//	mCamera->mPosition.y = 5.0f;							//init position
//	AddEntity(mPlayer);										//add to engine
//
//															//create grid using geometry generator
//	mGrid = new DPhoenix::GeometryGenerator::MeshData();
//	mGeoGen->CreateGrid(500.0f, 500.0f, 10, 10, *mGrid);
//
//	Material* materialFloor = new Material();
//
//	materialFloor->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	materialFloor->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	materialFloor->Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f);
//	materialFloor->Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
//
//	//create floor primitive instance using grid
//	mFloor = new  DPhoenix::PrimitiveInstance();
//	mFloor->LoadLitTexInstance(md3dDevice, "Models\\wood.png", mGrid, &mTexMgr);
//	mFloor->mMaterial = materialFloor;
//	mFloor->mNormalMap = mTexMgr.CreateTexture("Models\\woodNM.png");
//
//	//init scenery and collectables
//	InitScenery();
//	InitCollectables();
//	mCollected = 0;
//
//#pragma endregion SetupFPSScene
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
//	//pointing down-right - light red (ish)
//	mDirLights.push_back(new DirectionalLight());
//	mDirLights.back()->Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
//	mDirLights.back()->Diffuse = XMFLOAT4(1.0f, 0.4f, 0.4f, 1.0f);
//	mDirLights.back()->Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 0.0f);
//	mDirLights.back()->Direction = XMFLOAT3(0.0f, -0.707f, 0.707f);
//
//	//pointing forward-down - light blue (ish)
//	mDirLights.push_back(new DirectionalLight());
//	mDirLights.back()->Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
//	mDirLights.back()->Diffuse = XMFLOAT4(0.2f, 0.2f, 0.8f, 1.0f);
//	mDirLights.back()->Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 0.0f);
//	mDirLights.back()->Direction = XMFLOAT3(0.707f, -0.707f, 0.0f);
//
//	mNumDirLights = mDirLights.size();
//
//	//fog values
//	mFogEnabled = false;
//	mFogStart = 100.0f;
//	mFogRange = 1000.0f;
//	mFogColor = XMFLOAT4(0.25f, 0.0f, 0.0f, 1.0f);
//
//#pragma endregion SetupLights
//
//	//set initial position and velocity
//	mPosition.x = 5.0f; mPosition.y = 5.0f; mPosition.z = 0.0f;
//	mVelocity.x = 0.0f; mVelocity.y = 0.0f; mVelocity.z = 0.0f;
//
//	//camera and EAngle initialisation (cam in constructor due to use in OnResize)
//	mCamEAngle.r = 0.0f; mCamEAngle.p = 0.0f; mCamEAngle.y = 0.0f;
//
//	//debug font
//	//(Arial size 20 sending window, initial brush, writefactory)
//	mDebugFont = new DPhoenix::Font(mhMainWnd, mBlackBrush, mDWriteFactory, "Arial", 20.0f);
//
//	return true;
//}
//
////what to do on resize - mainly we'lll just call the parent / engine's method
//void NormalMapDemo::OnResize()
//{
//	Engine::OnResize();
//
//	//the window resized so the aspect ratio needs
//	//to be calculated again, and the projection matrix
//	mCamera->SetPerspective(0.25f * XM_PI, AspectRatio(), 1.0f, 100000.0f);
//}
//
////update scene - here is where our update logic for the game loop goes
//void NormalMapDemo::UpdateScene(float dt)
//{
//	//calculate scaling based on window dimensions
//	mScreenScale.x = (float)mClientWidth / 1440;
//	mScreenScale.y = (float)mClientHeight / 900;
//
//	//camera updating - get facing direction but no y component (horz)
//	XMFLOAT3 facingDir = mCamEAngle.ToFloat3();
//	facingDir.y = 0.0f;
//	XMVECTOR facingVec = XMLoadFloat3(&facingDir);
//
//	//normalise the vector (for direction) and store as XMFLOAT3
//	facingVec = XMVector3Normalize(facingVec);
//	XMStoreFloat3(&facingDir, facingVec);
//	//set the forward vector for the player and update velocity
//	//relative movement based on facing direction
//	mPlayer->mForwardVector = facingDir;
//	mPlayer->mRelativeVelocity = mVelocity;
//	mPlayer->Update(dt, true);
//	//update camera position to match player
//	mCamera->mPosition = mPlayer->mPosition;
//	mCamera->Update(mCamEAngle.ToFloat3());
//
//	//loop through colectable boxes
//	for (int i = 0; i < mCollectableBoxes.size(); )
//	{
//		//if the box has collided add to collected
//		//set alive to false for engine updating and remove from vector
//		if (mCollectableBoxes[i]->mCollided)
//		{
//			mCollected++;
//			mCollectableBoxes[i]->mAlive = false;
//			mCollectableBoxes.erase(mCollectableBoxes.begin() + i);
//		}
//		else
//		{
//			i++;
//		}
//	}
//
//	//rotate the boxes
//	for (int i = 0; i < mColorBoxes.size(); i++)
//	{
//		mColorBoxes[i]->mRotation.x += 1.0f * dt;
//		mColorBoxes[i]->mRotation.y += 1.0f * dt;
//		mColorBoxes[i]->mRotation.z += 1.0f * dt;
//
//	}
//
//	//reset velocity after each update as determined by input
//	mVelocity.x = 0.0f; mVelocity.y = 0.0f; mVelocity.z = 0.0f;
//}
//
//void NormalMapDemo::BuildGeometryBuffers()
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
//void NormalMapDemo::DrawScene()
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
//#pragma region RenderColorPrimitives
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::PosColor);
//
//	//here we will render the box using our new method
//	//we send the pointer to the instance and also the color rendering technique
//	//(using Color.fx)
//	//for (int i = 0; i < mColorBoxes.size(); i++)
//	//{
//	//	RenderColorPrimitive(mColorBoxes[i], activeColorTech);
//	//}
//
//#pragma endregion RenderColorPrimitives
//
//#pragma region RenderBasicTexturedPrimitives
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::PosTex);
//
//	////here we will render the box using our new method
//	////we send the pointer to the instance and also the texture rendering technique
//	////(using Texture.fx)
//	//for (int i = 0; i < mColorBoxes.size(); i++)
//	//{
//	//	RenderBasicTexPrimitive(mColorBoxes[i], activeBasicTexTech);
//	//}
//	////render floor
//	//RenderBasicTexPrimitive(mFloor, activeBasicTexTech);
//	////render scenery
//	//for (int i = 0; i < mSceneryBoxes.size(); i++)
//	//{
//	//	RenderBasicTexPrimitive(mSceneryBoxes[i], activeBasicTexTech);
//	//}
//	////render collectables
//	//for (int i = 0; i < mCollectableBoxes.size(); i++)
//	//{
//	//	RenderBasicTexPrimitive(mCollectableBoxes[i], activeBasicTexTech);
//	//}
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
//#pragma endregion SetUplights
//
//#pragma region RenderLitTexturedPrimitives
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::PosNormalTexTan);
//
//	//here we will render the box using our new method
//	//we send the pointer to the instance and also the texture rendering technique
//	//(using Texture.fx)
//	//for (int i = 0; i < mColorBoxes.size(); i++)
//	//{
//	//	RenderLitTexPrimitive(mColorBoxes[i], activeLitTexTech);
//	//}
//	////render floor
//	//RenderLitTexPrimitive(mFloor, activeLitTexTech);
//	////render scenery
//	//for (int i = 0; i < mSceneryBoxes.size(); i++)
//	//{
//	//	RenderLitTexPrimitive(mSceneryBoxes[i], activeLitTexTech);
//	//}
//	////render collectables
//	//for (int i = 0; i < mCollectableBoxes.size(); i++)
//	//{
//	//	RenderLitTexPrimitive(mCollectableBoxes[i], activeLitTexTech);
//	//}
//
//#pragma endregion RenderLitTexturedPrimitives
//
//#pragma region RenderLitTexturedNormalPrimitives
//
//	//set up the vertex input layout
//	//(would need to change for different types of rendering)
//	md3dImmediateContext->IASetInputLayout(DPhoenix::InputLayouts::PosNormalTexTan);
//
//	//here we will render the box using our new method
//	//we send the pointer to the instance and also the texture rendering technique
//	//(using Texture.fx)
//	for (int i = 0; i < mColorBoxes.size(); i++)
//	{
//		RenderLitTexNormPrimitive(mColorBoxes[i], activeLitTexNormalTech);
//	}
//	//render floor
//	RenderLitTexNormPrimitive(mFloor, activeLitTexNormalTech);
//	//render scenery
//	for (int i = 0; i < mSceneryBoxes.size(); i++)
//	{
//		RenderLitTexNormPrimitive(mSceneryBoxes[i], activeLitTexNormalTech);
//	}
//	//render collectables
//	for (int i = 0; i < mCollectableBoxes.size(); i++)
//	{
//		RenderLitTexNormPrimitive(mCollectableBoxes[i], activeLitTexNormalTech);
//	}
//
//#pragma endregion RenderLitTexturedNormalPrimitives
//
//#pragma region RenderSprites
//
//	//to render sprites we need to disable depth testing (so it layers on top of each other)
//	//like a collage
//	md3dImmediateContext->OMSetDepthStencilState(mDepthDisabledStencilState, 1);
//
//	//turn depth stencil back on for 3D rendering
//	md3dImmediateContext->OMSetDepthStencilState(mDepthStencilState, 1);
//
//#pragma endregion RenderSprites
//
//#pragma region RenderFonts
//
//	//string stream object for getting all text to display
//	std::ostringstream debugText;
//
//	debugText << "Debug text:" << std::endl;
//
//	debugText << "Collected:" << mCollected << std::endl;
//	debugText << "Camera Pos: x: " << mCamera->mPosition.x <<
//		" | y: " << mCamera->mPosition.y <<
//		" | z: " << mCamera->mPosition.z << std::endl;
//	debugText << "Camera LookAt: x: " << mCamera->mTarget.x <<
//		" | y: " << mCamera->mTarget.y <<
//		" | z: " << mCamera->mTarget.z << std::endl;
//
//	//calculate screen scaling
//	float scaleX = (float)mClientWidth / 1440;
//	float scaleY = (float)mClientHeight / 900;
//
//	//call draw font method sending all key info
//	mDebugFont->DrawFont(m2D1RT, debugText.str(), 10.0f, 500.0f,
//		1.0f, 1.0f, D2D1::ColorF(0xFFFFFF, 0.5f));
//
//#pragma endregion RenderFonts
//
//	//present to screen
//	HR(mSwapChain->Present(0, 0));
//
//}
//
//void NormalMapDemo::HandleEvents(DPhoenix::IEvent* e)
//{
//	switch (e->GetID())
//	{
//	case DPhoenix::EVENT_KEYPRESS:
//	{
//		DPhoenix::KeyPressEvent* kpe = (DPhoenix::KeyPressEvent*)e;
//		switch (kpe->mKeycode)
//		{
//		case DIK_D:
//			mVelocity.x = 100.0f;
//			break;
//		case DIK_A:
//			mVelocity.x = -100.0f;
//			break;
//		case DIK_W:
//			mVelocity.z = 100.0f;
//			break;
//		case DIK_S:
//			mVelocity.z = -100.0f;
//			break;
//		case DIK_Q:
//			mVelocity.y = -100.0f;
//			break;
//		case DIK_E:
//			mVelocity.y = 100.0f;
//			break;
//		case DIK_0:
//			mNumDirLights = 0;
//			break;
//		case DIK_1:
//			mNumDirLights = 1;
//			break;
//		case DIK_2:
//			mNumDirLights = 2;
//			break;
//		case DIK_3:
//			mNumDirLights = 3;
//			break;
//		case DIK_ESCAPE:
//			//shutdown here
//			break;
//		}
//	}
//	break;
//	case DPhoenix::EVENT_KEYRELEASE:
//	{
//		DPhoenix::KeyReleaseEvent* kre = (DPhoenix::KeyReleaseEvent*)e;
//		switch (kre->mKeycode)
//		{
//		case DIK_F:
//			mFogEnabled ? mFogEnabled = false : mFogEnabled = true;
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
//										//set euler angles for camera direction
//		mCamEAngle.y -= movex*0.0075;
//		mCamEAngle.p -= movey*0.0075;
//
//		//set restrictions on target movment
//		//(directly up / down)
//		if (mCamEAngle.p >= 1.565)
//			mCamEAngle.p = 1.565;
//		if (mCamEAngle.p <= -1.565)
//			mCamEAngle.p = -1.565;
//
//		mCamEAngle.Normalize();
//	}
//	break;
//	case DPhoenix::EVENT_XBOX_INPUT:
//	{
//		DPhoenix::XBoxEvent* xbe = (DPhoenix::XBoxEvent*)e;
//		if (xbe->mPadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
//		{
//			mVelocity.x = -500.0f;
//		}
//		if (xbe->mPadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
//		{
//			mVelocity.x = 500.0f;
//		}
//		if (xbe->mPadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
//		{
//			mVelocity.y = 500.0f;
//		}
//		if (xbe->mPadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
//		{
//			mVelocity.y = -500.0f;
//		}
//		//audio buttons
//		if (xbe->mPadState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
//		{
//			if (!mAudioMgr.GetSound("IgnoranceTitle")->IsPlaying())
//			{
//				mAudioMgr.GetSound("IgnoranceTitle")->Play(true);
//			}
//
//		}
//		if (xbe->mPadState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
//		{
//			if (!mAudioMgr.GetSound("LaserShootFX")->IsPlaying())
//			{
//				mAudioMgr.GetSound("LaserShootFX")->Play(false);
//			}
//			else
//			{
//				mAudioMgr.GetSound("LaserShootFX")->Stop();
//			}
//		}
//		if (xbe->mPadState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
//		{
//			if (!mAudioMgr.GetSound("FlatBelieveClip")->IsPlaying())
//			{
//				mAudioMgr.GetSound("FlatBelieveClip")->Play(false);
//			}
//			else
//			{
//				mAudioMgr.GetSound("FlatBelieveClip")->Stop();
//			}
//		}
//		if (xbe->mPadState.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
//		{
//			mAudioMgr.ResetAllSounds();
//		}
//	}
//	break;
//	}
//}
//
//void NormalMapDemo::InitAudio()
//{
//	mAudioMgr.CreateSound("IgnoranceTitle", "Audio\\Music\\IgnoranceTitle.wav");
//	mAudioMgr.CreateSound("LaserShootFX", "Audio\\FX\\LaserShootFX.wav");
//	mAudioMgr.CreateSound("FlatBelieveClip", "Audio\\Voice\\FlatBelieveClip.wav");
//
//}
//
//void NormalMapDemo::RenderSprite(DPhoenix::Sprite * sprite, ID3DX11EffectTechnique * tech, bool isHUD)
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
//void NormalMapDemo::RenderColorPrimitive(DPhoenix::PrimitiveInstance * model,
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
//void NormalMapDemo::RenderBasicTexPrimitive(DPhoenix::PrimitiveInstance * model,
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
//void NormalMapDemo::RenderLitTexPrimitive(DPhoenix::PrimitiveInstance * model,
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
//	DPhoenix::Effects::LitTexFX->SetDirLights(mDirLightsArray);
//	DPhoenix::Effects::LitTexFX->SetMaterial(*model->mMaterial);
//
//	DPhoenix::Effects::LitTexFX->SetDirLightCount(mNumDirLights);
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
//void NormalMapDemo::RenderLitTexNormPrimitive(DPhoenix::PrimitiveInstance * model,
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
//	DPhoenix::Effects::LitTexNormalFX->SetDirLights(mDirLightsArray);
//	DPhoenix::Effects::LitTexNormalFX->SetMaterial(*model->mMaterial);
//
//	DPhoenix::Effects::LitTexNormalFX->SetDirLightCount(mNumDirLights);
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
