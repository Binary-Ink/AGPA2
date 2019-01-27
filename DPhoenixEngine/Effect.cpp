#include "Effect.h"

namespace DPhoenix
{

//Effect class sets up ability to compile shaders and then use them
//within the code for rendering
#pragma region Effect
	Effect::Effect(ID3D11Device* device, const std::string& filename)
		: mFX(0)
	{
		DWORD shaderFlags = 0;
		//yeah it's D3D10, they just didn't update this part
		//in DX11
		#if defined(DEBUG) || defined(_DEBUG)
				shaderFlags |= D3D10_SHADER_DEBUG;
				shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
		#endif

		//THE BLOB!!!! (data for shader compilation)
		ID3D10Blob* compiledShader = 0;
		ID3D10Blob* compilationMsgs = 0;

		//load this particular shader (generic function should be easy)
		HRESULT hr = D3DX11CompileFromFile(
						filename.c_str(),		//filename
						0,						//array of macro definitions
						0,						//any included files
						0,						//any entry point for shader file
						"fx_5_0",				//FX / HLSL version
						shaderFlags,			//any shader flags (see above)
						0,						//additional effect compile flags
						0,						//thread pump
						&compiledShader,		//output pointer to compiled shader	
						&compilationMsgs,		//output pointer to any compiation messages
						0);						//output pointer to return value

		// compilationMsgs can store errors or warnings.
		//hence message if there are any
		if (compilationMsgs != 0)
		{
			MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationMsgs);
		}

		// Even if there are no compilationMsgs, check to make sure there
		// were no other errors.
		if (FAILED(hr))
		{
			DXTrace(__FILE__, (DWORD)__LINE__, hr,
				"D3DX11CompileFromFile", true);
		}

		//mFX wil store the compiled effect
		HR(D3DX11CreateEffectFromMemory(
			compiledShader->GetBufferPointer(),
			compiledShader->GetBufferSize(),
			0, device, &mFX));

		// Done with compiled shader.
		ReleaseCOM(compiledShader);
	}

	Effect::~Effect()
	{
		ReleaseCOM(mFX);
	}
#pragma endregion

//this defines the SpriteEffect class
#pragma region SpriteEffect
	//constructor - get device and filename (call parent constructor first)
	SpriteEffect::SpriteEffect(ID3D11Device* device, const std::string& filename)
		: Effect(device, filename)
	{
		//set the SpriteTech to the shader technique
		SpriteTech = mFX->GetTechniqueByName("Sprite");

		//link Effect variables to shader variables (for get / set)
		//Matrices
		WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
		World = mFX->GetVariableByName("gWorld")->AsMatrix();
		//asprite texture (shader resource view)
		SpriteTex = mFX->GetVariableByName("gSpriteTex")->AsShaderResource();
		//float values
		Column = mFX->GetVariableByName("gColumn")->AsScalar();
		NumCols = mFX->GetVariableByName("gNumCols")->AsScalar();
		FlipValue = mFX->GetVariableByName("gFlipValue")->AsScalar();
		LerpValue = mFX->GetVariableByName("gLerpValue")->AsScalar();
		OpacityValue = mFX->GetVariableByName("gOpacityValue")->AsScalar();
		//vector (color) value
		AltColorValue = mFX->GetVariableByName("gAltColor")->AsVector();
	}

	SpriteEffect::~SpriteEffect()
	{
	}
#pragma endregion

//this defines the ColorEffect class
#pragma region ColorEffect
	//constructor - get device and filename (call parent constructor first)
	ColorEffect::ColorEffect(ID3D11Device* device, const std::string& filename)
		: Effect(device, filename)
	{
		//set the ColorTech to the shader technique
		ColorTech = mFX->GetTechniqueByName("ColorTech");

		//link Effect variables to shader variables (for get / set)
		//Matrices
		WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();

		OpacityValue = mFX->GetVariableByName("gOpacityValue")->AsScalar();
	}

	ColorEffect::~ColorEffect()
	{
	}
#pragma endregion

//this defines the BasicTextureEffect class
#pragma region BasicTextureEffect
	//constructor - get device and filename (call parent constructor first)
	BasicTextureEffect::BasicTextureEffect(ID3D11Device* device, const std::string& filename)
		: Effect(device, filename)
	{
		//set the ColorTech to the shader technique
		BasicTexTech = mFX->GetTechniqueByName("BasicTexTech");

		//link Effect variables to shader variables (for get / set)
		//Matrices
		WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
		//texture map
		DiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
		//ambient lighting values
		AmbientIntensity = mFX->GetVariableByName("gAmbientIntensity")->AsScalar();
		AmbientColor = mFX->GetVariableByName("gAmbientColor")->AsVector();
		//texture scaling values
		TexScaleU = mFX->GetVariableByName("gTexScaleU")->AsScalar();
		TexScaleV = mFX->GetVariableByName("gTexScaleV")->AsScalar();

		OpacityValue = mFX->GetVariableByName("gOpacityValue")->AsScalar();
	}

	BasicTextureEffect::~BasicTextureEffect()
	{
	}
