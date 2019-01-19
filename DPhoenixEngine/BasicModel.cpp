#include "D3DUtil.h"

DPhoenix::BasicModel::BasicModel(ID3D11Device* device, TextureMgr& texMgr, 
	const std::string& modelFilename, const std::string& texturePath)
{
	//Assimp 
	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some postprocessing
	const aiScene* scene = importer.ReadFile(modelFilename,
		aiProcess_CalcTangentSpace |		//calculate tangent data for normal mapping
		aiProcess_Triangulate |				//arrange as triangles for rendering
		aiProcess_JoinIdenticalVertices |	//don't duplicate identical indices
		aiProcess_SortByPType |
		aiProcess_SplitLargeMeshes |
		aiProcess_MakeLeftHanded |
		aiProcess_FlipUVs);					//UVs need to be flipped and left handedness for D3D

	//total vertex size is 3 * 3D (pos, normal, tan) and 1 * 2D (UV)
	const int iVertexTotalSize = sizeof(aiVector3D) * 3 + sizeof(aiVector2D);
	//initilaise count for total vertices
	int iTotalVertices = 0;
	//vectors to hold counts and indices
	std::vector<int> materialIndices;
	std::vector<int> iMeshStartIndices;
	std::vector<int> iMeshSizes;
	//loop through the number of meshes in the 'scene'
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		//new subset struct
		MeshGeometry::Subset newSubset;

		//retrieve assmp mesh
		aiMesh* mesh = scene->mMeshes[i];
		//get number of faces and material index for subset
		int iMeshFaces = mesh->mNumFaces;
		materialIndices.push_back(mesh->mMaterialIndex);

		//where are we up to in vertices?
		//this is where the subset will start from in the buffers
		int iSizeBefore = Vertices.size();
		iMeshStartIndices.push_back(iSizeBefore);
		//set the material index for this subset
		newSubset.materialIndex = mesh->mMaterialIndex;
		//vertex start point in buffers for this subset
		newSubset.VertexStart = iSizeBefore;

		//if this is not the first subset
		if (iSizeBefore > 0)
		{
			//we are starting ourt face indices from the end of the last subsets indices
			newSubset.FaceStart = Subsets.back().FaceCount + Subsets.back().FaceStart;
		}
		else
		{
			//or start at the beginning of the buffer
			newSubset.FaceStart = 0;
		}
		//set teh face count for this subset
		newSubset.FaceCount = iMeshFaces;

		//loop through the faces
		for (int j = 0; j < iMeshFaces; j++)
		{
			//get the face data (should be three each for each triangle)
			const aiFace& face = mesh->mFaces[j];
			for (int k = 0; k < face.mNumIndices; k++)
			{
				//update indices vector offset by position in buffer
				Indices.push_back(face.mIndices[k] + iSizeBefore);
			}
		}

		//loop through the vertcices
		for (int j = 0; j < mesh->mNumVertices; j++)
		{
			//get the pos, tex, normal and tan
			aiVector3D pos = mesh->mVertices[j];
			aiVector3D uv = mesh->mTextureCoords[0][j];
			aiVector3D normal = mesh->mNormals[j];
			aiVector3D tangent = mesh->mTangents[j];

			//create new vertex and copy over values
			Vertex::PosNormalTexTan vertex;
			vertex.Pos.x = pos.x; vertex.Pos.y = pos.y; vertex.Pos.z = pos.z;
			vertex.Tex.x = uv.x; vertex.Tex.y = uv.y;
			vertex.Normal.x = normal.x; vertex.Normal.y = normal.y; vertex.Normal.z = normal.z;
			vertex.TangentU.x = tangent.x; vertex.TangentU.y = tangent.y;
			vertex.TangentU.z = tangent.z; vertex.TangentU.w = 1.0f;

			//copy over from vertex naemspace struct to this copy
			//damn namespace errors!!!!!
			PosNormalTexTan vertexCopy;
			vertexCopy.Normal = vertex.Normal;
			vertexCopy.Pos = vertex.Pos;
			vertexCopy.TangentU = vertex.TangentU;
			vertexCopy.Tex = vertex.Tex;

			//push back the vertex to the vector
			Vertices.push_back(vertexCopy);
		}

		//set number of vertices for this subset and add to total vertices
		int iMeshVertices = mesh->mNumVertices;
		iTotalVertices += iMeshVertices;
		//calculate the number of vertices for this subset
		iMeshSizes.push_back((Vertices.size() - iSizeBefore));
		//and set to this subset struct
		newSubset.VertexCount = Vertices.size() - iSizeBefore;
		//add the completed subset to the subsets vector
		Subsets.push_back(newSubset);
	}
	//set vertex and index buffers with complete data
	ModelMesh.SetVertices(device, &Vertices[0], Vertices.size());
	ModelMesh.SetIndices(device, &Indices[0], Indices.size());
	//set the subset table and count
	ModelMesh.SetSubsetTable(Subsets);
	SubsetCount = Subsets.size();
	//get the number of materials for the mesh
	int iNumMaterials = scene->mNumMaterials;

	Mat.resize(iNumMaterials);
	DiffuseMapSRV.resize(iNumMaterials);
	NormalMapSRV.resize(iNumMaterials);

	//loop through materials
	for (int i = 0; i < iNumMaterials; i++)
	{
		//create pointer to material in assimp format
		const aiMaterial* material = scene->mMaterials[i];
		int texIndex = 0;
		aiString path;  // filename

		//get diffuse color of material
		aiColor4D diffuseColor;
		aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);
		//get specular color of material
		aiColor4D specularColor;
		aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specularColor);
		//get ambient color of material
		aiColor4D ambientColor;
		aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambientColor);
		//get name of material (not actually used but could be...)
		aiString matName;
		material->Get(AI_MATKEY_NAME, matName);

		//not sure which one to use... / have used shininess for specular power
		float shininess;
		material->Get(AI_MATKEY_SHININESS, shininess);

		float shininessStrength;
		material->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);

		//material in struct format for our engine and shaders
		Material dxMaterial;
		//zeroise the struct
		ZeroMemory(&dxMaterial, sizeof(dxMaterial));
		//copy over diffuse properties
		dxMaterial.Diffuse.x = diffuseColor.r; dxMaterial.Diffuse.y = diffuseColor.g;
		dxMaterial.Diffuse.z = diffuseColor.b; dxMaterial.Diffuse.w = 1.0f;
		//copy over ambient properties
		dxMaterial.Ambient.x = ambientColor.r; dxMaterial.Ambient.y = ambientColor.g;
		dxMaterial.Ambient.z = ambientColor.b; dxMaterial.Ambient.w = 1.0f;
		//copuy over specualr properties (shininess for power)
		dxMaterial.Specular.x = specularColor.r; dxMaterial.Specular.y = specularColor.g;
		dxMaterial.Specular.z = specularColor.b; dxMaterial.Specular.w = shininess;
		//add the material to the vector
		Mat[i] = dxMaterial;

		int texCount = material->GetTextureCount(aiTextureType_DIFFUSE);

		for (texIndex = 0; texIndex < texCount; texIndex++)
		{

			//if the material has a diffuse texture...
			if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS)
			{
				//copy over path in string format
				const char* cPathStr = path.C_Str();
				std::string  strPathStr(cPathStr);
				//create texture and store the Shader Resource view
				ID3D11ShaderResourceView* diffuseMapSRV = texMgr.CreateTexture(texturePath + strPathStr);
				DiffuseMapSRV[i] = diffuseMapSRV;
			}
		}

		texCount = material->GetTextureCount(aiTextureType_NORMALS);

		for (texIndex = 0; texIndex < texCount; texIndex++)
		{
			//if the material has a normal map...
			if (material->GetTexture(aiTextureType_NORMALS, texIndex, &path) == AI_SUCCESS)
			{
				//copy over path in string format
				const char* cPathStr = path.C_Str();
				std::string  strPathStr(cPathStr);
				//create texture and store the Shader Resource view
				ID3D11ShaderResourceView* normalMapSRV = texMgr.CreateTexture(texturePath + strPathStr);
				NormalMapSRV[i] = normalMapSRV;
			}
		}
		
	}
}

