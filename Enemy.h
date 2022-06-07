#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include <list>

using namespace std;

/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init(Model* model, WorldTransform worldTransform);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
};