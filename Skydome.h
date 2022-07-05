#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"

class Skydome
{
public:
	void Init(Model* model, uint32_t texture);
	void Update();
	void Draw(ViewProjection viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texture_;
};