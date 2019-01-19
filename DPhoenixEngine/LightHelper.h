#pragma once
#include "D3DUtil.h"

//Note: Make sure structure alignment agrees with HLSL structure padding rules. 
//Elements are packed into 4D vectors with the restriction that an element
//cannot straddle a 4D vector boundary.

//light from an arbritary distant point (e.g. sunlight)
struct DirectionalLight
{
	//zeroise before use
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;	//ambient light colour (uniform brightness)	
	XMFLOAT4 Diffuse;	//diffuse light colour (scattered brightness)
	XMFLOAT4 Specular;	//specular colour (shiny highligts - w dimension is power)
	XMFLOAT3 Direction;	//direction light pointing
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

//as applied to models, how they react to light sources
struct Material
{
	//zeroise before use
	Material() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;	//ambient light colour (uniform brightness)
	XMFLOAT4 Diffuse;	//diffuse light colour (scattered brightness)
	XMFLOAT4 Specular;	//specular colour (shiny highligts - w dimension is power)
	XMFLOAT4 Reflect;	//relfection values (we're not using this, advanced challenge) 
};

//light from a positon that emanates in all directions
struct PointLight
{
	//zeroise before use
	PointLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;	//ambient light colour (uniform brightness)
	XMFLOAT4 Diffuse;	//diffuse light colour (scattered brightness)
	XMFLOAT4 Specular;	//specular colour (shiny highligts - w dimension is power)

	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 Position;
	float Range;

	// Packed into 4D vector: (A0, A1, A2, Pad) - attenuation (drop-off) around source
	XMFLOAT3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

//spotlight - coned light from a point in a direction
struct SpotLight
{
	//zeroise before use
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;	//ambient light colour (uniform brightness)
	XMFLOAT4 Diffuse;	//diffuse light colour (scattered brightness)
	XMFLOAT4 Specular;	//specular colour (shiny highligts - w dimension is power)

	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 Position;
	float Range;

	// Packed into 4D vector: (Direction, Spot)
	XMFLOAT3 Direction;
	float Spot;

	// Packed into 4D vector: (A0, A1, A2, Pad) - attenuation (drop-off) around source
	XMFLOAT3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

