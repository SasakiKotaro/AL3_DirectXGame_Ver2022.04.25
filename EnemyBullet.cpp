#include "EnemyBullet.h"
#include <assert.h>
#include "DebugText.h"

void EnemyBullet::Init(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("red.png");
	worldTransform_.Initialize();
	//�����Ŏ󂯎�������W���Z�b�g
	worldTransform_.translation_ = position;
	//�����Ŏ󂯎���������x�ŃZ�b�g
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

void EnemyBullet::onCollision()
{
	isDead_ = true;
}