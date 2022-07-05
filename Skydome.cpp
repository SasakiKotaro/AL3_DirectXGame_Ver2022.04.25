#include "Skydome.h"
#include "assert.h"

void Skydome::Init(Model* model, uint32_t texture)
{
	//nullptrチェック
	assert(model);
	//代入
	model_ = model;
	texture_ = texture;
	//シングルトンインスタンスを取得する
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