#pragma endregion

//this defines the LitTextureEffect class
#pragma region LitTextureEffect
	//constructor - get device and filename (call parent constructor first)
	LitTextureEffect::LitTextureEffect(ID3D11Device* device, const std::string& filename)
		: Effect(device, filename)
	{
		//for skinned meshes
		LitTexSkinnedTech = mFX->GetTechniqueByName("LitTexSkinnedTech");
		BoneTransforms = mFX->GetVariableByName("gBoneTransforms")->AsMatrix();

		//set the LitTexTech to the shader technique
		LitTexTech = mFX->GetTechniqueByName("LitTexTech");
		//link Effect variables to shader variables (for get / set)
		//Matrices
		WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
		World = mFX->GetVariableByName("gWorld")->AsMatrix();
		WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
		//Maps / materials
		DiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
		Mat = mFX->GetVariableByName("gMaterial");
		//fog stuff
		EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
		FogColor = mFX->GetVariableByName("gFogColor")->AsVector();
		FogStart = mFX->GetVariableByName("gFogStart")->AsScalar();
		FogRange = mFX->GetVariableByName("gFogRange")->AsScalar();
		FogEnabled = mFX->GetVariableByName("gFogEnabled");
		//lighting arrays and counts
		DirLights = mFX->GetVariableByName("gDirLights");
		DirLightCount = mFX->GetVariableByName("gDirLightCount")->AsScalar();
		PointLights = mFX->GetVariableByName("gPointLights");
		PointLightCount = mFX->GetVariableByName("gPointLightCount")->AsScalar();
		SpotLights = mFX->GetVariableByName("gSpotLights");
		SpotLightCount = mFX->GetVariableByName("gSpotLightCount")->AsScalar();
		//ambient light
		AmbientIntensity = mFX->GetVariableByName("gAmbientIntensity")->AsScalar();
		AmbientColor = mFX->GetVariableByName("gAmbientColor")->AsVector();

		OpacityValue = mFX->GetVariableByName("gOpacityValue")->AsScalar();
	}

	LitTextureEffect::~LitTextureEffect()
	{
	}
#pragma endregion

#pragma region LitTextureNormalEffect
	//constructor - get device and filename (call parent constructor first)
	LitTextureNormalEffect::LitTextureNormalEffect(ID3D11Device* device, const std::string& filename)
		: Effect(device, filename)
	{
		//for skinned meshes
		LitTexNormalSkinnedTech = mFX->GetTechniqueByName("LitTexNormSkinnedTech");
		BoneTransforms = mFX->GetVariableByName("gBoneTransforms")->AsMatrix();

		//set the LitTexTech to the shader technique
		LitTexNormalTech = mFX->GetTechniqueByName("LitTexNormalTech");
		//link Effect variables to shader variables (for get / set)
		//Matrices
		WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
		World = mFX->GetVariableByName("gWorld")->AsMatrix();
		WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
		//Maps / materials
		DiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
		Mat = mFX->GetVariableByName("gMaterial");
		NormalMap = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
		//fog stuff
		EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
		FogColor = mFX->GetVariableByName("gFogColor")->AsVector();
		FogStart = mFX->GetVariableByName("gFogStart")->AsScalar();
		FogRange = mFX->GetVariableByName("gFogRange")->AsScalar();
		FogEnabled = mFX->GetVariableByName("gFogEnabled");
		//lighting arrays and counts
		DirLights = mFX->GetVariableByName("gDirLights");
		DirLightCount = mFX->GetVariableByName("gDirLightCount")->AsScalar();
		PointLights = mFX->GetVariableByName("gPointLights");
		PointLightCount = mFX->GetVariableByName("gPointLightCount")->AsScalar();
		SpotLights = mFX->GetVariableByName("gSpotLights");
		SpotLightCount = mFX->GetVariableByName("gSpotLightCount")->AsScalar();
		//ambient light
		AmbientIntensity = mFX->GetVariableByName("gAmbientIntensity")->AsScalar();
		AmbientColor = mFX->GetVariableByName("gAmbientColor")->AsVector();

		OpacityValue = mFX->GetVariableByName("gOpacityValue")->AsScalar();

		CelEnabled = mFX->GetVariableByName("gCelShaderEnabled"); 
		InvertEnabled = mFX->GetVariableByName("gInvertEnabled");
	
	}

	LitTextureNormalEffect::~LitTextureNormalEffect()
	{
	}
