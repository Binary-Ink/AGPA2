#include "D3DUtil.h"

//here we begin - skinned model constructor, assimp loader and extractor
DPhoenix::SkinnedModel::SkinnedModel(ID3D11Device* device, TextureMgr& texMgr, 
	const std::string& modelFilename,
	const std::string& texturePath, bool flipTexY)
{
	//Assimp importer
	Assimp::Importer importer;
	//give the file and set flags (these contain lots of subflags that
	//tidy up the mesh overall)
	const aiScene* model = importer.ReadFile(modelFilename,
		aiProcessPreset_TargetRealtime_Quality
		| aiProcess_ConvertToLeftHanded
		| aiProcess_SplitByBoneCount);

	//Load animation data from the loaded assimp mesh
	Animator = new SceneAnimator();
	Animator->Init(model);

	//create our vertex-to-boneweights lookup
	std::map<UINT, std::vector<VertexWeight>> vertToBoneWeights;

	//loop through meshes (subsets) -------------------------------------------------------------------

	for (int i = 0; i < model->mNumMeshes; i++)
	{
		//pointer to mesh (easier to convert code)
		aiMesh* mesh = model->mMeshes[i];
		//extract the bone weights from the mesh to help construct vertices
		ExtractBoneWeightsFromMesh(mesh, vertToBoneWeights);
		//create the subset data based on the mesh data
		MeshGeometry::Subset subset;
		subset.VertexCount = mesh->mNumVertices;
		subset.VertexStart = Vertices.size();
		subset.FaceStart = Indices.size() / 3;
		subset.FaceCount = mesh->mNumFaces;
		subset.materialIndex = mesh->mMaterialIndex;
		//add the subset to the subset table
		Subsets.push_back(subset);

		//extract the vertices and add the bpone weights / bone indices
		ExtractVertices(mesh, vertToBoneWeights, flipTexY, Vertices);

		// extract indices and shift them to the proper offset into the 
		//combined vertex buffer
		for (int j = 0; j < mesh->mNumFaces; j++)
		{
			//poiner to the face data
			const aiFace& face = mesh->mFaces[j];
			for (int k = 0; k < face.mNumIndices; k++)
			{
				//need to add the right vertex offset so it refers to the correct
				//vertex related to the current mesh / subset
				Indices.push_back(face.mIndices[k] + subset.VertexStart);
			}
		}

	}
	//add data to model mesh class for rendering -----------------------------------------------
	ModelMesh.SetSubsetTable(Subsets);
	SubsetCount = Subsets.size();
	ModelMesh.SetVertices(device, &Vertices[0], Vertices.size());
	ModelMesh.SetIndices(device, &Indices[0], Indices.size());

	//get the number of materials for the mesh
	//(same loop from BasicModel.cpp with a couple adjusted names)
	int iNumMaterials = model->mNumMaterials;

	Mat.resize(iNumMaterials);
	DiffuseMapSRV.resize(iNumMaterials);
	NormalMapSRV.resize(iNumMaterials);

	//loop through materials
	for (int i = 0; i < iNumMaterials; i++)
	{
		//create pointer to material in assimp format
		const aiMaterial* material = model->mMaterials[i];
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
			if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) 
				== AI_SUCCESS)
			{
				//copy over path in string format
				const char* cPathStr = path.C_Str();
				std::string  strPathStr(cPathStr);
				//create texture and store the Shader Resource view
				ID3D11ShaderResourceView* diffuseMapSRV = 
					texMgr.CreateTexture(texturePath + strPathStr);
				DiffuseMapSRV[i] = diffuseMapSRV;
			}
		}

		texCount = material->GetTextureCount(aiTextureType_NORMALS);

		for (texIndex = 0; texIndex < texCount; texIndex++)
		{
			//if the material has a normal map...
			if (material->GetTexture(aiTextureType_NORMALS, texIndex, &path) 
				== AI_SUCCESS)
			{
				//copy over path in string format
				const char* cPathStr = path.C_Str();
				std::string  strPathStr(cPathStr);
				//create texture and store the Shader Resource view
				ID3D11ShaderResourceView* normalMapSRV = 
					texMgr.CreateTexture(texturePath + strPathStr);
				NormalMapSRV[i] = normalMapSRV;
			}
		}
	}
}

