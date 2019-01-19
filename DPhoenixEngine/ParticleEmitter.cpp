#include "ParticleEmitter.h"

DPhoenix::ParticleEmitter::ParticleEmitter()
{
	//Billboard Sprite as 'template'
	mSprite = NULL;

	//position vars
	mPosition.x = 0.0f; mPosition.y = 0.0f; mPosition.z = 0.0f;
	mDirectionAngleH = 0.0f;
	mDirectionAngleV = 0.0f;
	mLength = 50.0f;

	//colour / amount / alpha variation
	mMax = 100;
	mMinAlpha = 255; mMaxAlpha = 255;
	mMinR = 255; mMinG = 255; mMinB = 255;
	mMaxR = 255; mMaxG = 255; mMaxB = 255;

	//spread vars
	mSpreadH = 360; mSpreadV = 360;
	mVelocityH = 10.0f; mVelocityV = 10.0f;
	mMinScale = 1.0f; mMaxScale = 1.0f;

	mEmitDelay = 0.01f;
}

DPhoenix::ParticleEmitter::~ParticleEmitter()
{
	for each(BillboardSprite* sprite in mParticles)
	{
		delete sprite;
		sprite = NULL;
	}

	mParticles.clear();
}

void DPhoenix::ParticleEmitter::Update(float deltaTime)
{
	static GameTimer timer;

	timer.Tick();

	//do we need to add a new particle?
	if (mParticles.size() < mMax)
	{
		//slowdown based on emission delay
		if (timer.TotalTime() > mEmitDelay)
		{
			Add();
			timer.Reset();
		}
	}

	//loop through each particle
	for each(BillboardSprite* particle in mParticles)
	{
		//update particle
		particle->UpdateBS(deltaTime);

		//is particle beyond the emotters range?
		float diffX, diffY, diffZ;
		diffX = particle->mPosition.x - mPosition.x;
		diffY = particle->mPosition.y - mPosition.y;
		diffZ = particle->mPosition.z - mPosition.z;
		float dist = sqrtf(diffX * diffX + diffY * diffY + diffZ * diffZ);

		if (dist > mLength)
		{
			//reset particle to origin
			particle->mPosition = mPosition;
		}

	}
}

bool DPhoenix::ParticleEmitter::Load(std::string filename, TextureMgr * mTexMgr,
	float width, float height, ID3D11Device * md3dDevice)
{

	mSprite = new BillboardSprite();

	mSprite->LoadBS(filename, mTexMgr, width, height, md3dDevice);

	return true;
}

void DPhoenix::ParticleEmitter::Add()
{
	//velocities on x,y,z
	float vX, vY, vZ;

	//new billboard sprite from 'template'; set image and position
	BillboardSprite* p = new BillboardSprite(*mSprite);
	p->mPosition = mPosition;

	//add some randomness to the spread
	float variationH = (rand() % mSpreadH - mSpreadH / 2) / 100.0f;
	float variationV = (rand() % mSpreadV - mSpreadV / 2) / 100.0f;

	//set linear velocity
	float dirH = mDirectionAngleH - 90.0f;
	float dirV = mDirectionAngleV - 90.0f;

	dirH = XMConvertToRadians(dirH);
	dirV = XMConvertToRadians(dirV);

	vX = cos(dirH) + variationH;
	vY = sin(dirV) + variationV;
	vZ = sin(dirH) + variationH;

	p->mVelocity.x = vX * mVelocityH;
	p->mVelocity.y = vY * mVelocityV;
	p->mVelocity.z = vZ * mVelocityH;


	//add particle to the emitter
	mParticles.push_back(p);
}
