#include "EnemyBullet.h"
#include <assert.h>
#include "DebugText.h"

void EnemyBullet::Init(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red.png");
	worldTransform_.Initialize();
	//引数で受け取った座標をセット
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = { 0.5,0.5,2 };
	worldTransform_.Update();
	//引数で受け取った加速度でセット
	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	worldTransform_.translation_ -= velocity_;
	worldTransform_.Update();
	if (--deathTimer_ <= 0) { isDead_ = true; }
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}