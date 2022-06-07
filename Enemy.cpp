#include "Enemy.h"
#include <assert.h>

void Enemy::Init(Model* model, WorldTransform worldTransform)
{
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");
	worldTransform_.Initialize();
	//�����Ŏ󂯎�������W���Z�b�g
	worldTransform_.translation_ = worldTransform.translation_;
	pCurrent = &Enemy::ApproachP;
}

void Enemy::Update()
{
	(this->*pCurrent)();
	(this->*pFuncTable[static_cast<size_t>(phase_)])();

	worldTransform_.Update();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::ApproachP()
{
	Vector3 ApproachSpeed(0, 0, 0.2);
	worldTransform_.translation_ -= ApproachSpeed;
	//���̈ʒu�ŗ��E��Ԃֈڍs
	if (worldTransform_.translation_.z < -20.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveP()
{
	Vector3 LeaveSpeed(0.5, 0.2, 0);
	worldTransform_.translation_ -= LeaveSpeed;
}

void (Enemy::* Enemy::pFuncTable[])() =
{
	&Enemy::ApproachP,
	&Enemy::LeaveP
};