#include "PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Init(Model* model, WorldTransform worldTransform)
{
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	worldTransform_.Initialize();
	//引数で受け取った座標をセット
	worldTransform_.translation_ = worldTransform.translation_;
}

void PlayerBullet::Update()
{

}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{

}