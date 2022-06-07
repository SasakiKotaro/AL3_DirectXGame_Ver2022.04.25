#include "Enemy.h"
#include <assert.h>

void Enemy::Init(Model* model, WorldTransform worldTransform)
{
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	worldTransform_.Initialize();
	//引数で受け取った座標をセット
	worldTransform_.translation_ = worldTransform.translation_;
}

void Enemy::Update()
{
	worldTransform_.translation_.z -= 0.3;
	worldTransform_.Update();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
