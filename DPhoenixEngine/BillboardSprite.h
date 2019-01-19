#pragma once
#include "Sprite.h"

namespace DPhoenix
{
	//inherit from sprite (key animation vars etc.)
	class BillboardSprite : public Sprite
	{
	public:

		bool maintainY;		//stand straight on Y axis 
		XMFLOAT3 mRotation;	//rotation var for orienting towards camera

		//constructor / destructor
		BillboardSprite();
		BillboardSprite(const BillboardSprite &sprite);
		virtual ~BillboardSprite();

		//load image
		bool LoadBS(std::string filename, TextureMgr* mTexMgr,
			float width, float height, ID3D11Device* md3dDevice);

		void UpdateBS(float deltaTime);
		void RenderBS(ID3D11DeviceContext* dc);		//draw sprite to screen

		//this is used to generate the matrices for rendering the sprite
		XMMATRIX GetWorldBS(XMFLOAT3 screenScale);

	};
}

