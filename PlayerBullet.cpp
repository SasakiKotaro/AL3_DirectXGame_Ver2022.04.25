#include "PlayerBullet.h"
#include <assert.h>
#include "DebugText.h"

void PlayerBullet::Init(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");
	worldTransform_.Initialize();
	//�����Ŏ󂯎�������W���Z�b�g
	worldTransform_.translation_ = position;
	//�����Ŏ󂯎���������x�ŃZ�b�g
	velocity_ = velocity;
}

void PlayerBullet::Update()
{
	worldTransform_.translation_ += velocity_;
	//�Čv�Z
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