#include "PrimitiveInstance.h"

//constructor - initialise members
DPhoenix::PrimitiveInstance::PrimitiveInstance()
{
	//null pointers before use
	mMeshPtr = NULL;
	mPrimitiveVB = NULL;
	mPrimitiveIB = NULL;
	mDiffuseMap = NULL;
	//default type
	mPrimitiveType = PRIMITIVE_TEX;

	//default values for all properties
	//note scale must be 1! Nothing will render at 0!
	mPosition.x = 0.0f; mPosition.y = 0.0f; mPosition.z = 0.0f;
	mVelocity.x = 0.0f; mVelocity.y = 0.0f; mVelocity.z = 0.0f;
	mRelativeVelocity.x = 0.0f; mRelativeVelocity.y = 0.0f; mRelativeVelocity.z = 0.0f;
	mScale.x = 1.0f; mScale.y = 1.0f; mScale.z = 1.0f;					
	mRotation.x = 0.0f; mRotation.y = 0.0f; mRotation.z = 0.0f;
	mForwardVector.x = 0.0f; mForwardVector.y = 0.0f; mForwardVector.z = 1.0f;
	mRightVector.x = 1.0f; mRightVector.y = 0.0f; mRightVector.z = 0.0f;
	mTexScaleU = 1.0f; mTexScaleV = 1.0f;

	mOpacity = 1.0f;
}

//destructor - release memory
DPhoenix::PrimitiveInstance::~PrimitiveInstance()
{
	//release vertex and index buffers
	ReleaseCOM(mPrimitiveVB);
	ReleaseCOM(mPrimitiveIB);
}

//Update method; handle movement based on velocity and delta time
void DPhoenix::PrimitiveInstance::Update(float deltaTime, bool moveRelative)
{
	//hold previous position
	mPrevPosition = mPosition;

	//if we are moving relatively to the object's forward vector we use this
	//(May need to adjust after some experimentation)
	if (moveRelative)
	{
		//get up vector (positive y)
		XMFLOAT3 vecUp(0.0f, 1.0f, 0.0f);
		//calculate right vector based on cross product of up and forward
		XMVECTOR vecRight = XMVector3Cross(XMLoadFloat3(&vecUp),
			XMLoadFloat3(&mForwardVector));
		XMVECTOR vecForward = XMLoadFloat3(&mForwardVector);
		//scale direction forward / right based on X / Z relative velocity
		vecForward = XMVectorScale(vecForward, mRelativeVelocity.z);
		vecRight = XMVectorScale(vecRight, mRelativeVelocity.x);

		//adding the forward and right vectors gives us a final velocity
		XMVECTOR velocityVec = XMVectorAdd(vecForward, vecRight);
		XMFLOAT3 velocity; XMStoreFloat3(&velocity, velocityVec);

		//update position based on velocity updating
		mPosition.x += velocity.x * deltaTime;
		mPosition.y += mRelativeVelocity.y * deltaTime;
		mPosition.z += velocity.z * deltaTime;
	}
	//otherwise this is global movement / world space
	else
	{
		mPosition.x += mVelocity.x * deltaTime;
		mPosition.y += mVelocity.y * deltaTime;
		mPosition.z += mVelocity.z * deltaTime;
	}

	SetFacingRotation(0.0f);
}

//Load a mesh instance with only vertex and color data
void DPhoenix::PrimitiveInstance::LoadColorInstance(ID3D11Device * md3dDevice, XMFLOAT4 color,
	GeometryGenerator::MeshData* meshPtr)
{
	//set render type
	mPrimitiveType = PRIMITIVE_COLOR;

	//assign pointer to teh MeshData
	mMeshPtr = meshPtr;

	//create a new vector of vertices for the buffer
	//same size as amount of vrtices from mesh data
	std::vector<Vertex::PosColor> vertices;
	vertices.resize(mMeshPtr->Vertices.size());

	//this loop extracts the position data for each vertex
	//and assigns it to our new vector as well as the color data
	for (size_t i = 0; i < mMeshPtr->Vertices.size(); ++i)
	{
		vertices[i].Pos = mMeshPtr->Vertices[i].Position;
		vertices[i].Color = color;
	}

	//Vertex buffer 
	D3D11_BUFFER_DESC vbd;						//desc struct for vertex buffer
	vbd.Usage = D3D11_USAGE_IMMUTABLE;			//usage flags (can't edit but quicker)
	vbd.ByteWidth = sizeof(Vertex::PosColor)	//multiply struct size by num of vertices
		* mMeshPtr->Vertices.size();			
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//bind as vertex buffer
	vbd.CPUAccessFlags = 0;						//CPU access flags
	vbd.MiscFlags = 0;							//Misc flags
	vbd.StructureByteStride = 0;				//this goes pretty deep! Lets leave at 0...

	//create subresource with vertex buffer data
	D3D11_SUBRESOURCE_DATA vinitData;
	//apply vertices array
	vinitData.pSysMem = &vertices[0];
	//create buffer from device(stored in mPrimitiveVB)
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mPrimitiveVB));

	//create new indices vector
	std::vector<UINT> indices;
	//this transfers the data from MeshData to our new vector to create the buffer
	indices.insert(indices.end(), mMeshPtr->Indices.begin(), mMeshPtr->Indices.end());

	//create index buffer subresource
	D3D11_BUFFER_DESC ibd;										//desc struct for vertex buffer
	ibd.Usage = D3D11_USAGE_IMMUTABLE;							//immutable as the shape will be the same
	ibd.ByteWidth = sizeof(UINT) * mMeshPtr->Indices.size();	//amount of indices * size per index
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;					//Bind as index buffer
	ibd.CPUAccessFlags = 0;										//CPU access flags
	ibd.MiscFlags = 0;											//Misc flags
	ibd.StructureByteStride = 0;								//this goes pretty deep! Lets leave at 0...

	D3D11_SUBRESOURCE_DATA iinitData;
	//apply indices array
	iinitData.pSysMem = &indices[0];
	//create buffer from device (stored in mPrimitiveIB)
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mPrimitiveIB));
}

