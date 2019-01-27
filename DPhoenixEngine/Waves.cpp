//***************************************************************************************
// Waves.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************
//Adapted by Deany P for educational purposes

#include "Waves.h"

DPhoenix::Waves::Waves()
	: mNumRows(0), mNumCols(0), mVertexCount(0), mTriangleCount(0),
	mK1(0.0f), mK2(0.0f), mK3(0.0f), mTimeStep(0.0f), mSpatialStep(0.0f),
	mPrevSolution(0), mCurrSolution(0), mNormals(0), mTangentX(0)
{
	mEntityType = ENTITY_WATERWAVES;
}

DPhoenix::Waves::~Waves()
{
	delete[] mPrevSolution;
	delete[] mCurrSolution;
	delete[] mNormals;
	delete[] mTangentX;
}

UINT DPhoenix::Waves::RowCount()const
{
	return mNumRows;
}

UINT DPhoenix::Waves::ColumnCount()const
{
	return mNumCols;
}

UINT DPhoenix::Waves::VertexCount()const
{
	return mVertexCount;
}

UINT DPhoenix::Waves::TriangleCount()const
{
	return mTriangleCount;
}

float DPhoenix::Waves::Width()const
{
	return mNumCols * mSpatialStep;
}

float DPhoenix::Waves::Depth()const
{
	return mNumRows * mSpatialStep;
}

void DPhoenix::Waves::Init(UINT m, UINT n, float dx, float dt, float speed, float damping,
	ID3D11Device * md3dDevice)
{
	mNumRows = m;
	mNumCols = n;

	mVertexCount = m * n;
	mTriangleCount = (m - 1)*(n - 1) * 2;

	mTimeStep = dt;
	mSpatialStep = dx;

	// here we calculate the 'steps' for neighbouring waves
	// and descent over time
	float d = damping * dt + 2.0f;
	float e = (speed*speed)*(dt*dt) / (dx*dx);
	mK1 = (damping*dt - 2.0f) / d;
	mK2 = (4.0f - 8.0f*e) / d;
	mK3 = (2.0f*e) / d;

	// In case Init() called again.
	delete[] mPrevSolution;
	delete[] mCurrSolution;
	delete[] mNormals;
	delete[] mTangentX;

	mPrevSolution = new XMFLOAT3[m*n];
	mCurrSolution = new XMFLOAT3[m*n];
	mNormals = new XMFLOAT3[m*n];
	mTangentX = new XMFLOAT3[m*n];

	// Generate grid vertices in system memory.

	float halfWidth = (n - 1)*dx*0.5f;
	float halfDepth = (m - 1)*dx*0.5f;
	for (UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dx;
		for (UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;

			mPrevSolution[i*n + j] = XMFLOAT3(x, 0.0f, z);
			mCurrSolution[i*n + j] = XMFLOAT3(x, 0.0f, z);
			mNormals[i*n + j] = XMFLOAT3(0.0f, 1.0f, 0.0f);
			mTangentX[i*n + j] = XMFLOAT3(1.0f, 0.0f, 0.0f);
		}
	}

	//now we'll create the initial buffers for the grid
	//becuase we are changing the vertex data, we will
	//need to keep the buffer DYNAMIC - hence we're not assigning 
	//the vertex 'data' yet, just the space
	//we'll assign the vertex data on each update

	// Create the vertex buffer.  Note that we allocate space only, as
	// we will be updating the data every time step of the simulation.

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex::PosNormalTexTan) * VertexCount();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	HR(md3dDevice->CreateBuffer(&vbd, 0, &mWavesVB));

	// Create the index buffer.  The index buffer is fixed, so we only 
	// need to create and set once.

	std::vector<UINT> indices(3 * TriangleCount()); // 3 indices per face

													// Iterate over each quad.
	int k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (DWORD j = 0; j < n - 1; ++j)
		{
			indices[k] = i * n + j;
			indices[k + 1] = i * n + j + 1;
			indices[k + 2] = (i + 1)*n + j;

			indices[k + 3] = (i + 1)*n + j;
			indices[k + 4] = i * n + j + 1;
			indices[k + 5] = (i + 1)*n + j + 1;

			k += 6; // next quad
		}
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mWavesIB));

	//set up the material for lighting
	mWavesMat = new Material();
	mWavesMat->Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mWavesMat->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mWavesMat->Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 32.0f);
}

