#include "PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Init(Model* model, WorldTransform worldTransform)
{
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");
	worldTransform_.Initialize();
	//�����Ŏ󂯎�������W���Z�b�g
	worldTransform_.translation_ = worldTransform.translation_;
}

void PlayerBullet::Update()
{

}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{

}