#pragma once

#include "D3DUtil.h"

namespace DPhoenix
{

//this region is the overall effect class used to construct all effects
#pragma region Effect
	class Effect
	{
	public:
		//we need the D3D device and filename to construct
		Effect(ID3D11Device* device, const std::string& filename);
		virtual ~Effect();

	private:
		Effect(const Effect& rhs);
		Effect& operator=(const Effect& rhs);

	protected:
		ID3DX11Effect* mFX;
	};
#pragma endregion

	//this class manages the sprite rendering specifically
#pragma region SpriteEffect
	class SpriteEffect : public Effect
	{
	public:
		//we need the D3D device and shader filenameto construct
		SpriteEffect(ID3D11Device* device, const std::string& filename);
		~SpriteEffect();

		//set the WorldViewProjection combined matrix
		void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
		//set the World matrix
		void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
		//set the Diffuse map (colour-map / visible texture)
		void SetDiffuseMap(ID3D11ShaderResourceView* tex) { SpriteTex->SetResource(tex); }
		//set current column of spritesheet (for UV position)
		void SetColumn(float f) { Column->SetRawValue(&f, 0, sizeof(float)); }
		//set width of individual column (for UV mapping)
		void SetNumCols(float f) { NumCols->SetRawValue(&f, 0, sizeof(float)); }
		//set whether the sprite is flipped horizontally or not (facing left or right)
		void SetFlipValue(float f) { FlipValue->SetRawValue(&f, 0, sizeof(float)); }
		//set whether the sprite is to be interpolated between default texture and alternative color
		void SetLerpValue(float f) { LerpValue->SetRawValue(&f, 0, sizeof(float)); }
		//set an alternative colour to interpolate between the txture with
		void SetAltColorValue(const FXMVECTOR v) { AltColorValue->SetFloatVector(reinterpret_cast<const float*>(&v)); }
		//set an opacity value
		void SetOpacityValue(float f) { OpacityValue->SetRawValue(&f, 0, sizeof(float)); }

		//effect techniques that can be used
		ID3DX11EffectTechnique* SpriteTech;
		//matrix variables for the shader
		ID3DX11EffectMatrixVariable* WorldViewProj;
		ID3DX11EffectMatrixVariable* WorldViewProjTex;
		ID3DX11EffectMatrixVariable* World;
		//float values for the shader
		ID3DX11EffectVariable* Column;
		ID3DX11EffectVariable* NumCols;
		ID3DX11EffectVariable* FlipValue;
		ID3DX11EffectVariable* LerpValue;
		ID3DX11EffectVariable* OpacityValue;
		//vector variables
		ID3DX11EffectVectorVariable* AltColorValue;
		//shader resource view - this is for the sprite texture
		ID3DX11EffectShaderResourceVariable* SpriteTex;
	};
#pragma endregion

#pragma region ColorEffect
	class ColorEffect : public Effect
	{
	public:
		//we need the D3D device and shader filenameto construct
		ColorEffect(ID3D11Device* device, const std::string& filename);
		~ColorEffect();

		//set the WorldViewProjection combined matrix
		void SetWorldViewProj(CXMMATRIX M) 
		{ 
			WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); 
		}
		//set an opacity value
		void SetOpacityValue(float f) { OpacityValue->SetRawValue(&f, 0, sizeof(float)); }

		//effect techniques that can be used
		ID3DX11EffectTechnique* ColorTech;
		//matrix variables for the shader
		ID3DX11EffectMatrixVariable* WorldViewProj;

		//for transparency
		ID3DX11EffectScalarVariable* OpacityValue;

	};
#pragma endregion

