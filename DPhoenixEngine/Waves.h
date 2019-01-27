#pragma once
#include "D3DUtil.h"

namespace DPhoenix
{

	class Waves : public Entity
	{
	private:
		UINT mNumRows;
		UINT mNumCols;

		UINT mVertexCount;
		UINT mTriangleCount;

		// Simulation constants we can precompute.
		float mK1;
		float mK2;
		float mK3;

		float mTimeStep;
		float mSpatialStep;

		XMFLOAT3* mPrevSolution;
		XMFLOAT3* mCurrSolution;
		XMFLOAT3* mNormals;
		XMFLOAT3* mTangentX;

		//let's keep these buffers in the class rather than outside it
		//I know Luna recvkons the class could be general purpose
		//but we're making water, so here they go
		ID3D11Buffer* mWavesVB;
		ID3D11Buffer* mWavesIB;

	public:

		//keeping these public though it breaks encapsulation simply for speed
		//we are going to assume that there will be one of each of these textures
		//which there will be in the demo code
		ID3D11ShaderResourceView* mWaterColorMapSRV;
		ID3D11ShaderResourceView* mWaterNormalMapSRV;
		ID3D11ShaderResourceView* mWaterEnvironmentalMapSRV;

		//material for lighting
		Material* mWavesMat;

		Waves();
		~Waves();

		UINT RowCount()const;
		UINT ColumnCount()const;
		UINT VertexCount()const;
		UINT TriangleCount()const;
		float Width()const;
		float Depth()const;

		// Returns the solution at the ith grid point.
		const XMFLOAT3& operator[](int i)const { return mCurrSolution[i]; }

		// Returns the solution normal at the ith grid point.
		const XMFLOAT3& Normal(int i)const { return mNormals[i]; }

		// Returns the unit tangent vector at the ith grid point in the local x-axis direction.
		const XMFLOAT3& TangentX(int i)const { return mTangentX[i]; }

		void Init(UINT m, UINT n, float dx, float dt, float speed, float damping,
			ID3D11Device * md3dDevice);
		void Update(float dt, ID3D11DeviceContext* md3dImmediateContext);
		void Disturb(UINT i, UINT j, float magnitude);

		//this generates the World matrix based on scale, 
		//translation (movement) and rotation for rendering
		XMMATRIX CalculateTransforms();

		void Render(ID3D11DeviceContext* md3dImmediateContext);
	};
}