DPhoenix::BasicModel::~BasicModel()
{
}

DPhoenix::BasicModelInstance::BasicModelInstance()
{
	//default values for all properties
	//note scale must be 1! Nothing will render at 0!
	mPosition.x = 0.0f; mPosition.y = 0.0f; mPosition.z = 0.0f;
	mVelocity.x = 0.0f; mVelocity.y = 0.0f; mVelocity.z = 0.0f;
	mRelativeVelocity.x = 0.0f; mRelativeVelocity.y = 0.0f; mRelativeVelocity.z = 0.0f;
	mScale.x = 1.0f; mScale.y = 1.0f; mScale.z = 1.0f;
	mRotation.x = 0.0f; mRotation.y = 0.0f; mRotation.z = 0.0f;
	mForwardVector.x = 0.0f; mForwardVector.y = 0.0f; mForwardVector.z = 1.0f;
	mRightVector.x = 1.0f; mRightVector.y = 0.0f; mRightVector.z = 0.0f;

	mOpacity = 1.0f;

}

//Update method; handle movement based on velocity and delta time
void DPhoenix::BasicModelInstance::Update(float deltaTime, bool moveRelative)
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
}

//apply transformations and output World matrix
//(to convert from object space to world space)
XMMATRIX DPhoenix::BasicModelInstance::CalculateTransforms()
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

	//return the matrix so it can be used with the shader
	return World;
}

void DPhoenix::BasicModelInstance::SetFacingRotation(float offsetDeg)
{
	// Calculate the rotation that needs to be applied to the 
	//model to face the forward vector
	float angle = atan2( -mForwardVector.x, -mForwardVector.z) * (180.0 / XM_PI);

	// Convert rotation into radians 
	//(offset is there in case models face odd directions from origin).
	float rotation = (float)(angle + offsetDeg) * 0.0174532925f;

	mRotation.y = rotation; 
}

DPhoenix::BasicModelInstance::~BasicModelInstance()
{ }

