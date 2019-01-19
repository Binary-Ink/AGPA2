#include "Engine.h"

namespace DPhoenix
{
	//constructor - initialise vars
	Entity::Entity()
	{
		mId = -1;
		mName = "";
		mVisible = false;
		mAlive = true;
		mEntityType = ENTITY_UNKNOWN;
		mLifetimeStart = 0;
		mLifetimeCounter = 0;
		mLifetimeLength = 0;
		mCollidable = false;
		mCollided = false;

		mCollidedWithCamera = false;

		//default values for all properties
		//note scale must be 1! Nothing will render at 0!
		mPosition.x = 0.0f; mPosition.y = 0.0f; mPosition.z = 0.0f;
		mPrevPosition.x = 0.0f; mPrevPosition.y = 0.0f; mPrevPosition.z = 0.0f;
		mVelocity.x = 0.0f; mVelocity.y = 0.0f; mVelocity.z = 0.0f;
		mRelativeVelocity.x = 0.0f; mRelativeVelocity.y = 0.0f; mRelativeVelocity.z = 0.0f;
		mScale.x = 1.0f; mScale.y = 1.0f; mScale.z = 1.0f;
		mRotation.x = 0.0f; mRotation.y = 0.0f; mRotation.z = 0.0f;
		mForwardVector.x = 0.0f; mForwardVector.y = 0.0f; mForwardVector.z = 1.0f;
		mRightVector.x = 1.0f; mRightVector.y = 0.0f; mRightVector.z = 0.0f;

		mHalfSizes.x = 0.0f; 	mHalfSizes.y = 0.0f; 	mHalfSizes.z = 0.0f;

	}

};


