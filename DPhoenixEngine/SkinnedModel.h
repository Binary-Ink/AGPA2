#pragma once
#include "D3DUtil.h"

//adapted from RichardsSoftware
//http://www.richardssoftware.net/Home/Post/34

namespace DPhoenix
{

	//this is a copy from the vertex namespace for convenience
	//dependency issues from header files led to this solution
	struct PosNormalTexTanSkinned
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT4 TangentU;
		XMFLOAT3 Weights;
		BYTE BoneIndices[4];
	};

	//this represents each bone / 
	//node in the skeleton hierarchy
	class Bone
	{
	public:
		std::string Name;
		//Bind space transform
		aiMatrix4x4 Offset;
		//local matrix transform
		aiMatrix4x4 LocalTransform;
		//To-root transform
		aiMatrix4x4 GlobalTransform;
		//Copy of the original local transform
		aiMatrix4x4 OriginalLocalTransform;
		//parent bone reference
		Bone* Parent;
		//child bone references
		std::vector<Bone*> Children;
		//is this a bone (or just node)?
		bool isBone = false;

		Bone() { }
	};

	//for skinning - each vertex will have
	//a set of bones it is weighted by
	//shoudl be max 4 and all add up to 1
	struct VertexWeight
	{
		UINT boneIndex;
		float weight;
	};

	//For each animation (channel) over time
	//there will be a set of keys to scale, position, rotate etc.
	//this holds them and keeps the animation name also
	class AnimationChannel
	{
	public:
		std::string Name;
		std::vector<aiVectorKey> PositionKeys;
		std::vector<aiQuatKey> RotationKeys;
		std::vector<aiVectorKey> ScalingKeys;
	};

	//Animation Evaluator - handles skinned / skeletal animations
	//and interpolations
	class AnimEvaluator
	{
	private:
		//how many animations?
		std::vector<AnimationChannel> Channels;
		//what was the last time point the animation was at?
		float LastTime;
		//set of indexes for pos / scale / rot keys in animation
		//used to interpolate beetween the animations
		//(new STL structure!)
		std::vector<std::tuple<int, int, int>> LastPositions;
	public:
		//animation name
		std::string Name;
		bool PlayAnimationForward;	//play forward?
		float TicksPerSecond;		//animation rate per second
		float Duration;				//how long the animation cycle is
		//2D STL vector of matrices - for each bone, what are the
		//transformation matrices for them?  (a set per animation 'frame')
		std::vector<std::vector<aiMatrix4x4>> Transforms;
		//constructor - takes in assimp animation class
		AnimEvaluator(aiAnimation* anim);
		//this constructs the full animation to store in memory
		void Evaluate(float dt, std::map<std::string, Bone*>& bones);
		//get bone transforms 
		std::vector<aiMatrix4x4> GetTransforms(float dt);
		//get the frame at a given index
		int GetFrameIndexAt(float dt);
	};

	class SceneAnimator
	{
	private:
		//helper structures
		//the root node of the skeleton
		Bone* _skeleton;
		//a map to match names with Bone structs
		std::map<std::string, Bone*> _bonesByName;
		//a map to match bone names with their index
		std::map<std::string, int> _bonesToIndex;
		//a map to match an animation name with it's id
		std::map<std::string, int> _animationNameToId;
		//what is the current animation?
		int CurrentAnimationIndex;
		int _i; //for dummy name iterator
		//create the bone hierarchy
		Bone* CreateBoneTree(aiNode* node, Bone* parent);
		//this concatenates the transforms in bone space up the tree
		static void CalculateBoneToWorldTransform(Bone* child);
		//get the animations from the assimp model import
		void ExtractAnimations(const aiScene* scene);
		//calculate the current transforms interpolated between frames
		void Calculate(float dt);
		//update the bone hierarchy transforms
		static void UpdateTransforms(Bone* node);

	public:
		//list of the animations and bones
		std::vector<AnimEvaluator> Animations;
		std::vector<Bone*> _bones;
		//quick evaluator for checking is 'skinned'
		bool HasSkeleton()
		{
			return _bones.size() > 0;
		}

		//constructor
		SceneAnimator();
		//Initialises the skeleton and animations based on assimp scene
		void Init(const aiScene* scene);
		//get a bone id the name
		int GetBoneIndex(std::string name);
		//set an animation given the name 
		void SetAnimation(std::string name);
		//get transforms from bone hierarchy
		std::vector<aiMatrix4x4> GetTransforms(float dt);

		//NEW for multiple anims
		//this method assums we are using the same mesh / heirarchy but loading 
		//JUST the animation data
		void LoadNewAnimations(std::string filename);
		void ConstructAnimations();
		//set an animation given the index
		void SetAnimation(int index);
		//cycle to next animation
		void NextAnimation();
		void PrevAnimation();
	};

	//The model geometry for rendering
	class SkinnedModel
	{
	private:
		//this gets the bone indices and blend weights necessary for defining vertices
		void ExtractBoneWeightsFromMesh(aiMesh* mesh, std::map<UINT, 
			std::vector<VertexWeight>>& vertToBoneWeights);
		//this then constructs the vertex input layout from the mesh data
		void ExtractVertices(aiMesh* mesh, std::map<UINT, 
			std::vector<VertexWeight>>& vertToBoneWeights,
			bool flipTexY, std::vector<PosNormalTexTanSkinned>& _vertices);

	public:
		//each subset of the mesh
		std::vector<MeshGeometry::Subset> Subsets;
		//the vertices for rendering
		std::vector<PosNormalTexTanSkinned> Vertices;
		//the indices for rendering
		std::vector<USHORT> Indices;
		//a pointer to the animator class for the model
		SceneAnimator* Animator;
		//the mesh geometry itself
		MeshGeometry ModelMesh;
		//number of subsets
		int SubsetCount;
		//materials and shader resource views for mapping
		std::vector<Material> Mat;
		std::vector<ID3D11ShaderResourceView*> DiffuseMapSRV;
		std::vector<ID3D11ShaderResourceView*> NormalMapSRV;
		//constrcutor, takes in filename and texture path to extract everything
		SkinnedModel(ID3D11Device* device, TextureMgr& texMgr, const std::string& modelFilename,
			const std::string& texturePath, bool flipTexY = false);

	};

	//an instance of the model to ender - as entity for inherited values
	class SkinnedModelInstance : public Entity
	{
	private:
		//animation data
		float _timePos;								//current time position
		std::string _clipName;						//name of animation clip
		bool _loopClips;							//loop animations?
		std::vector<XMFLOAT4X4> _finalTransforms;	//a set of the final bone transforms for the shader
		std::queue<std::string> _clipQueue;			//clip queue for animations (so they don't cut into each other)

	public:

		float mOpacity;

		//pointer to the mesh data
		SkinnedModel* Model;
		//constructor
		SkinnedModelInstance(std::string clipName, 
							SkinnedModel* model);
		
		//getters and setters for animation clips
		std::string GetClipName() { return _clipName; };
		void SetClipName(std::string name);
		std::vector<std::string> GetClips();
		//getter and setter for looping animation
		bool GetLoopClips() { return _loopClips; }
		void SetLoopClips(bool value) { _loopClips = value; }
		//get the final bone transforms for the shader
		std::vector<XMFLOAT4X4> GetFinalTransforms();
		
		//main entity management for instances
		void Update(float deltaTime, bool moveRelative);
		XMMATRIX CalculateTransforms();		//world matrix
		void SetFacingRotation(float offsetDeg);

		//get bone transforms for skeleton
		std::vector<XMFLOAT4X4> GetBoneTransforms();
	};
}