#pragma endregion

//skybox effect
#pragma region SkyEffect
	SkyEffect::SkyEffect(ID3D11Device* device, const std::string& filename)
		: Effect(device, filename)
	{
		SkyTech = mFX->GetTechniqueByName("SkyTech");
		WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
		CubeMap = mFX->GetVariableByName("gCubeMap")->AsShaderResource();
	}

	SkyEffect::~SkyEffect()
	{
	}
#pragma endregion

#pragma region LitBillboardEffect
	//constructor - get device and filename (call parent constructor first)
	LitBillboardEffect::LitBillboardEffect(ID3D11Device* device, const std::string& filename)
		: Effect(device, filename)
	{
		//set the LitBillboardTech to the shader technique
		LitBillboardTech = mFX->GetTechniqueByName("LitBillboardTech");
		//link Effect variables to shader variables (for get / set)
		//Matrices
		WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
		World = mFX->GetVariableByName("gWorld")->AsMatrix();
		WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
		//Maps / materials
		DiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
		Mat = mFX->GetVariableByName("gMaterial");
		//fog stuff
		EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
		FogColor = mFX->GetVariableByName("gFogColor")->AsVector();
		FogStart = mFX->GetVariableByName("gFogStart")->AsScalar();
		FogRange = mFX->GetVariableByName("gFogRange")->AsScalar();
		FogEnabled = mFX->GetVariableByName("gFogEnabled");
		//lighting arrays and counts
		DirLights = mFX->GetVariableByName("gDirLights");
		DirLightCount = mFX->GetVariableByName("gDirLightCount")->AsScalar();
		PointLights = mFX->GetVariableByName("gPointLights");
		PointLightCount = mFX->GetVariableByName("gPointLightCount")->AsScalar();
		SpotLights = mFX->GetVariableByName("gSpotLights");
		SpotLightCount = mFX->GetVariableByName("gSpotLightCount")->AsScalar();
		//ambient light
		AmbientIntensity = mFX->GetVariableByName("gAmbientIntensity")->AsScalar();
		AmbientColor = mFX->GetVariableByName("gAmbientColor")->AsVector();

		//sprite vars
		//float values
		Column = mFX->GetVariableByName("gColumn")->AsScalar();
		NumCols = mFX->GetVariableByName("gNumCols")->AsScalar();
		FlipValue = mFX->GetVariableByName("gFlipValue")->AsScalar();
		LerpValue = mFX->GetVariableByName("gLerpValue")->AsScalar();
		OpacityValue = mFX->GetVariableByName("gOpacityValue")->AsScalar();
		//vector (color) value
		AltColorValue = mFX->GetVariableByName("gAltColor")->AsVector();
	}

	LitBillboardEffect::~LitBillboardEffect()
	{
	}
#pragma endregion

#pragma region Effects

	SpriteEffect*				Effects::SpriteFX = 0;
	ColorEffect*				Effects::ColorFX = 0;
	BasicTextureEffect*			Effects::BasicTexFX = 0;
	LitTextureEffect*			Effects::LitTexFX = 0;
	LitTextureNormalEffect*		Effects::LitTexNormalFX = 0;
	SkyEffect*					Effects::SkyFX = 0;
	LitBillboardEffect*			Effects::LitBillboardFX = 0;

	void Effects::InitAll(ID3D11Device* device)
	{
		SpriteFX = new SpriteEffect(device, "FX/Sprite.fx");
		ColorFX = new ColorEffect(device, "FX/Color.fx");
		BasicTexFX = new BasicTextureEffect(device, "FX/Texture.fx");
		LitTexFX = new LitTextureEffect(device, "FX/LitTexture.fx");
		LitTexNormalFX = new LitTextureNormalEffect(device, "FX/LitTextureNormal.fx");
		SkyFX = new SkyEffect(device, "FX/Sky.fx");
		LitBillboardFX = new LitBillboardEffect(device, "FX/LitBillboard.fx");
	}

	void Effects::DestroyAll()
	{
		SafeDelete(SpriteFX);
		SafeDelete(ColorFX);
		SafeDelete(BasicTexFX);
		SafeDelete(LitTexFX);
		SafeDelete(LitTexNormalFX);
		SafeDelete(SkyFX);
		SafeDelete(LitBillboardFX);
	}
#pragma endregion

}

