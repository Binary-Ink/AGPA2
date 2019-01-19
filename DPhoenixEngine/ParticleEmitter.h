#pragma once
#include "BillboardSprite.h"

namespace DPhoenix
{
	class ParticleEmitter
	{
	public:
		//STL vector for particles
		std::vector<BillboardSprite*> mParticles;

		//Billboard Sprite as 'template'
		BillboardSprite* mSprite;

		//position vars
		XMFLOAT3 mPosition;
		float mDirectionAngleH;
		float mDirectionAngleV;
		float mLength;

		//colour / amount / alpha variation
		int mMax;
		int mMinAlpha, mMaxAlpha;
		int mMinR, mMinG, mMinB;
		int mMaxR, mMaxG, mMaxB;

		//emission
		float mEmitDelay;

		//spread vars
		int mSpreadH, mSpreadV;
		float mVelocityH, mVelocityV;
		float mMinScale, mMaxScale;

		//methods ----------------------------

		//constructor / destructor
		ParticleEmitter();
		virtual ~ParticleEmitter();

		//render / update
		void Update(float deltaTime);

		//load image
		bool Load(std::string filename, TextureMgr* mTexMgr,
			float width, float height, ID3D11Device* md3dDevice);

		//dynamically add particles
		void Add();

		void SetAlphaRange(float min, float max) { mMinAlpha = min; mMaxAlpha = max; }

		void SetColorRange(float minR, float minG, float minB,
			float maxR, float maxG, float maxB)
		{
			mMinR = minR; mMinG = minG; mMinB = minB;
			mMaxR = maxR; mMaxG = maxG; mMaxB = maxB;
		}
	};
}
