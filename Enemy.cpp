#include "Enemy.h"
#include <assert.h>
#include "Player.h"
void Enemy::Init(Model* model, WorldTransform worldTransform)
{
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");
	worldTransform_.Initialize();
	//�����Ŏ󂯎�������W���Z�b�g
	worldTransform_.translation_ = Vector3(10, 0, 50);
	pCurrent = &Enemy::ApproachP;
	ApproachInit();
}

void Enemy::Update()
{
	(this->*pCurrent)();
	(this->*pFuncTable[static_cast<size_t>(phase_)])();

	bullets_.remove_if([](unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		}
	);

	for (unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
	worldTransform_.Update();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire()
{
	//�e�̑��x
	const float kBulletSpeed = 0.2f;
	Vector3 velocity;

	Vector3 pl = player_->GetWorldPosition();
	Vector3 en = GetWorldPosition();
	Vector3 distance = en - pl;
	distance.norm();

	//velocity = distance * kBulletSpeed;

	//float angle = atan2(velocity.y, velocity.z);

	//Vector3 velocityZ;

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = multiV3M4(worldTransform_.matWorld_, velocity);
	//�e�𐶐���������
	unique_ptr<EnemyBullet> newBullet = make_unique<EnemyBullet>();
	newBullet->Init(model_, worldTransform_.translation_, velocity);
	//�e�̓o�^
	bullets_.push_back(move(newBullet));
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;
	//�������擾
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

void Enemy::ApproachP()
{
	Vector3 ApproachSpeed(0, 0, 0.01);
	worldTransform_.translation_ -= ApproachSpeed;
	timer--;
	if (timer <= 0)
	{
		Fire();
		timer = kFireInterval;
	}
	//���̈ʒu�ŗ��E��Ԃֈڍs
	//if (worldTransform_.translation_.z < -20.0f)
	//{
	//	phase_ = Phase::Leave;
	//}
}

void Enemy::LeaveP()
{
	Vector3 LeaveSpeed(0.5, 0.2, 0);
	worldTransform_.translation_ -= LeaveSpeed;
}

void Enemy::ApproachInit()
{
	timer = 0;
}

void (Enemy::* Enemy::pFuncTable[])() =
{
	&Enemy::ApproachP,
	&Enemy::LeaveP
};