#pragma region BasicTextureEffect
	class BasicTextureEffect : public Effect
	{
	public:
		//we need the D3D device and shader filenameto construct
		BasicTextureEffect(ID3D11Device* device, const std::string& filename);
		~BasicTextureEffect();

		//set the WorldViewProjection combined matrix
		void SetWorldViewProj(CXMMATRIX M)
		{
			WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M));
		}
		//set the texture to be applied
		void SetDiffuseMap(ID3D11ShaderResourceView* tex)
		{
			DiffuseMap->SetResource(tex);
		}
		//set the intensity of natural scene lighting
		void SetAmbIntensity(float f)
		{
			AmbientIntensity->SetRawValue(&f, 0, sizeof(float));
		}
		//set the color of natural scene lighting
		void SetAmbLight(const FXMVECTOR v)
		{
			AmbientColor->SetFloatVector(reinterpret_cast<const float*>(&v));
		}
		
		//texture scaling
		void SetTexScaleU(float f) { TexScaleU->SetFloat(f); }
		void SetTexScaleV(float f) { TexScaleV->SetFloat(f); }

		//set an opacity value
		void SetOpacityValue(float f) { OpacityValue->SetRawValue(&f, 0, sizeof(float)); }

		//effect techniques that can be used
		ID3DX11EffectTechnique* BasicTexTech;
		//matrix variables for the shader
		ID3DX11EffectMatrixVariable* WorldViewProj;
		//diffuse / color texture map
		ID3DX11EffectShaderResourceVariable* DiffuseMap;
		//float values for the shader
		ID3DX11EffectVariable* AmbientIntensity;
		//vector variables
		ID3DX11EffectVectorVariable* AmbientColor;
		//tex scale
		ID3DX11EffectScalarVariable* TexScaleU;
		ID3DX11EffectScalarVariable* TexScaleV;

		//for transparency
		ID3DX11EffectScalarVariable* OpacityValue;

	};
#pragma endregion