//here, per mesh, we get the bone weights that act on it - 
//one half of look up for vertex creation
void DPhoenix::SkinnedModel::ExtractBoneWeightsFromMesh(aiMesh* mesh,
	std::map<UINT, std::vector<VertexWeight>>& vertToBoneWeights)
{
	//loop through bones in the mesh
	for (int i = 0; i < mesh->mNumBones; i++)
	{
		//get bone class pointer
		aiBone* bone = mesh->mBones[i];

		//string conversion for bone name
		const char* cBoneStr = bone->mName.C_Str();
		std::string  boneName(cBoneStr);

		//bone names at this point should be fully populated so get the index it relates to
		int boneIndex = Animator->GetBoneIndex(boneName);
		// bone weights are recorded per bone in assimp, 
		//with each bone containing a list of the vertices influenced by it
		// we really want the reverse mapping, 
		//i.e. lookup the vertexID and get the bone id and weight
		// We'll support up to 4 bones per vertex, so we need a list of weights for each vertex
		for (int j = 0; j < bone->mNumWeights; j++)
		{
			//get the inidvidual weight
			aiVertexWeight weight = bone->mWeights[j];
			//now construct our local struct to hold it
			//containing bone index and the blend weight
			VertexWeight vertexWeight;
			vertexWeight.boneIndex = boneIndex;
			vertexWeight.weight = weight.mWeight;
			//add to our map - for each vertex we have a vector of VertexWeights
			//to then loop through to extract the informatin in the relevant method
			vertToBoneWeights[weight.mVertexId].push_back(vertexWeight);
		}
	}
}

//second half of our look-up to construct vertex data
void DPhoenix::SkinnedModel::ExtractVertices(aiMesh* mesh, std::map<UINT, 
	std::vector<VertexWeight>>& vertToBoneWeights,
	bool flipTexY, std::vector<PosNormalTexTanSkinned>& _vertices)
{
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		//get position / normals / tangents
		aiVector3D pos = mesh->mVertices[i];
		aiVector3D uv = mesh->mTextureCoords[0][i];
		aiVector3D normal = mesh->mNormals[i];
		aiVector3D tangent = mesh->mTangents[i];

		//now convert this data to our vertex input layout
		PosNormalTexTanSkinned vertex;
		vertex.Pos.x = pos.x; vertex.Pos.y = pos.y; vertex.Pos.z = pos.z;
		vertex.Tex.x = uv.x; vertex.Tex.y = uv.y;
		vertex.Normal.x = normal.x; vertex.Normal.y = normal.y; vertex.Normal.z = normal.z;
		vertex.TangentU.x = tangent.x; vertex.TangentU.y = tangent.y;
		if (flipTexY) { vertex.TangentU.y = -vertex.TangentU.y; } //flip uv if flag set
		vertex.TangentU.z = tangent.z; vertex.TangentU.w = 1.0f;

		//get the vector of weights / bone indices from given vertex id
		std::vector<VertexWeight> vWeights = vertToBoneWeights[i];

		//This is ridiculously inelegant but solves index exceptions
		//needs refactoring in a better manner
		if (vWeights.size() > 0)
		{
			vertex.BoneIndices[0] = (BYTE)(int)vWeights[0].boneIndex;
			vertex.Weights.x = vWeights[0].weight;
		}
		else
		{
			vertex.BoneIndices[0] = (BYTE)0;
			vertex.Weights.x = 0.0f;
		}

		if (vWeights.size() > 1)
		{
			vertex.BoneIndices[1] = (BYTE)vWeights[1].boneIndex;
			vertex.Weights.y = vWeights[1].weight;
		}
		else
		{
			vertex.BoneIndices[1] = (BYTE)0;
			vertex.Weights.y = 0.0f;
		}

		if (vWeights.size() > 2)
		{
			vertex.BoneIndices[2] = (BYTE)vWeights[2].boneIndex;
			vertex.Weights.z = vWeights[2].weight;
		}
		else
		{
			vertex.BoneIndices[2] = (BYTE)0;
			vertex.Weights.z = 0.0f;
		}

		//no fourth weight added as calculated in shader
		if (vWeights.size() > 3)
		{
			vertex.BoneIndices[3] = (BYTE)vWeights[3].boneIndex;
		}
		else
		{
			vertex.BoneIndices[3] = (BYTE)0;
		}


		//add to vertices vector
		_vertices.push_back(vertex);
	}
}

