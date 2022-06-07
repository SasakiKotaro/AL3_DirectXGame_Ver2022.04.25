#include "Player.h"
#include "assert.h"
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

	worldTransform_.Initialize();
}

void Player::Update()
{
	bullets_.remove_if([](unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});
	worldTransform_.Initialize();
	Move();
	Attack();
	for (unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	//debug
#ifdef DEBUG
	debugText_->SetPos(50, 50);
	debugText_->Printf("angle:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
#endif // DEBUG
	debugText_->SetPos(50, 50);
	debugText_->Printf("angle:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
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
	worldTransform_.translation_ = pos;
	//拡縮
	Matrix4 scaleMat;
	scaleMat.SetScale(1.0f, 1.0f, 1.0f);
	//回転
	Matrix4 rotaMat;
	rotaMat.SetRotate(
		TORADIAN(0),
		worldTransform_.rotation_.y,
		TORADIAN(0));
	//平行移動
	Matrix4 transMat = MathUtility::Matrix4Identity();
	transMat.SetTranslate(
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
	//掛け合わせ
	worldTransform_.matWorld_.SetMultiple(scaleMat, rotaMat, transMat);
	//転送
	worldTransform_.TransferMatrix();
}

void Player::Attack()
{
	if (TRIGGER(SPACE))
	{
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = multiV3M4(worldTransform_.matWorld_, velocity);
		//弾を生成＆初期化
		unique_ptr<PlayerBullet> newBullet = make_unique<PlayerBullet>();
		newBullet->Init(model_, worldTransform_.translation_, velocity);
		//弾の登録
		bullets_.push_back(move(newBullet));
	}
}
void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