#pragma region LitTextureEffect
	class LitTextureEffect : public Effect
	{
	public:
		//we need the D3D device and shader filenameto construct
		LitTextureEffect(ID3D11Device* device, const std::string& filename);
		~LitTextureEffect();
		//set matrices
		void SetWorldViewProj(CXMMATRIX M) { 
			WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M));	}
		void SetWorld(CXMMATRIX M) { 
			World->SetMatrix(reinterpret_cast<const float*>(&M)); }
		void SetWorldInvTranspose(CXMMATRIX M) { 
			WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
		//set maps / materials
		void SetDiffuseMap(ID3D11ShaderResourceView* tex) {	DiffuseMap->SetResource(tex); }
		void SetMaterial(const Material& mat) { 
			Mat->SetRawValue(&mat, 0, sizeof(Material)); }
		//fog vars

		void SetEyePosW(const XMFLOAT3& v) { 
			EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
		void SetFogColor(const FXMVECTOR v) { 
			FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
		void SetFogStart(float f) { FogStart->SetFloat(f); }
		void SetFogRange(float f) { FogRange->SetFloat(f); }
		void SetFogEnabled(bool b) { FogEnabled->SetRawValue(&b, 0, sizeof(bool)); }
		//light arrays
		void SetDirLights(const DirectionalLight* lights) { 
			DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
		void SetPointLights(const PointLight* lights) { 
			PointLights->SetRawValue(lights, 0, 10 * sizeof(PointLight)); }
		void SetSpotLights(const SpotLight* lights) {
			SpotLights->SetRawValue(lights, 0, 10 * sizeof(SpotLight));
		}
		//light counts
		void SetDirLightCount(float f) { 
			DirLightCount->SetRawValue(&f, 0, sizeof(float)); }
		void SetPointLightCount(float f)		{
			PointLightCount->SetRawValue(&f, 0, sizeof(float));
		}
		void SetSpotLightCount(float f) {
			SpotLightCount->SetRawValue(&f, 0, sizeof(float));
		}
		//ambient light (not really used if using other light sources)
		void SetAmbIntensity(float f)	{	
			AmbientIntensity->SetRawValue(&f, 0, sizeof(float));	}
		void SetAmbLight(const FXMVECTOR v)	{	
			AmbientColor->SetFloatVector(reinterpret_cast<const float*>(&v));	}
		
		//bone transforms
		void SetBoneTransforms(const XMFLOAT4X4* M, int cnt) { 
			BoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }

		//set an opacity value
		void SetOpacityValue(float f) { OpacityValue->SetRawValue(&f, 0, sizeof(float)); }

		//effect techniques that can be used
		ID3DX11EffectTechnique* LitTexSkinnedTech;
		//bone transforms
		ID3DX11EffectMatrixVariable* BoneTransforms;

		//effect techniques that can be used
		ID3DX11EffectTechnique* LitTexTech;
		//matrix variables for the shader
		ID3DX11EffectMatrixVariable* WorldViewProj;
		ID3DX11EffectMatrixVariable* World;
		ID3DX11EffectMatrixVariable* WorldInvTranspose;
		//diffuse / color texture map / materials
		ID3DX11EffectShaderResourceVariable* DiffuseMap;
		ID3DX11EffectVariable* Mat;
		//fog stuff
		ID3DX11EffectVectorVariable* EyePosW;
		ID3DX11EffectVectorVariable* FogColor;
		ID3DX11EffectScalarVariable* FogStart;
		ID3DX11EffectScalarVariable* FogRange;
		ID3DX11EffectVariable* FogEnabled;
		//lighting
		ID3DX11EffectVariable* DirLights;		
		ID3DX11EffectVariable* DirLightCount;
		ID3DX11EffectVariable* PointLights;
		ID3DX11EffectVariable* PointLightCount;
		ID3DX11EffectVariable* SpotLights;
		ID3DX11EffectVariable* SpotLightCount;
		//float values for the shader
		ID3DX11EffectVariable* AmbientIntensity;
		ID3DX11EffectVectorVariable* AmbientColor;

		//for transparency
		ID3DX11EffectScalarVariable* OpacityValue;

	};
#pragma endregion

#pragma region LitTextureNormalEffect
	class LitTextureNormalEffect : public Effect
	{
	public:
		//we need the D3D device and shader filenameto construct
		LitTextureNormalEffect(ID3D11Device* device, const std::string& filename);
		~LitTextureNormalEffect();
		//set matrices
		void SetWorldViewProj(CXMMATRIX M) {
			WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M));
		}
		void SetWorld(CXMMATRIX M) {
			World->SetMatrix(reinterpret_cast<const float*>(&M));
		}
		void SetWorldInvTranspose(CXMMATRIX M) {
			WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M));
		}
		//set maps / materials
		void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }
		void SetMaterial(const Material& mat) {
			Mat->SetRawValue(&mat, 0, sizeof(Material));
		}
		void SetNormalMap(ID3D11ShaderResourceView* tex)
		{
			NormalMap->SetResource(tex);
		}

		//fog vars
		void SetEyePosW(const XMFLOAT3& v) {
			EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3));
		}
		void SetFogColor(const FXMVECTOR v) {
			FogColor->SetFloatVector(reinterpret_cast<const float*>(&v));
		}
		void SetFogStart(float f) { FogStart->SetFloat(f); }
		void SetFogRange(float f) { FogRange->SetFloat(f); }
		void SetFogEnabled(bool b) { FogEnabled->SetRawValue(&b, 0, sizeof(bool)); }
		//light arrays
		void SetDirLights(const DirectionalLight* lights) {
			DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight));
		}
		void SetPointLights(const PointLight* lights) {
			PointLights->SetRawValue(lights, 0, 10 * sizeof(PointLight));
		}
		void SetSpotLights(const SpotLight* lights) {
			SpotLights->SetRawValue(lights, 0, 10 * sizeof(SpotLight));
		}
		//light counts
		void SetDirLightCount(float f) {
			DirLightCount->SetRawValue(&f, 0, sizeof(float));
		}
		void SetPointLightCount(float f) {
			PointLightCount->SetRawValue(&f, 0, sizeof(float));
		}
		void SetSpotLightCount(float f) {
			SpotLightCount->SetRawValue(&f, 0, sizeof(float));
		}
		//ambient light (not really used if using other light sources)
		void SetAmbIntensity(float f) {
			AmbientIntensity->SetRawValue(&f, 0, sizeof(float));
		}
		void SetAmbLight(const FXMVECTOR v) {
			AmbientColor->SetFloatVector(reinterpret_cast<const float*>(&v));
		}

		//bone transforms
		void SetBoneTransforms(const XMFLOAT4X4* M, int cnt) {
			BoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt);
		}

		//set an opacity value
		void SetOpacityValue(float f) { OpacityValue->SetRawValue(&f, 0, sizeof(float)); }

		//effect techniques that can be used
		ID3DX11EffectTechnique* LitTexNormalSkinnedTech;
		//bone transforms
		ID3DX11EffectMatrixVariable* BoneTransforms;

		//effect techniques that can be used
		ID3DX11EffectTechnique* LitTexNormalTech;
		//matrix variables for the shader
		ID3DX11EffectMatrixVariable* WorldViewProj;
		ID3DX11EffectMatrixVariable* World;
		ID3DX11EffectMatrixVariable* WorldInvTranspose;
		//diffuse / color texture map / materials
		ID3DX11EffectShaderResourceVariable* DiffuseMap;
		ID3DX11EffectVariable* Mat;
		ID3DX11EffectShaderResourceVariable* NormalMap;
		//fog stuff
		ID3DX11EffectVectorVariable* EyePosW;
		ID3DX11EffectVectorVariable* FogColor;
		ID3DX11EffectScalarVariable* FogStart;
		ID3DX11EffectScalarVariable* FogRange;
		ID3DX11EffectVariable* FogEnabled;
		//lighting
		ID3DX11EffectVariable* DirLights;
		ID3DX11EffectVariable* DirLightCount;
		ID3DX11EffectVariable* PointLights;
		ID3DX11EffectVariable* PointLightCount;
		ID3DX11EffectVariable* SpotLights;
		ID3DX11EffectVariable* SpotLightCount;
		//float values for the shader
		ID3DX11EffectVariable* AmbientIntensity;
		ID3DX11EffectVectorVariable* AmbientColor;

		//for transparency
		ID3DX11EffectScalarVariable* OpacityValue;

	};