//Load a mesh instance with only vertex and texture data
void DPhoenix::PrimitiveInstance::LoadBasicTexInstance(ID3D11Device* md3dDevice, std::string texFilename,
	GeometryGenerator::MeshData* meshPtr, TextureMgr* texMgr)
{

	mPrimitiveType = PRIMITIVE_TEX;

	//create new texture from texture manager
	mDiffuseMap = texMgr->CreateTexture(texFilename);

	//assign pointer to teh MeshData
	mMeshPtr = meshPtr;

	//create a new vector of vertices for the buffer
	//same size as amount of vrtices from mesh data
	std::vector<Vertex::PosTex> vertices;
	vertices.resize(mMeshPtr->Vertices.size());

	//this loop extracts the position data for each vertex
	//and assigns it to our new vector as well as the color data
	for (size_t i = 0; i < mMeshPtr->Vertices.size(); ++i)
	{
		vertices[i].Pos = mMeshPtr->Vertices[i].Position;
		vertices[i].Tex = mMeshPtr->Vertices[i].TexC;
	}

	//Vertex buffer 
	D3D11_BUFFER_DESC vbd;						//desc struct for vertex buffer
	vbd.Usage = D3D11_USAGE_IMMUTABLE;			//usage flags (can't edit but quicker)
	vbd.ByteWidth = sizeof(Vertex::PosTex)	//multiply struct size by num of vertices
		* mMeshPtr->Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//bind as vertex buffer
	vbd.CPUAccessFlags = 0;						//CPU access flags
	vbd.MiscFlags = 0;							//Misc flags
	vbd.StructureByteStride = 0;				//this goes pretty deep! Lets leave at 0...

	//create subresource with vertex buffer data
	D3D11_SUBRESOURCE_DATA vinitData;
	//apply vertices array
	vinitData.pSysMem = &vertices[0];
	//create buffer from device(stored in mPrimitiveVB)
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mPrimitiveVB));

	//create new indices vector
	std::vector<UINT> indices;
	//this transfers the data from MeshData to our new vector to create the buffer
	indices.insert(indices.end(), mMeshPtr->Indices.begin(), mMeshPtr->Indices.end());

	//create index buffer subresource
	D3D11_BUFFER_DESC ibd;										//desc struct for vertex buffer
	ibd.Usage = D3D11_USAGE_IMMUTABLE;							//immutable as the shape will be the same
	ibd.ByteWidth = sizeof(UINT) * mMeshPtr->Indices.size();	//amount of indices * size per index
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;					//Bind as index buffer
	ibd.CPUAccessFlags = 0;										//CPU access flags
	ibd.MiscFlags = 0;											//Misc flags
	ibd.StructureByteStride = 0;								//this goes pretty deep! Lets leave at 0...

	D3D11_SUBRESOURCE_DATA iinitData;
	//apply indices array
	iinitData.pSysMem = &indices[0];
	//create buffer from device (stored in mPrimitiveIB)
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mPrimitiveIB));
}

