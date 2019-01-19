#include "D3DUtil.h"

DPhoenix::Line::Line() : Entity()
{
	mEntityType = ENTITY_LINE;
}

DPhoenix::Line::~Line()
{
}

void DPhoenix::Line::Init(XMFLOAT3 startFrom, XMFLOAT3 direction, float distance)
{
	//set properties for beginning and direction of line
	mLineBegin = startFrom;
	mDirection = direction;
	mLineSize = distance;
	   	 
	mLineEnd.x = mLineBegin.x + mDirection.x * distance;
	mLineEnd.y = mLineBegin.y + mDirection.y * distance;
	mLineEnd.z = mLineBegin.z + mDirection.z * distance;

}

void DPhoenix::Line::AddCollision(float distance, XMFLOAT3 intersectionPoint, Entity* collideBuddy)
{
	//collision detection properties - add new collision
	mCollisionDistances.push_back(distance);
	mIntersectionPoints.push_back(intersectionPoint);
	mCollidedEntities.push_back(collideBuddy);
}

XMFLOAT3 DPhoenix::Line::GetNearestPoint()
{
	float lowestDistance = mLineSize;
	int mLowPointIndex = 0;

	//loop through distances and get lowest
	for (int i = 0; i < mCollisionDistances.size(); i++)
	{
		if (mCollisionDistances[i] < lowestDistance)
		{
			lowestDistance = mLineSize;
			mLowPointIndex = i;
		}
	}

	return mIntersectionPoints[mLowPointIndex];
}

void DPhoenix::Line::SortCollisions()
{
	//should all be same size
	std::vector<float> oldDistances = mCollisionDistances;
	std::vector<XMFLOAT3> newIntersections;
	newIntersections.resize(mIntersectionPoints.size());
	std::vector<Entity*> newCollisionEntities;
	newCollisionEntities.resize(mCollidedEntities.size());

	sort(mCollisionDistances.begin(), mCollisionDistances.end());

	for (int i = 0; i < mCollisionDistances.size(); i++)
	{
		for (int j = 0; j < oldDistances.size(); j++)
		{
			if (mCollisionDistances[i] == oldDistances[j])
			{
				newIntersections[i] = mIntersectionPoints[j];
				newCollisionEntities[i] = mCollidedEntities[j];
			}
		}
	}
	mCollidedEntities = newCollisionEntities;
	mIntersectionPoints = newIntersections;
}
