#include "D3DUtil.h"

//constructor
DPhoenix::Camera::Camera()
{
	//create default perspective matrix
	//position at +10 on Z
	mPosition = XMFLOAT3(0.0f, 0.0f, 10.0f);
	
	//new for BTLT
	mGoalPosition = XMFLOAT3(0.0f, 0.0f, 10.0f);

	//Up direction is Y
	mUpDir = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//aspect ratio (should be updated on resize)
	float ratio = 1440 / 900;
	//Send FOV, asepct ratio, nearplane, farplane
	SetPerspective(XM_PI / 4.0f, ratio, 1.0f, 10000.0f);
	//default target looking right
	mTarget.x = 1.0f; mTarget.y = 0.0f; mTarget.z = 0.0f;

	mTargetPos = XMFLOAT3(0.0f, 0.0f, 10.0f);
	mTargetPos.x = 10.0f;
	mGoalTarget = mTargetPos;

	//create default view matrix
	Update(0.0f);
}

//default destructor
DPhoenix::Camera::~Camera()
{
}

void DPhoenix::Camera::SetPerspective(float FOV, float aspectRatio, 
	float nearRange, float farRange)
{
	//set vars
	mFOV = FOV;
	mAspectRatio = aspectRatio;
	mNearRange = nearRange;
	mFarRange = farRange;

	//set the camera's perspecive matrix (projection)
	mMatrixProj = XMMatrixPerspectiveFovLH(mFOV, mAspectRatio, 
		mNearRange, mFarRange);
}

//Update
void DPhoenix::Camera::Update(float dt)
{
	//new for BTLT - move to goal position
	if (mPosition.x != mGoalPosition.x ||
		mPosition.x != mGoalPosition.y ||
		mPosition.x != mGoalPosition.z)
	{
		XMVECTOR goalPosVec = XMLoadFloat3(&mGoalPosition);
		XMVECTOR cameraPosVec = XMLoadFloat3(&mPosition);
		XMVECTOR pointAtGoalVec = XMVectorSubtract(goalPosVec, cameraPosVec);
		pointAtGoalVec = XMVector3Normalize(pointAtGoalVec);

		XMFLOAT3 moveDir;
		XMStoreFloat3(&moveDir, pointAtGoalVec);

		//the camera positions might be very close or vary wildly -
		//hence it best to move 2x distance / ps* dt each time of the distance
		//should get an easing effect
		float xDistPc = abs(mPosition.x - mGoalPosition.x);
		float yDistPc = abs(mPosition.y - mGoalPosition.y);
		float zDistPc = abs(mPosition.z - mGoalPosition.z);

		mPosition.x += moveDir.x * xDistPc * 2.0f * dt;
		mPosition.y += moveDir.y * yDistPc * 2.0f * dt;
		mPosition.z += moveDir.z * zDistPc * 2.0f * dt;

		if (abs(mGoalPosition.x - mPosition.x) < 0.001)
			mPosition.x = mGoalPosition.x;

		if (abs(mGoalPosition.y - mPosition.y) < 0.001)
			mPosition.y = mGoalPosition.y;

		if (abs(mGoalPosition.z - mPosition.z) < 0.001)
			mPosition.z = mGoalPosition.z;
	}

	//new for BTLT - move to goal position
	if (mTargetPos.x != mGoalTarget.x ||
		mTargetPos.x != mGoalTarget.y ||
		mTargetPos.x != mGoalTarget.z)
	{
		XMVECTOR goalLookVec = XMLoadFloat3(&mGoalTarget);
		XMVECTOR cameraLookVec = XMLoadFloat3(&mTargetPos);
		XMVECTOR pointAtGoalVec = XMVectorSubtract(goalLookVec, cameraLookVec);
		pointAtGoalVec = XMVector3Normalize(pointAtGoalVec);

		XMFLOAT3 moveDir;
		XMStoreFloat3(&moveDir, pointAtGoalVec);

		//the camera positions might be very close or vary wildly -
		//hence it best to move 50% * dt each time of the distance
		//should get an easing effect
		float xDistPc = abs(mTargetPos.x - mGoalTarget.x);
		float yDistPc = abs(mTargetPos.y - mGoalTarget.y);
		float zDistPc = abs(mTargetPos.z - mGoalTarget.z);

		mTargetPos.x += moveDir.x * xDistPc * 3.0f * dt;
		mTargetPos.y += moveDir.y * yDistPc * 3.0f * dt;
		mTargetPos.z += moveDir.z * zDistPc * 3.0f * dt;

		if (abs(mGoalTarget.x - mTargetPos.x) < 0.001)
			mTargetPos.x = mGoalTarget.x;

		if (abs(mGoalTarget.y - mTargetPos.y) < 0.001)
			mTargetPos.y = mGoalTarget.y;

		if (abs(mGoalTarget.z - mTargetPos.z) < 0.001)
			mTargetPos.z = mGoalTarget.z;
	}
/*
	mLookDirection = mGoalLookDirection;*/

	XMVECTOR targetPosVec = XMLoadFloat3(&mTargetPos);
	XMVECTOR cameraPosVec = XMLoadFloat3(&mPosition);

	XMVECTOR pointAtTargetVec = XMVectorSubtract(targetPosVec, cameraPosVec);
	pointAtTargetVec = XMVector3Normalize(pointAtTargetVec);

	XMFLOAT3 newLookDir;
	XMStoreFloat3(&newLookDir, pointAtTargetVec);

	//This updates the camera target based on look direction
	mTarget.x = mPosition.x + newLookDir.x;
	mTarget.y = mPosition.y + newLookDir.y;
	mTarget.z = mPosition.z + newLookDir.z;

	//Build the view matrix (position, target, up direction)
	XMVECTOR pos = XMVectorSet(mPosition.x, mPosition.y, mPosition.z, 1.0f);
	XMVECTOR target = XMVectorSet(mTarget.x, mTarget.y, mTarget.z, 1.0f);
	XMVECTOR up = XMVectorSet(mUpDir.x, mUpDir.y, mUpDir.z, 0.0f);

	//create view matrix
	mMatrixView = XMMatrixLookAtLH(pos, target, up);
}

//adjust rotation relative to current rotation values 
void DPhoenix::Camera::Rotate(float x, float y, float z)
{
	mRotation.x += x; mRotation.y += y; mRotation.z += z;
}

//set target valuesrelative to current
void DPhoenix::Camera::Look(float x, float y, float z)
{
	mPosition.x += x; mPosition.y += y; mPosition.z += z;
}

//move relative position and target
void DPhoenix::Camera::Move(float x, float y, float z)
{
	mPosition.x += x; mPosition.y += y; mPosition.z += z;
	mTarget.x += x; mTarget.y += y; mTarget.z += z;
}