//Load a mesh instance with full data (position, normal, texture, tangent)
void DPhoenix::PrimitiveInstance::LoadLitTexInstance(ID3D11Device* md3dDevice, std::string texFilename,
	GeometryGenerator::MeshData* meshPtr, TextureMgr* texMgr)
{

	mPrimitiveType = PRIMITIVE_LITTEX;

	//create new texture from texture manager
	mDiffuseMap = texMgr->CreateTexture(texFilename);

	//assign pointer to teh MeshData
	mMeshPtr = meshPtr;

	//create a new vector of vertices for the buffer
	//same size as amount of vrtices from mesh data
	std::vector<Vertex::PosNormalTexTan> vertices;
	vertices.resize(mMeshPtr->Vertices.size());

	//this loop extracts the position data for each vertex
	//and assigns it to our new vector as well as the color data
	for (size_t i = 0; i < mMeshPtr->Vertices.size(); ++i)
	{
		vertices[i].Pos = mMeshPtr->Vertices[i].Position;
		vertices[i].Normal = mMeshPtr->Vertices[i].Normal;
		vertices[i].Tex = mMeshPtr->Vertices[i].TexC;

		//pad out the tangent for efficiency
		XMFLOAT4 newTangent(mMeshPtr->Vertices[i].TangentU.x,
			mMeshPtr->Vertices[i].TangentU.y,
			mMeshPtr->Vertices[i].TangentU.z,
			1.0f);

		vertices[i].TangentU = newTangent;
	}

	//Vertex buffer 
	D3D11_BUFFER_DESC vbd;						//desc struct for vertex buffer
	vbd.Usage = D3D11_USAGE_IMMUTABLE;			//usage flags (can't edit but quicker)
	vbd.ByteWidth = sizeof(Vertex::PosNormalTexTan)	//multiply struct size by num of vertices
		* mMeshPtr->Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//bind as vertex buffer
	vbd.CPUAccessFlags = 0;						//CPU access flags
	vbd.MiscFlags = 0;							//Misc flags
	vbd.StructureByteStride = 0;				//this goes pretty deep! Lets leave at 0...

												//create subresource with vertex buffer data
	D3D11_SUBRESOURCE_DATA vinitData;
	//apply vertices array
	vinitData.pSysMem = &vertices[0];
	//create buffer from device(stored in mPrimitiveVB)
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mPrimitiveVB));

	//create new indices vector
	std::vector<UINT> indices;
	//this transfers the data from MeshData to our new vector to create the buffer
	indices.insert(indices.end(), mMeshPtr->Indices.begin(), mMeshPtr->Indices.end());

	//create index buffer subresource
	D3D11_BUFFER_DESC ibd;										//desc struct for vertex buffer
	ibd.Usage = D3D11_USAGE_IMMUTABLE;							//immutable as the shape will be the same
	ibd.ByteWidth = sizeof(UINT) * mMeshPtr->Indices.size();	//amount of indices * size per index
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;					//Bind as index buffer
	ibd.CPUAccessFlags = 0;										//CPU access flags
	ibd.MiscFlags = 0;											//Misc flags
	ibd.StructureByteStride = 0;								//this goes pretty deep! Lets leave at 0...

	D3D11_SUBRESOURCE_DATA iinitData;
	//apply indices array
	iinitData.pSysMem = &indices[0];
	//create buffer from device (stored in mPrimitiveIB)
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mPrimitiveIB));
}

//apply transformations and output World matrix
//(to convert from object space to world space)
XMMATRIX DPhoenix::PrimitiveInstance::CalculateTransforms()
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

//render - takes in a device context and draws the buffers
void DPhoenix::PrimitiveInstance::Render(ID3D11DeviceContext* dc)
{
	//stride (size of each data point)
	UINT stride;

	switch (mPrimitiveType)
	{
	case PRIMITIVE_COLOR:
		stride = sizeof(Vertex::PosColor);
		break;
	case PRIMITIVE_TEX:
		stride = sizeof(Vertex::PosTex);
		break;
	case PRIMITIVE_LITTEX:
		stride = sizeof(Vertex::PosNormalTexTan);
		break;
	}

	//the offset is where in the data to begin
	//0 means the beginning
	UINT offset = 0;
	
	//this sets the vertex buffer to the
	//Input Assembler
	dc->IASetVertexBuffers(
		0, 1, &mPrimitiveVB, &stride, &offset
	);
	//this sets the index buffer to the 
	//Input Assembler
	dc->IASetIndexBuffer(
		mPrimitiveIB, DXGI_FORMAT_R32_UINT, 0
	);

	//draw the mesh using the indices
	dc->DrawIndexed(mMeshPtr->Indices.size(), 0, 0);
}

void DPhoenix::PrimitiveInstance::SetFacingRotation(float offsetDeg)
{
	// Calculate the rotation that needs to be applied to the 
	//model to face the forward vector
	float angle = atan2(-mForwardVector.x, -mForwardVector.z) * (180.0 / XM_PI);

	// Convert rotation into radians 
	//(offset is there in case models face odd directions from origin).
	float rotation = (float)(angle + offsetDeg) * 0.0174532925f;

	mRotation.y = rotation;
}
