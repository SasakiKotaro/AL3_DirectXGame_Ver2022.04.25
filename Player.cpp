#include "Player.h"
#include "assert.h"
#include "MathUtility.h"
#define KEY(k) (input_->PushKey(k))
#define TRIGGER(k) (input_->TriggerKey(k))
#define UP DIK_UP
#define DOWN DIK_DOWN
#define LEFT DIK_LEFT
#define RIGHT DIK_RIGHT
#define SPACE DIK_SPACE
#define PI 3.141592
#define TORADIAN(r) (r*PI/180)

void Player::Init(Model* model, uint32_t textureHandle)
{
	//nullptrチェック
	assert(model);
	//代入
	model_ = model;
	textureHandle_ = textureHandle;
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	worldTransform_.translation_.z = 50;
	worldTransform_.Initialize();
}

void Player::Update()
{
	bullets_.remove_if([](unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

	Move();
	Attack();
	for (unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

void Player::Move()
{
	if (input_->PushKey(RIGHT))
	{
		pos.x += moveSpeed;
	}
	else if (input_->PushKey(LEFT))
	{
		pos.x -= moveSpeed;
	}
	if (input_->PushKey(UP))
	{
		pos.y += moveSpeed;
	}
	else if (input_->PushKey(DOWN))
	{
		pos.y -= moveSpeed;
	}

	if (KEY(DIK_Q))
	{
		worldTransform_.rotation_.y += rotateSpeed;
	}
	else if (KEY(DIK_E))
	{
		worldTransform_.rotation_.y -= rotateSpeed;
	}
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	pos.x = max(pos.x, -kMoveLimitX);
	pos.x = min(pos.x, +kMoveLimitX);
	pos.y = max(pos.y, -kMoveLimitY);
	pos.y = min(pos.y, +kMoveLimitY);
	worldTransform_.translation_ += pos;
	worldTransform_.Update();
	pos = { 0,0,0 };
}

void Player::Attack()
{
	if (TRIGGER(SPACE))
	{
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//弾を生成＆初期化
		unique_ptr<PlayerBullet> newBullet = make_unique<PlayerBullet>();
		Vector3 worldTransration = 
			MathUtility::Vector3TransformCoord(worldTransform_.translation_, worldTransform_.matWorld_);
		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = multiV3M4(worldTransform_.matWorld_, velocity);
		newBullet->Init(
			model_,
			worldTransration,
			velocity);

		//弾の登録
		bullets_.push_back(move(newBullet));
	}
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;
	//成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

void Player::onCollision()
{
}

void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