void DPhoenix::Waves::Update(float dt, ID3D11DeviceContext* md3dImmediateContext)
{
	static float t = 0;

	// Accumulate time.
	t += dt;

	// Only update the simulation at the specified time step.
	if (t >= mTimeStep)
	{
		// Only update interior points; we use zero boundary conditions.
		for (UINT i = 1; i < mNumRows - 1; ++i)
		{
			for (UINT j = 1; j < mNumCols - 1; ++j)
			{
				// After this update we will be discarding the old previous
				// buffer, so overwrite that buffer with the new update.
				// Note how we can do this inplace (read/write to same element) 
				// because we won't need prev_ij again and the assignment happens last.

				// Note j indexes x and i indexes z: h(x_j, z_i, t_k)
				// Moreover, our +z axis goes "down"; this is just to 
				// keep consistent with our row indices going down.

				mPrevSolution[i*mNumCols + j].y =
					mK1 * mPrevSolution[i*mNumCols + j].y +
					mK2 * mCurrSolution[i*mNumCols + j].y +
					mK3 * (
						mCurrSolution[(i + 1)*mNumCols + j].y +
						mCurrSolution[(i - 1)*mNumCols + j].y +
						mCurrSolution[i*mNumCols + j + 1].y +
						mCurrSolution[i*mNumCols + j - 1].y
						);
			}
		}

		// We just overwrote the previous buffer with the new data, so
		// this data needs to become the current solution and the old
		// current solution becomes the new previous solution.
		std::swap(mPrevSolution, mCurrSolution);

		t = 0.0f; // reset time

				  //
				  // Compute normals using finite difference scheme.
				  //

				  // Dean Note: finite difference equations enable us in this case
				  // to average out the difference between 'wave points' to calculate the new normal
				  // see notes in worksheet
				  // this fulfils the criteria for implementing mathematical difference / differentiation

		for (UINT i = 1; i < mNumRows - 1; ++i)
		{
			for (UINT j = 1; j < mNumCols - 1; ++j)
			{
				//get neighbouing y positions (x)
				float l = mCurrSolution[i*mNumCols + j - 1].y;
				float r = mCurrSolution[i*mNumCols + j + 1].y;
				//get neighbouing y positions (y)
				float t = mCurrSolution[(i - 1)*mNumCols + j].y;
				float b = mCurrSolution[(i + 1)*mNumCols + j].y;

				//now apply the finite difference equation 
				mNormals[i*mNumCols + j].x = -r + l;
				mNormals[i*mNumCols + j].y = 2.0f*mSpatialStep;
				mNormals[i*mNumCols + j].z = b - t;

				//normalise the result to provide the final normal direction
				XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&mNormals[i*mNumCols + j]));
				XMStoreFloat3(&mNormals[i*mNumCols + j], n);

				//tangent is calculated also
				mTangentX[i*mNumCols + j] = XMFLOAT3(2.0f*mSpatialStep, r - l, 0.0f);
				XMVECTOR T = XMVector3Normalize(XMLoadFloat3(&mTangentX[i*mNumCols + j]));
				XMStoreFloat3(&mTangentX[i*mNumCols + j], T);
			}
		}
	}

	//this bit here integrates the mapping of the vertex buffers with the new data
	//hence the dynamic vertex buffer

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(md3dImmediateContext->Map(mWavesVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	//generate input layout from vertex data as calculated in 'waves' solution
	//(the calculations just performed for the wave behaviour)
	Vertex::PosNormalTexTan* v = reinterpret_cast<Vertex::PosNormalTexTan*>(mappedData.pData);
	for (UINT i = 0; i < VertexCount(); ++i)
	{
		v[i].Pos = mCurrSolution[i];
		v[i].Normal = Normal(i);
		v[i].TangentU.x = TangentX(i).x;
		v[i].TangentU.y = TangentX(i).y;
		v[i].TangentU.z = TangentX(i).z;
		v[i].TangentU.w = 1.0f;

		// Derive tex-coords in [0,1] from position.
		v[i].Tex.x = 0.5f + mCurrSolution[i].x / Width();
		v[i].Tex.y = 0.5f - mCurrSolution[i].z / Depth();
	}

	md3dImmediateContext->Unmap(mWavesVB, 0);
}

void DPhoenix::Waves::Disturb(UINT i, UINT j, float magnitude)
{
	// Don't disturb boundaries.
	assert(i > 1 && i < mNumRows - 2);
	assert(j > 1 && j < mNumCols - 2);

	float halfMag = 0.5f*magnitude;

	// Disturb the ijth vertex height and its neighbors.
	mCurrSolution[i*mNumCols + j].y += magnitude;
	mCurrSolution[i*mNumCols + j + 1].y += halfMag;
	mCurrSolution[i*mNumCols + j - 1].y += halfMag;
	mCurrSolution[(i + 1)*mNumCols + j].y += halfMag;
	mCurrSolution[(i - 1)*mNumCols + j].y += halfMag;
}

//apply transformations and output World matrix
//(to convert from object space to world space)
XMMATRIX DPhoenix::Waves::CalculateTransforms()
{
	//initialise matrices with identity matrix where appropriate
	XMMATRIX Scale = XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	XMMATRIX Translation = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	XMMATRIX RotationX = XMMatrixRotationX(mRotation.x);
	XMMATRIX RotationY = XMMatrixRotationY(mRotation.y);
	XMMATRIX RotationZ = XMMatrixRotationZ(mRotation.z);

	//rotations must be conmcatenated correctly in this order
	XMMATRIX Rotation = XMMatrixMultiply(RotationZ, XMMatrixMultiply(RotationY, RotationX));

	//final transforms must also be concatenated in this order (TSR)
	XMMATRIX World = XMMatrixMultiply(Rotation, XMMatrixMultiply(Scale, Translation));

	//return the matrix so it can be used with teh shader
	return World;
}

void DPhoenix::Waves::Render(ID3D11DeviceContext * md3dImmediateContext)
{
	UINT stride = sizeof(Vertex::PosNormalTexTan);
	UINT offset = 0;

	md3dImmediateContext->IASetVertexBuffers(0, 1, &mWavesVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mWavesIB, DXGI_FORMAT_R32_UINT, 0);

	md3dImmediateContext->DrawIndexed(3 * TriangleCount(), 0, 0);
}