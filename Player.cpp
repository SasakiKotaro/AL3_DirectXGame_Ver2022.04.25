#include "Player.h"
#include "assert.h"
#define UP DIK_UP
#define DOWN DIK_DOWN
#define LEFT DIK_LEFT
#define RIGHT DIK_RIGHT
#define PI 3.141592
#define TORADIAN(r) (r*PI/180)

void Player::Init(Model* model, uint32_t textureHandle)
{
	//nullptr�`�F�b�N
	assert(model);
	//���
	model_ = model;
	textureHandle_ = textureHandle;
	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update()
{
	worldTransform_.Initialize();
	Move();

	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	pos.x = max(pos.x, -kMoveLimitX);
	pos.x = min(pos.x, +kMoveLimitX);
	pos.y = max(pos.y, -kMoveLimitY);
	pos.y = min(pos.y, +kMoveLimitY);
	worldTransform_.translation_ = pos;
	//�g�k
	Matrix4 scaleMat;
	scaleMat.SetScale(1.0f, 1.0f, 1.0f);
	//��]
	Matrix4 rotaMat;
	rotaMat.SetRotate(
		TORADIAN(0),
		worldTransform_.rotation_.y,
		TORADIAN(0));
	//���s�ړ�
	Matrix4 transMat = MathUtility::Matrix4Identity();
	transMat.SetTranslate(
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
	//�|�����킹
	worldTransform_.matWorld_.SetMultiple(scaleMat, rotaMat, transMat);
	//�]��
	worldTransform_.TransferMatrix();
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

}

void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
