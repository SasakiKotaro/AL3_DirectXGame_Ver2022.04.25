#include "PlayerBullet.h"
#include <assert.h>
#include "DebugText.h"

void PlayerBullet::Init(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	worldTransform_.Initialize();
	//引数で受け取った座標をセット
	worldTransform_.translation_ = position;
	//引数で受け取った加速度でセット
	velocity_ = velocity;
}

void PlayerBullet::Update()
{
	worldTransform_.translation_ += velocity_;
	//再計算
	worldTransform_.Update();
	if (--deathTimer_ <= 0) { isDead_ = true; }
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::onCollision()
{
	isDead_ = true;
}