//scene animator constructor - initialises vars
DPhoenix::SceneAnimator::SceneAnimator()
{
	_skeleton = NULL;
	CurrentAnimationIndex = -1;
	_i = 0;
}

//called from the SkinnedModel constructor - gets animations and hierarchy
void DPhoenix::SceneAnimator::Init(const aiScene* scene)
{
	//if no animations, this isn't necessary 
	//(we'd assume animations though when using this class)
	if (!scene->HasAnimations())
	{
		return;
	}
	//get the root node and populate the hierarchy
	_skeleton = CreateBoneTree(scene->mRootNode, NULL);

	//loop through meshes and bones
	//(different subsets may relate to the same bones
	//so we need to do some checking while adding
	//to ensure no duplicate entries)
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++)
		{
			//get pointer to assimp bone
			aiBone* bone = scene->mMeshes[i]->mBones[j];

			//if we don't have this bone, we need it!
			Bone* found;
			std::string bName(bone->mName.C_Str());
			//name doesn't match in our hierarchy
			//skip to next iteration
			if (!_bonesByName.count(bName))
				continue;

			//if found, it is a bone in our tree!
			//assign bone details to 'found'
			_bonesByName[bName]->isBone = true;
			found = _bonesByName[bName];

			//check to see if this bone is already in the collection
			//if so then we skip the rest of this loop
			//otherwise we move on to the next bits and add the bone to our collection
			bool skip = false;
			for (int k = 0; k < _bones.size(); k++)
			{
				if (_bones[k]->Name == bName)
				{
					skip = true;
				}
			}

			if (skip) continue;

			//we need to offset matrix (transposed for DX rendering)
			//to calculate bone position in bone space
			found->Offset = bone->mOffsetMatrix.Transpose();
			//add to vector and update our name - index lookup
			_bones.push_back(found);
			_bonesToIndex[found->Name] = _bones.size() - 1;
		}
	}

	//where there are nodes that aren't bones
	//they still need to be added to the hierarchy for transforms
	std::string rootName(scene->mRootNode->mName.C_Str());
	//this odd notation is how you loop through a map structure
	for (const auto &node : _bonesByName)
	{
		//check to see if in bone vector
		bool isAlreadyBoned = false;
		//loop through bones vector
		for (int k = 0; k < _bones.size(); k++)
		{
			//if the current node matches a bone name
			//it is already in
			if (node.first == _bones[k]->Name)
			{
				isAlreadyBoned = true;
			}
		}
		//if not in the hierarchy, need to add the node
		if (!isAlreadyBoned)
		{
			//get parent offset if not a bone with its own
			if (node.first != rootName)
			{
				node.second->Offset = node.second->Parent->Offset;
			}

			//add the node data to our bones and update our name - index lookup
			_bones.push_back(node.second);
			_bonesToIndex[node.second->Name] = _bones.size() - 1;
		}
	}

	//extract the animation data 
	ExtractAnimations(scene);

	//ConstructAnimations();

	
}

//recursively construct node hierarchy for skeleton
DPhoenix::Bone* DPhoenix::SceneAnimator::CreateBoneTree(aiNode* node, Bone* parent)
{
	Bone* internalNode = new Bone();
	//string conversion for bone name
	std::string name(node->mName.C_Str());
	internalNode->Name = name;
	//store parent pointer
	internalNode->Parent = parent;
	//add dummy name if bone unnamed to prevent awfulness
	if (name == "")
	{
		std::ostringstream stream;
		stream << "foo" << _i++;
		name = stream.str();
		internalNode->Name = name;
	}

	//add to the map the curent node by name
	_bonesByName[name] = internalNode;
	//store the transformation matrix (transposed for DX)
	aiMatrix4x4 trans = node->mTransformation;
	trans = trans.Transpose();
	internalNode->LocalTransform = trans;
	//store the original local transform (that might be updated by animations)
	internalNode->OriginalLocalTransform = internalNode->LocalTransform;
	//here we calculate the combined matrices in bonespace up to parents
	//(recursive traversal)
	CalculateBoneToWorldTransform(internalNode);

	//create bone tree recursive transformations
	for (int i = 0; i < node->mNumChildren; i++) {
		//recursive call to update child data
		Bone* child = CreateBoneTree(node->mChildren[i], internalNode);
		//if child discovered we push ths back to our vector of child bones
		if (child != NULL) {
			internalNode->Children.push_back(child);
		}
	}
	//return our updated node
	return internalNode;
}

