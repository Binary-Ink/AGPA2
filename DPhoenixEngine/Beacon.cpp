#include "Beacon.h"

DPhoenix::Beacon::Beacon(XMFLOAT3 position, TextureMgr * texMgr, ID3D11Device * d3dDevice, 
	GeometryGenerator::MeshData * sphere, GeometryGenerator::MeshData * pole)
{
	mTexMgr = texMgr;
	md3dDevice = d3dDevice;

	mBeaconState = BEACON_UNLIT;
	mIsGrowing = true;
	mRadianceRange = 80.0f;

	Material* materialShiny = new Material();
	materialShiny->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	materialShiny->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	materialShiny->Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f);
	materialShiny->Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);


	mPoleUnlitDiffuseMap = texMgr->CreateTexture("Textures\\Beacons\\BeaconPoleUnlit_cm.png");
	mPolePlayerLitDiffuseMap = texMgr->CreateTexture("Textures\\Beacons\\BeaconPolePlayerLit_cm.png");
	mPoleEnemyLitDiffuseMap = texMgr->CreateTexture("Textures\\Beacons\\BeaconPoleEnemyLit_cm.png");

	mCurrentPoleNormalMap = texMgr->CreateTexture("Textures\\Beacons\\BeaconPole_nm.png");

	//Beacon ---------------------------------------------
	mBeaconUnlitDiffuseMap = texMgr->CreateTexture("Textures\\Beacons\\BeaconLightUnlit_cm.png");
	mBeaconPlayerLitDiffuseMap = texMgr->CreateTexture("Textures\\Beacons\\BeaconLightPlayerLit_cm.png");
	mBeaconEnemyLitDiffuseMap = texMgr->CreateTexture("Textures\\Beacons\\BeaconLightEnemyLit_cm.png");

	mCurrentBeaconNormalMap = texMgr->CreateTexture("Textures\\Beacons\\BeaconLight_nm.png");
	
	mPoleMeshInstance = new PrimitiveInstance();
	mPoleMeshInstance->LoadLitTexInstance(md3dDevice, "Textures\\Beacons\\BeaconPoleUnlit_cm.png", pole, mTexMgr);
	mPoleMeshInstance->mMaterial = materialShiny;
	mPoleMeshInstance->mNormalMap = mTexMgr->CreateTexture("Textures\\Beacons\\BeaconPole_nm.png");
	mPoleMeshInstance->mPosition = position; mPoleMeshInstance->mPrevPosition = position;
	mPoleMeshInstance->mHalfSizes = XMFLOAT3(5.0f, 20.0f, 5.0f);
	mPoleMeshInstance->mEntityType = ENTITY_BEACON_MESH;
	mPoleMeshInstance->mCollidable = true;

	mBeaconMeshInstance = new PrimitiveInstance();
	mBeaconMeshInstance->LoadLitTexInstance(md3dDevice, "Textures\\Beacons\\BeaconLightUnlit_cm.png", sphere, mTexMgr);
	mBeaconMeshInstance->mMaterial = materialShiny;
	mBeaconMeshInstance->mNormalMap = mTexMgr->CreateTexture("Textures\\Beacons\\BeaconLight_nm.png");
	mBeaconMeshInstance->mPosition = position; mBeaconMeshInstance->mPrevPosition = position;
	mBeaconMeshInstance->mPosition = position;
	mBeaconMeshInstance->mPosition.y = 60.0f;
}

DPhoenix::Beacon::~Beacon()
{
}

void DPhoenix::Beacon::Update(float dt)
{
	if (mBeaconState != BEACON_UNLIT)
	{		
		if (mIsGrowing)
		{
			mRadianceRange += 20.0f * dt;

			if (mRadianceRange >= 150.0f)
			{
				mRadianceRange = 150.0f;
				mIsGrowing = false;
			}
		}
		else
		{
			mRadianceRange -= 20.0f * dt;

			if (mRadianceRange <= 20.0f)
			{
				mRadianceRange = 20.0f;
				mIsGrowing = true;
			}
		}

		mRadiance->Range = mRadianceRange;
	}
}

void DPhoenix::Beacon::LightBeacon(bool isPlayer)
{

	mRadiance = new PointLight();

	//specular and attenuation constant
	mRadiance->Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	
	mRadiance->Att = XMFLOAT3(1.0f, 0.1f, 0.02f);
	//could set button to update range
	mRadiance->Range = mRadianceRange;
	mRadiance->Position = mBeaconMeshInstance->mPosition;

	if (isPlayer)
	{
		mRadiance->Diffuse = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f);
		mRadiance->Specular = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f);
		//mRadiance->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mPoleMeshInstance->mDiffuseMap = mPolePlayerLitDiffuseMap;
		mBeaconMeshInstance->mDiffuseMap = mBeaconPlayerLitDiffuseMap;
		mBeaconState = BEACON_PLAYER_LIT;
	}
	else
	{
		mRadiance->Diffuse = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
		mRadiance->Specular = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
		//mRadiance->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mPoleMeshInstance->mDiffuseMap = mPoleEnemyLitDiffuseMap;
		mBeaconMeshInstance->mDiffuseMap = mBeaconEnemyLitDiffuseMap;
		mBeaconState = BEACON_ENEMY_LIT;
	}

}


