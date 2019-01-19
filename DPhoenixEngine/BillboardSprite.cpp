#include "BillboardSprite.h"

DPhoenix::BillboardSprite::BillboardSprite() : Sprite()
{
	maintainY = true;
	mRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

DPhoenix::BillboardSprite::BillboardSprite(const BillboardSprite & sprite)
{
	*this = sprite;
}

DPhoenix::BillboardSprite::~BillboardSprite()
{

}

bool DPhoenix::BillboardSprite::LoadBS(std::string filename, TextureMgr * mTexMgr,
	float width, float height, ID3D11Device * md3dDevice)
{
	//create new texture from texture manager
	mSpriteTexSRV = mTexMgr->CreateTexture(filename);

	//get the size on x, y from width, height
	mSize.x = width;
	mSize.y = height;

	//calculate half sizes so origin in centre
	float halfWidth = mSize.x / 2;
	float halfHeight = mSize.y / 2;

	//A quad with UV co-ords
	Vertex::PosNormalTexTan vertices[] =
	{
		//pos, normal, tex, tan
		{ XMFLOAT3(-halfWidth, halfHeight, 1.0f),
			XMFLOAT3(0.f, 0.0f, 1.0f),
			XMFLOAT2(1.0f, 0.0f),
			XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3(-halfWidth, -halfHeight, 1.0f),
			XMFLOAT3(0.f, 0.0f, 1.0f),
			XMFLOAT2(1.0f, 1.0f),
			XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3(halfWidth, -halfHeight, 1.0f),
			XMFLOAT3(0.f, 0.0f, 1.0f),
			XMFLOAT2(0.0f, 1.0f),
			XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3(halfWidth, halfHeight, 1.0f),
			XMFLOAT3(0.f, 0.0f, 1.0f),
			XMFLOAT2(0.0f, 0.0f),
			XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f) }
	};

	//create vertex buffer
	D3D11_BUFFER_DESC vbd;									//desc struct for vertex buffer
	vbd.Usage = D3D11_USAGE_IMMUTABLE;						//usage flags (can't edit but quicker)
	vbd.ByteWidth = sizeof(Vertex::PosNormalTexTan) * 4;	//four vertices so multiply size by 4
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//bind as vertex buffer
	vbd.CPUAccessFlags = 0;									//CPU access flags
	vbd.MiscFlags = 0;										//Misc flags
	vbd.StructureByteStride = 0;							//this goes pretty deep! Lets leave at 0...
															//create subresource with vertex buffer data
	D3D11_SUBRESOURCE_DATA vinitData;
	//apply vertices array
	vinitData.pSysMem = vertices;
	//create buffer from device (stored in mSpriteVB)
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mSpriteVB));

	// Create the index buffer
	//(two triangles from teh four vertices)
	UINT indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,
	};

	//create index buffer subresource
	D3D11_BUFFER_DESC ibd;								//desc struct for vertex buffer
	ibd.Usage = D3D11_USAGE_IMMUTABLE;					//immutable as the box will always be a box
	ibd.ByteWidth = sizeof(UINT) * 6;					//6 indices
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;			//Bind as index buffer
	ibd.CPUAccessFlags = 0;								//CPU access flags
	ibd.MiscFlags = 0;									//Misc flags
	ibd.StructureByteStride = 0;						//this goes pretty deep! Lets leave at 0...
														//create subresource with index buffer data
	D3D11_SUBRESOURCE_DATA iinitData;
	//apply indices array
	iinitData.pSysMem = indices;
	//create buffer from device (stored in mSpriteIB)
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mSpriteIB));

	return true;
}

void DPhoenix::BillboardSprite::UpdateBS(float deltaTime)
{
	//move sprite by velocity amount
	mPosition.x += mVelocity.x * deltaTime;
	mPosition.y += mVelocity.y * deltaTime;
	mPosition.z += mVelocity.z * deltaTime;

	//increment animation frame
	mCurrentFrame += (float)((float)mAnimationDirection * deltaTime);

	//keep current frame within bounds
	//ceil / floor used to consider animation directions
	if ((int)ceil(mCurrentFrame) < mFirstFrame)
		mCurrentFrame = (float)mLastFrame;
	if ((int)floor(mCurrentFrame) > mLastFrame)
		mCurrentFrame = (float)mFirstFrame;
}

void DPhoenix::BillboardSprite::RenderBS(ID3D11DeviceContext * dc)
{
	//calculate the size of individual data points
	UINT stride = sizeof(Vertex::PosNormalTexTan);
	UINT offset = 0;

	//set the vertex buffer to the input assembler
	dc->IASetVertexBuffers(0, 1, &mSpriteVB, &stride, &offset);
	//set the index buffer to the input assembler
	dc->IASetIndexBuffer(mSpriteIB, DXGI_FORMAT_R32_UINT, 0);

	//draw the quad using the indices (6 indices total)
	dc->DrawIndexed(6, 0, 0);
}

XMMATRIX DPhoenix::BillboardSprite::GetWorldBS(XMFLOAT3 eyePos)
{
	//initialise matrices with identity matrix where appropriate
	XMMATRIX Scale = XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	XMMATRIX Translation = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

	XMMATRIX RotationX;
	XMMATRIX RotationY;
	XMMATRIX RotationZ;

	if (maintainY)
	{
		// Calculate the rotation that needs to be applied to the 
		//billboard model to face the current camera position using the arc tangent function.
		float angle = atan2(eyePos.x - mPosition.x, eyePos.z - mPosition.z);

		RotationX = XMMatrixRotationX(mRotation.x);
		RotationY = XMMatrixRotationY(angle);
		RotationZ = XMMatrixRotationZ(mRotation.z);
	}
	else
	{
		//how to make it face always????

	}

	//rotations must be concatenated correctly in this order
	XMMATRIX Rotation = XMMatrixMultiply(RotationZ, XMMatrixMultiply(RotationY, RotationX));

	//final transforms must also be concatenated in this order (TSR)
	XMMATRIX World = XMMatrixMultiply(Rotation, XMMatrixMultiply(Scale, Translation));

	//return the matrix so it can be used with teh shader
	return World;
}