//this constructs the combined transformation matrices for each node
void DPhoenix::SceneAnimator::CalculateBoneToWorldTransform(Bone* child)
{
	//we begin with our local transform
	child->GlobalTransform = child->LocalTransform;
	//get parent data
	Bone* parent = child->Parent;
	//loop thrpough parent - to -parent until at root
	while (parent != NULL) {
		//concatenate parent transforms
		child->GlobalTransform *= parent->LocalTransform;
		parent = parent->Parent;
	}
}

//extract animations from the assimp scene
void DPhoenix::SceneAnimator::ExtractAnimations(const aiScene* scene)
{
	//for each animation in the scene
	//construct a new animation evaluator and store in Animations
	for (int i = 0; i < scene->mNumAnimations; i++)
	{
		Animations.push_back(AnimEvaluator(scene->mAnimations[i]));
	}
	//now map the animation names to ids
	for (int i = 0; i < Animations.size(); i++)
	{
		_animationNameToId[Animations[i].Name] = i;
	}
	//set the current index to 0
	CurrentAnimationIndex = 0;
}

//retireives the bone index for a given name
int DPhoenix::SceneAnimator::GetBoneIndex(std::string name)
{
	//if one is there, return the index otherwise -1
	if (_bonesToIndex.count(name)) {

		return _bonesToIndex[name];
	}
	return -1;
}

//calculates the transforms at a given time position
void DPhoenix::SceneAnimator::Calculate(float dt)
{
	//if we do not have a suitable current animation index, return
	if ((CurrentAnimationIndex < 0) || (CurrentAnimationIndex >= Animations.size()))
	{
		return;
	}
	//call the evaluate method for the current animation
	//giving the time position and the bones map
	Animations[CurrentAnimationIndex].Evaluate(dt, _bonesByName);
	//update the transforms for the skeleton following animation evaluation
	UpdateTransforms(_skeleton);
}

//this updates the transforms throughout the skeletion
void DPhoenix::SceneAnimator::UpdateTransforms(Bone* node)
{
	//calculate the bone transforms and recursively do so
	//for all children
	CalculateBoneToWorldTransform(node);
	for (int i = 0; i < node->Children.size(); i++)
	{
		UpdateTransforms(node->Children[i]);
	}
}

//constructor for animation evaluator
DPhoenix::AnimEvaluator::AnimEvaluator(aiAnimation* anim)
{
	//the initial 'last time' for interpolations is the beginning (0)
	LastTime = 0.0f;
	//if the ticks per second is greater than 0 we get it stored, otherwise we assume 920
	TicksPerSecond = anim->mTicksPerSecond > 0.0f ? (float)anim->mTicksPerSecond : 920.0f;
	//how long is the animation?
	Duration = (float)anim->mDuration;
	//get teh name of the animation
	std::string animName(anim->mName.C_Str());
	Name = animName;
	//loop through the animation channels (i.e. affected bones)
	for (int i = 0; i < anim->mNumChannels; i++)
	{
		//get the channel
		aiNodeAnim* channel = anim->mChannels[i];
		//create new Animation Channel object
		AnimationChannel c = AnimationChannel();
		//store the channel name (usually related to bone name)
		std::string chanName(channel->mNodeName.C_Str());
		c.Name = chanName;
		//loop through the position keys
		for (int j = 0; j < channel->mNumPositionKeys; j++)
		{
			//push back the positon, scale and rotation value keys
			c.PositionKeys.push_back(channel->mPositionKeys[j]);
			c.ScalingKeys.push_back(channel->mScalingKeys[j]);
			c.RotationKeys.push_back(channel->mRotationKeys[j]);
		}
		//push back the channel data
		Channels.push_back(c);
	}
	//default play animation forward
	PlayAnimationForward = true;
	//the last positions vector need to match the channels size
	LastPositions.resize(Channels.size());
}

