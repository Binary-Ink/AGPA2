#pragma once
#include "D3DUtil.h"

namespace DPhoenix
{
	class Line : public Entity
	{
	public:
		XMFLOAT3 mLineBegin;						//origin
		XMFLOAT3 mLineEnd;							//end
		XMFLOAT3 mDirection;						//direction from origin
		float mLineSize;							//size of line
		std::vector<float> mCollisionDistances;		//distance from where line hits
		std::vector<XMFLOAT3> mIntersectionPoints;	//point of intersection
		

		//constructor / destructor
		Line();
		~Line();

		//init - takes in origin, direction and line size / distance
		void Init(XMFLOAT3 startFrom, XMFLOAT3 direction, float distance);

		//add collision properties - build up collection of intersections
		void AddCollision(float distance, XMFLOAT3 intersectionPoint, Entity* collideBuddy);
		XMFLOAT3 GetNearestPoint();

		//empty method for inheritance
		XMMATRIX CalculateTransforms() { return XMMATRIX(); };
		void SortCollisions();
	};
}

