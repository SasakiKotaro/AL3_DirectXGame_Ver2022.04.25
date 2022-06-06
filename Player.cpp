#include "Player.h"
#include "assert.h"
#define KEY(k) (input_->PushKey(k))
#define UP DIK_UP
#define DOWN DIK_DOWN
#define LEFT DIK_LEFT
#define RIGHT DIK_RIGHT
#define PI 3.141592
#define TORADIAN(r) (r*PI/180)

void Player::Init(Model* model, uint32_t textureHandle)
{
	//nullptrƒ`ƒFƒbƒN
	assert(model);
	//‘ã“ü
	model_ = model;
	textureHandle_ = textureHandle;
	//ƒVƒ“ƒOƒ‹ƒgƒ“ƒCƒ“ƒXƒ^ƒ“ƒX‚ðŽæ“¾‚·‚é
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update()
{
	worldTransform_.Initialize();
	Move();
	Attack();
	if (bullet_) { bullet_->Update(); }
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
	else if (input_->PushKey(DIK_E))
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
	//Šgk
	Matrix4 scaleMat;
	scaleMat.SetScale(1.0f, 1.0f, 1.0f);
	//‰ñ“]
	Matrix4 rotaMat;
	rotaMat.SetRotate(
		TORADIAN(0),
		worldTransform_.rotation_.y,
		TORADIAN(0));
	//•½sˆÚ“®
	Matrix4 transMat = MathUtility::Matrix4Identity();
	transMat.SetTranslate(
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
	//Š|‚¯‡‚í‚¹
	worldTransform_.matWorld_.SetMultiple(scaleMat, rotaMat, transMat);
	//“]‘—
	worldTransform_.TransferMatrix();
}

void Player::Attack()
{
	if (KEY(DIK_SPACE))
	{
		//’e‚ð¶¬•‰Šú‰»
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Init(model_, worldTransform_);
		//’e‚Ì“o˜^
		bullet_ = newBullet;
	}
}
void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	if (bullet_) { bullet_->Draw(viewProjection); }
}