//evaluate the animation given a specific time position
//thsi is where we interpolate the movements to animate
void DPhoenix::AnimEvaluator::Evaluate(float dt, std::map<std::string, Bone*>& bones)
{
	//multiply the time position by the ticks per second
	dt *= TicksPerSecond;
	float time = 0.0f;

	if (Duration > 0.0f)
	{
		//float modulo operation time = fd % Duration
		//so what time point 'within' the duration are we?
		time = fmod(dt, Duration);
	}
	//loop through animation channels
	for (int i = 0; i < Channels.size(); i++)
	{
		//create animation channel object
		AnimationChannel channel = Channels[i];
		//if we can't find the bone / channel name however
		//skip to next iteration
		if (!bones.count(channel.Name))
		{
			//did not find the bone node
			continue;
		}

		//interpolate position keyframes
		aiVector3D pPosition = aiVector3D();
		//assuming we have position keys...
		if (channel.PositionKeys.size() > 0)
		{
			//init frame to then calculate
			int frame = 0;
			//check our last positions (to interpolate from)
			if (LastPositions.size() > 0)
			{
				//if the time was greater than or equal to the last time
				//we want to get the last position key from LastPositions
				//otherwise 0 (as we haven't advanced the time)
				frame = (time >= LastTime) ? std::get<0>(LastPositions[i]) : 0;
			}
			//as long as the frame is less than the end of the position keys
			while (frame < channel.PositionKeys.size() - 1)
			{
				//if the current time is less than next frames 
				//time then break, otherwise increase frame number
				if (time < channel.PositionKeys[frame + 1].mTime)
				{
					break;
				}
				frame++;
			}
			//if the frame is greater than the position key size we reset to beginning
			if (frame >= channel.PositionKeys.size())
			{
				frame = 0;
			}
			//calculate the next frame, 0 if at end hence modulo
			int nextFrame = (frame + 1) % channel.PositionKeys.size();

			//get the current and next position keys
			aiVectorKey key = channel.PositionKeys[frame];
			aiVectorKey nextKey = channel.PositionKeys[nextFrame];
			//get the time difference between them
			float diffTime = nextKey.mTime - key.mTime;
			//if the time difference is less than 0 then compensate 
			if (diffTime < 0.0)
			{
				diffTime += Duration;
			}
			//assuming a time difference exists
			if (diffTime > 0.0)
			{
				//interpolation calc; this gets the value to interpolate between 0 and 1
				//based on current time, minus current key time and divided by 
				//time difference betwen frames
				float factor = (float)((time - key.mTime) / diffTime);
				//the actual position at this point is then somewhere between the next position
				//and the previous position key - the multiplication by factor is the interpolation / tween
				pPosition = key.mValue + (nextKey.mValue - key.mValue) * factor;
			}
			else
			{
				//if exactly 0 we can just set the current key value
				pPosition = key.mValue;
			}
			//the current frame is stored in LastPositions (get<0> is the position key)
			std::get<0>(LastPositions[i]) = frame;
		}

		//interpolate rotation keyframes
		aiQuaternion pRot = aiQuaternion(1.0f, 0.0f, 0.0f, 0.0f);
		//assuming we have rotation keys...
		if (channel.RotationKeys.size() > 0)
		{
			//if the time was greater than or equal to the last time
			//we want to get the last rotation key from LastPositions
			//otherwise 0 (as we haven't advanced the time)
			int frame = (time >= LastTime) ? std::get<1>(LastPositions[i]) : 0;
			//as long as the frame is less than the end of the rotation keys
			while (frame < channel.RotationKeys.size() - 1)
			{
				//if the current time is less than next frames 
				//time then break, otherwise increase frame number
				if (time < channel.RotationKeys[frame + 1].mTime)
				{
					break;
				}
				frame++;
			}
			//if the frame is greater than the rotation key size we reset to beginning
			if (frame >= channel.RotationKeys.size())
			{
				frame = 0;
			}
			//calculate the next frame, 0 if at end hence modulo
			int nextFrame = (frame + 1) % channel.RotationKeys.size();

			//get the current and next rotation keys
			aiQuatKey key = channel.RotationKeys[frame];
			aiQuatKey nextKey = channel.RotationKeys[nextFrame];
			//normalize the quaternions before any interpolations
			key.mValue = key.mValue.Normalize();
			nextKey.mValue = nextKey.mValue.Normalize();
			//get the time difference between them
			float diffTime = nextKey.mTime - key.mTime;
			//if the time difference is less than 0 then compensate 
			if (diffTime < 0.0)
			{
				diffTime += Duration;
			}
			//assuming a time difference exists
			if (diffTime > 0.0)
			{
				//interpolation calc; this gets the value to interpolate between 0 and 1
				//based on current time, minus current key time and divided by 
				//time difference betwen frames
				float factor = (float)((time - key.mTime) / diffTime);
				//slerping :) - this calculates the spherical interpolation based on the factor given
				pRot.Interpolate(pRot, key.mValue, nextKey.mValue, factor);
			}
			else
			{
				//if exactly 0 we can just set the current key value
				pRot = key.mValue;
			}
			//the current frame is stored in LastPositions (get<1> is the rotation key)
			std::get<1>(LastPositions[i]) = frame;
		}

		//interpolate scale keyframes
		aiVector3D pScale = aiVector3D();
		//assuming we have scaling keys...
		if (channel.ScalingKeys.size() > 0)
		{
			//if the time was greater than or equal to the last time
			//we want to get the last scaling key from LastPositions
			//otherwise 0 (as we haven't advanced the time)
			int frame = (time >= LastTime) ? std::get<2>(LastPositions[i]) : 0;
			//as long as the frame is less than the end of the scaling keys
			while (frame < channel.ScalingKeys.size() - 1)
			{
				//if the current time is less than next frames 
				//time then break, otherwise increase frame number
				if (time < channel.ScalingKeys[frame + 1].mTime)
				{
					break;
				}
				frame++;
			}
			//if the frame is greater than the scaling key size we reset to beginning
			if (frame >= channel.ScalingKeys.size())
			{
				frame = 0;
			}
			//calculate the next frame, 0 if at end hence modulo
			int nextFrame = (frame + 1) % channel.ScalingKeys.size();
			//get the current and next scaling keys
			aiVectorKey key = channel.ScalingKeys[frame];
			aiVectorKey nextKey = channel.ScalingKeys[nextFrame];
			//get the time difference between them
			float diffTime = nextKey.mTime - key.mTime;
			//if the time difference is less than 0 then compensate 
			if (diffTime < 0.0)
			{
				diffTime += Duration;
			}
			//assuming a time difference exists
			if (diffTime > 0.0)
			{
				//interpolation calc; this gets the value to interpolate between 0 and 1
				//based on current time, minus current key time and divided by 
				//time difference betwen frames
				float factor = (float)((time - key.mTime) / diffTime);
				//the actual scale at this point is then somewhere between the next position
				//and the previous position key - the multiplication by factor is the interpolation / tween
				pScale = key.mValue + (nextKey.mValue - key.mValue) * factor;
			}
			else
			{
				//if exactly 0 we can just set the current key value
				pScale = key.mValue;
			}
			//the current frame is stored in LastPositions (get<2> is the scale key)
			std::get<2>(LastPositions[i]) = frame;
		}

		//create the combined transformation matrix
		//first get XMFLOAT structures
		XMFLOAT3 scaleVec; scaleVec.x = pScale.x;  scaleVec.y = pScale.y;  scaleVec.z = pScale.z;
		XMFLOAT3 posVec; posVec.x = pPosition.x;  posVec.y = pPosition.y;  posVec.z = pPosition.z;
		XMFLOAT4 quatVec; quatVec.x = pRot.x;  quatVec.y = pRot.y;  
			quatVec.z = pRot.z; quatVec.w = pRot.w;

		//convert to vector format
		XMVECTOR S = XMLoadFloat3(&scaleVec);
		XMVECTOR P = XMLoadFloat3(&posVec);
		XMVECTOR Q = XMLoadFloat4(&quatVec);
		//zero matrix for rotation origin
		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		//here we create the 4X4 matrix object using the transformation values given
		XMFLOAT4X4 getMatrix;
		XMStoreFloat4x4(&getMatrix, XMMatrixAffineTransformation(S, zero, Q, P));

		//transpose to get DirectX style matrix
		aiMatrix4x4 mat;
		//(this is manually transposed and converted between matrix formats - 
		//busy work but one way of doing it)
		mat.a1 = getMatrix._11; mat.a2 = getMatrix._12; 
			mat.a3 = getMatrix._13; mat.a4 = getMatrix._14;
		mat.b1 = getMatrix._21; mat.b2 = getMatrix._22; 
			mat.b3 = getMatrix._23; mat.b4 = getMatrix._24;
		mat.c1 = getMatrix._31; mat.c2 = getMatrix._32; 
			mat.c3 = getMatrix._33; mat.c4 = getMatrix._34;
		mat.d1 = getMatrix._41; mat.d2 = getMatrix._42; 
			mat.d3 = getMatrix._43; mat.d4 = getMatrix._44;
		//set the updated transformation matrix to the bone
		bones[channel.Name]->LocalTransform = mat;
	}
	//set the last time position for the next time round
	LastTime = time;
}

