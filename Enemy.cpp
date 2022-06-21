#include "Enemy.h"
#include <assert.h>
#include "Player.h"
void Enemy::Init(Model* model, WorldTransform worldTransform)
{
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	worldTransform_.Initialize();
	//引数で受け取った座標をセット
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
	//弾の速度
	const float kBulletSpeed = 0.2f;
	Vector3 velocity;

	Vector3 pl = player_->GetWorldPosition();
	Vector3 en = GetWorldPosition();
	Vector3 distance = en - pl;
	distance.norm();

	//velocity = distance * kBulletSpeed;

	//float angle = atan2(velocity.y, velocity.z);

	//Vector3 velocityZ;

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = multiV3M4(worldTransform_.matWorld_, velocity);
	//弾を生成＆初期化
	unique_ptr<EnemyBullet> newBullet = make_unique<EnemyBullet>();
	newBullet->Init(model_, worldTransform_.translation_, velocity);
	//弾の登録
	bullets_.push_back(move(newBullet));
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;
	//成分を取得
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
	//基底の位置で離脱状態へ移行
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