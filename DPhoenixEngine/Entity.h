#pragma once

#include "D3DUtil.h"

namespace DPhoenix
{

	/**
	Entity type used internally to identify type of entity
	**/
	enum EntityType {
		ENTITY_UNKNOWN = -1,
		ENTITY_PLAYER_MESH = 1,
		ENTITY_SCENERY_MESH,
		ENTITY_RAY,
		ENTITY_COLLECTABLE_MESH,
		ENTITY_ENEMY_MESH,
		ENTITY_WEAPON_MESH,
		ENTITY_LINE,
		ENTITY_CAMERA_LINE,
		ENTITY_SELECTION,
		ENTITY_WEAPON_LINE,
		ENTITY_MAGIC_PROJECTILE,
		ENTITY_WALL_MESH,
		ENTITY_COVER_MESH,
		ENTITY_BEACON_MESH,
		ENTITY_CAMERA_COLLISION, 
		ENTITY_WATERWAVES
	};

	class Entity
	{
	public:
		int mId;						//unique id
		std::string mName;				//name
		bool mAlive;					//is alive?
		enum EntityType mEntityType;	//entity type
		float mLifetimeStart;			//start of life
		float mLifetimeCounter;			//how long lived
		float mLifetimeLength;			//how long to live
		bool mVisible;					//is visible?
		bool mCollidable;				//is collidable?
		bool mCollided;					//has collided?
		Entity* mCollideBuddy;			//pointer to collided entity
		std::vector<Entity*> mCollidedEntities;

		//to be reset and then repopulated each turn
		std::vector<Entity*> mAABBCollidedEntities;

		bool mCollidedWithCamera;

		//to be used by child classes
		XMFLOAT3 mPosition;				//position			
		XMFLOAT3 mPrevPosition;			//previous position
		XMFLOAT3 mVelocity;				//velocity
		XMFLOAT3 mRelativeVelocity;		//velocity based on forward vector
		XMFLOAT3 mScale;				//scale
		XMFLOAT3 mRotation;				//rotation
		XMFLOAT3 mForwardVector;		//forward vector
		XMFLOAT3 mRightVector;			//right vector

		//for collision checking
		XMFLOAT3 mHalfSizes;

		//constructor / destructor
		Entity();
		virtual ~Entity() { };

		virtual XMMATRIX CalculateTransforms() = 0;

	};
}