//get transforms based on time position
std::vector<aiMatrix4x4> DPhoenix::SceneAnimator::GetTransforms(float dt)
{
	//call get transforms for the current animation (anim evaluator)
	return Animations[CurrentAnimationIndex].GetTransforms(dt);
}

//get transforms at specific time point
std::vector<aiMatrix4x4> DPhoenix::AnimEvaluator::GetTransforms(float dt)
{
	//return the set of transforms based on index current time position
	//relates to
	return Transforms[GetFrameIndexAt(dt)];
}

//this maps the current time position to the nearest 'tick' / 'keyframe'
//of the animation
int DPhoenix::AnimEvaluator::GetFrameIndexAt(float dt)
{
	//multiply time position by the ticks per second
	dt *= TicksPerSecond;
	float time = 0.0f;
	//if a duration exists
	if (Duration > 0.0f)
	{
		//then figure out the time as it maps
		//within the duration (fmod = float modulo)
		time = fmod(dt, Duration);
	}
	//the percentage calc gives where the current time is
	//in the animation as a percent overall
	float percent = time / Duration;
	//if we're not playing forward...
	if (!PlayAnimationForward)
	{
		//then we need to reverse the percentage
		//(e.g. 0.7 becomes 0.3)
		percent = (percent - 1.0f) * -1.0f;
	}
	//the frame index is the nearest (from int truncation)
	//based on percent of overall transforms size
	//(keyframes over the duration at 30fps)
	int frameIndexAt = (int)(Transforms.size() * percent);
	//return the frame index
	return frameIndexAt;
}

