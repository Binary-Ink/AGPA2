#pragma once
#include "D3DUtil.h"

namespace DPhoenix
{
	class Camera
	{
	private:
		//Projection, View and Rotation matrices
		//And Up direction
		XMMATRIX mMatrixProj;
		XMMATRIX mMatrixView;
		XMMATRIX mMatrixRot;
		XMFLOAT3 mUpDir;

		//camera properties
		//near, far, range and aspect ratio
		float mNearRange;
		float mFarRange;
		float mAspectRatio;
		float mFOV;

	public:
		//Position, rotation and target vectors
		XMFLOAT3 mPosition;
		XMFLOAT3 mRotation;
		XMFLOAT3 mTarget;

		//new for BTLT
		XMFLOAT3 mGoalPosition;

		XMFLOAT3 mTargetPos;
		XMFLOAT3 mGoalTarget;

		//constructor / destructor
		Camera();
		~Camera();

		//set perspective
		void SetPerspective(float FOV, float aspectRatio,
			float nearRange, float farRange);

		//get view / projection matrix

		XMMATRIX GetView() { return mMatrixView; }
		XMMATRIX GetProj() { return mMatrixProj; }

		void Update(float dt);

		//rotate / look / move
		void Rotate(float x, float y, float z);
		void Look(float x, float y, float z);
		void Move(float x, float y, float z);

	};
}