#pragma endregion

//effect for skybox
#pragma region SkyEffect
	class SkyEffect : public Effect
	{
	public:
		SkyEffect(ID3D11Device* device, const std::string& filename);
		~SkyEffect();
		//matrix and cubemap (for each 'face' of the sky)
		void SetWorldViewProj(CXMMATRIX M) { 
			WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
		void SetCubeMap(ID3D11ShaderResourceView* cubemap) { 
			CubeMap->SetResource(cubemap); }

		//technique
		ID3DX11EffectTechnique* SkyTech;
		//matrix and cubemap
		ID3DX11EffectMatrixVariable* WorldViewProj;
		ID3DX11EffectShaderResourceVariable* CubeMap;
	};
#pragma endregion

#pragma region LitBillboardEffect
	class LitBillboardEffect : public Effect
	{
	public:
		//we need the D3D device and shader filenameto construct
		LitBillboardEffect(ID3D11Device* device, const std::string& filename);
		~LitBillboardEffect();
		//set matrices
		void SetWorldViewProj(CXMMATRIX M) {
			WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M));
		}
		void SetWorld(CXMMATRIX M) {
			World->SetMatrix(reinterpret_cast<const float*>(&M));
		}
		void SetWorldInvTranspose(CXMMATRIX M) {
			WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M));
		}
		//set maps / materials
		void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }
		void SetMaterial(const Material& mat) {
			Mat->SetRawValue(&mat, 0, sizeof(Material));
		}

		//fog vars
		void SetEyePosW(const XMFLOAT3& v) {
			EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3));
		}
		void SetFogColor(const FXMVECTOR v) {
			FogColor->SetFloatVector(reinterpret_cast<const float*>(&v));
		}
		void SetFogStart(float f) { FogStart->SetFloat(f); }
		void SetFogRange(float f) { FogRange->SetFloat(f); }
		void SetFogEnabled(bool b) { FogEnabled->SetRawValue(&b, 0, sizeof(bool)); }
		//light arrays
		void SetDirLights(const DirectionalLight* lights) {
			DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight));
		}
		void SetPointLights(const PointLight* lights) {
			PointLights->SetRawValue(lights, 0, 10 * sizeof(PointLight));
		}
		void SetSpotLights(const SpotLight* lights) {
			SpotLights->SetRawValue(lights, 0, 10 * sizeof(SpotLight));
		}
		//light counts
		void SetDirLightCount(float f) {
			DirLightCount->SetRawValue(&f, 0, sizeof(float));
		}
		void SetPointLightCount(float f) {
			PointLightCount->SetRawValue(&f, 0, sizeof(float));
		}
		void SetSpotLightCount(float f) {
			SpotLightCount->SetRawValue(&f, 0, sizeof(float));
		}
		//ambient light (not really used if using other light sources)
		void SetAmbIntensity(float f) {
			AmbientIntensity->SetRawValue(&f, 0, sizeof(float));
		}
		void SetAmbLight(const FXMVECTOR v) {
			AmbientColor->SetFloatVector(reinterpret_cast<const float*>(&v));
		}

		//sprite vars
		//set current column of spritesheet (for UV position)
		void SetColumn(float f) { Column->SetRawValue(&f, 0, sizeof(float)); }
		//set width of individual column (for UV mapping)
		void SetNumCols(float f) { NumCols->SetRawValue(&f, 0, sizeof(float)); }
		//set whether the sprite is flipped horizontally or not (facing left or right)
		void SetFlipValue(float f) { FlipValue->SetRawValue(&f, 0, sizeof(float)); }
		//set whether the sprite is to be interpolated between default texture and alternative color
		void SetLerpValue(float f) { LerpValue->SetRawValue(&f, 0, sizeof(float)); }
		//set an alternative colour to interpolate between the txture with
		void SetAltColorValue(const FXMVECTOR v) { AltColorValue->SetFloatVector(reinterpret_cast<const float*>(&v)); }
		//set an opacity value
		void SetOpacityValue(float f) { OpacityValue->SetRawValue(&f, 0, sizeof(float)); }

		//effect techniques that can be used
		ID3DX11EffectTechnique* LitBillboardTech;
		//matrix variables for the shader
		ID3DX11EffectMatrixVariable* WorldViewProj;
		ID3DX11EffectMatrixVariable* World;
		ID3DX11EffectMatrixVariable* WorldInvTranspose;
		//diffuse / color texture map / materials
		ID3DX11EffectShaderResourceVariable* DiffuseMap;
		ID3DX11EffectVariable* Mat;
		//fog stuff
		ID3DX11EffectVectorVariable* EyePosW;
		ID3DX11EffectVectorVariable* FogColor;
		ID3DX11EffectScalarVariable* FogStart;
		ID3DX11EffectScalarVariable* FogRange;
		ID3DX11EffectVariable* FogEnabled;
		//lighting
		ID3DX11EffectVariable* DirLights;
		ID3DX11EffectVariable* DirLightCount;
		ID3DX11EffectVariable* PointLights;
		ID3DX11EffectVariable* PointLightCount;
		ID3DX11EffectVariable* SpotLights;
		ID3DX11EffectVariable* SpotLightCount;
		//float values for the shader
		ID3DX11EffectVariable* AmbientIntensity;
		ID3DX11EffectVectorVariable* AmbientColor;

		//sprite vars
		//float values for the shader
		ID3DX11EffectVariable* Column;
		ID3DX11EffectVariable* NumCols;
		ID3DX11EffectVariable* FlipValue;
		ID3DX11EffectVariable* LerpValue;
		ID3DX11EffectVariable* OpacityValue;
		//vector variables
		ID3DX11EffectVectorVariable* AltColorValue;
	};
#pragma endregion

//The Effects class (with the additional s) manages
//the initialisation and use of all indivisual Effect child classes
#pragma region Effects
	class Effects
	{
	public:
		static void InitAll(ID3D11Device* device);
		static void DestroyAll();

		static SpriteEffect* SpriteFX;
		static ColorEffect* ColorFX;
		static BasicTextureEffect* BasicTexFX;
		static LitTextureEffect* LitTexFX;
		static LitTextureNormalEffect* LitTexNormalFX;
		static SkyEffect* SkyFX;
		static LitBillboardEffect* LitBillboardFX;

	};
#pragma endregion
}