//sets animation index based on name given
void DPhoenix::SceneAnimator::SetAnimation(std::string name)
{
	if (_animationNameToId.count(name))
	{
		CurrentAnimationIndex = _animationNameToId[name];
	}
	else
	{
		CurrentAnimationIndex = -1;
	}
}

//sets animation based on 'clip' name given - some checking to ensure it is valid
void DPhoenix::SkinnedModelInstance::SetClipName(std::string name)
{
	bool hasAnimation = false;
	for (int i = 0; i < Model->Animator->Animations.size(); i++)
	{
		if (Model->Animator->Animations[i].Name == name)
		{
			hasAnimation = true;
			_clipName = name;
		}
	}
	if (!hasAnimation)
	{
		_clipName = "Still";
	}
	Model->Animator->SetAnimation(_clipName);
	_timePos = 0;

}
//returns a vector of strings containing each clip name
std::vector<std::string> DPhoenix::SkinnedModelInstance::GetClips()
{
	std::vector<std::string> clipNames;
	for (int i = 0; i < Model->Animator->Animations.size(); i++)
	{
		clipNames.push_back(Model->Animator->Animations[i].Name);
	}
	return clipNames;
}

//constructor for instance; initialises vars including model to render and animation clip
DPhoenix::SkinnedModelInstance::SkinnedModelInstance(std::string clipName,
			SkinnedModel* model)
{
	Model = model;
	SetClipName(clipName);
	_loopClips = true;

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

//update animation and movement
void DPhoenix::SkinnedModelInstance::Update(float deltaTime, bool moveRelative) {
	
	//animation updating
	_timePos += deltaTime;
	GetFinalTransforms();
	
	//queueing of animation clips could go here
	//(Advanced - can adapt from given sources)	

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

//get the final transforms for the model / shader to render
std::vector<XMFLOAT4X4> DPhoenix::SkinnedModelInstance::GetFinalTransforms()
{
	//based on current time position, get teh transforms for the skeleton / keyframe
	std::vector<aiMatrix4x4> animTransforms = Model->Animator->GetTransforms(_timePos);
	std::vector<XMFLOAT4X4> convertedTransforms;

	//loop through and convert the matrix format
	for (int i = 0; i < animTransforms.size(); i++)
	{
		XMFLOAT4X4 convertedMat = XMFLOAT4X4(&animTransforms[i].a1);
		convertedTransforms.push_back(convertedMat);

	}
	//return the vector of the matrices
	_finalTransforms = convertedTransforms;
	return convertedTransforms;
}

//this should get the bone transforms in bind pose but doesn't quite work
//Advanced task - fix it!
std::vector<XMFLOAT4X4> DPhoenix::SkinnedModelInstance::GetBoneTransforms()
{
	std::vector<aiMatrix4x4> bindTransforms;

	for (int i = 0; i < Model->Animator->_bones.size(); i++)
	{
		bindTransforms.push_back(Model->Animator->_bones[i]->GlobalTransform);
	}

	std::vector<XMFLOAT4X4> convertedTransforms;

	for (int i = 0; i < bindTransforms.size(); i++)
	{
		XMFLOAT4X4 convertedMat = XMFLOAT4X4(&bindTransforms[i].a1);
		convertedTransforms.push_back(convertedMat);
	}
	_finalTransforms = convertedTransforms;
	return convertedTransforms;
}

//apply transformations and output World matrix
//(to convert from object space to world space)
XMMATRIX DPhoenix::SkinnedModelInstance::CalculateTransforms()
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

void DPhoenix::SkinnedModelInstance::SetFacingRotation(float offsetDeg)
{
	// Calculate the rotation that needs to be applied to the 
	//model to face the forward vector
	float angle = atan2(-mForwardVector.x, -mForwardVector.z) * (180.0 / XM_PI);

	// Convert rotation into radians 
	//(offset is there in case models face odd directions from origin).
	float rotation = (float)(angle + offsetDeg) * 0.0174532925f;

	mRotation.y = rotation;
}

//FOR EXTRACTING JUST ANIMATION DATA AND ADDING TO EXISTING INSTANCE ==================================

void DPhoenix::SceneAnimator::LoadNewAnimations(std::string filename)
{
	//Assimp importer
	Assimp::Importer importer;
	//give the file and set flags (these contain lots of subflags that
	//tidy up the mesh overall)
	const aiScene* model = importer.ReadFile(filename,
		aiProcessPreset_TargetRealtime_Quality
		| aiProcess_ConvertToLeftHanded
		| aiProcess_SplitByBoneCount);

	//Load animation data from the loaded assimp mesh
	ExtractAnimations(model);
}

//SceneAnimator Updates
void DPhoenix::SceneAnimator::ConstructAnimations()
{
	//here we set our timestep as 30 fps
	const float timestep = 1.0f / 30.0f;
	//loop through animations
	for (int i = 0; i < Animations.size(); i++)
	{
		//set current animation index
		CurrentAnimationIndex = i;
		//delta time at 0 to begin with
		float dt = 0.0f;
		//an animation is defined in 'ticks' - for each 'tick' we'll interpolate the position at 30 fps
		//and store the transforms at that tick point so we have a set of interpolated data from  
		//the model for the bones.  Essentially these rae key frames we'll work out 
		//the transformations between when updating
		for (float ticks = 0.0f; ticks < Animations[i].Duration;
			ticks += Animations[i].TicksPerSecond / 30.0f)
		{
			//advance the delta time by the timestep
			dt += timestep;
			//calculate the transforms at this point in the animation
			Calculate(dt);
			//transformation matrix vector
			std::vector<aiMatrix4x4> trans;
			//loop through all bones
			for (int a = 0; a < _bones.size(); a++)
			{
				//bone matrix - if a bone then multiply by offset
				//otherwise just get the global transform
				aiMatrix4x4 boneMat;
				if (_bones[a]->isBone)
					boneMat = _bones[a]->Offset * _bones[a]->GlobalTransform;
				else
					boneMat = _bones[a]->GlobalTransform;
				//add to the transformations vector
				trans.push_back(boneMat);
			}
			//for this animation, push back the set of transforms for this tick / keyframe
			Animations[i].Transforms.push_back(trans);
		}
	}
}

void DPhoenix::SceneAnimator::SetAnimation(int index)
{
	CurrentAnimationIndex = index;
}

void DPhoenix::SceneAnimator::NextAnimation()
{
	CurrentAnimationIndex++;
	if (CurrentAnimationIndex == Animations.size())
		CurrentAnimationIndex = 0;
}

void DPhoenix::SceneAnimator::PrevAnimation()
{
	CurrentAnimationIndex--;
	if (CurrentAnimationIndex < 0)
		CurrentAnimationIndex = Animations.size() - 1;
}

