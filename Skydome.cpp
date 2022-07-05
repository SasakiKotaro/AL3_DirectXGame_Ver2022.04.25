#include "Skydome.h"
#include "assert.h"

void Skydome::Init(Model* model, uint32_t texture)
{
	//nullptr�`�F�b�N
	assert(model);
	//���
	model_ = model;
	texture_ = texture;
	//�V���O���g���C���X�^���X���擾����
	worldTransform_.Initialize();
}

void Skydome::Update()
{
	worldTransform_.Initialize();
	worldTransform_.scale_.x = 100;
	worldTransform_.scale_.y = 100;
	worldTransform_.scale_.z = 100;
	worldTransform_.Update();
}

void Skydome::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}