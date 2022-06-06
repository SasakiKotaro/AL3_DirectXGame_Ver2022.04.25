#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(Model* model, WorldTransform worldtransform);

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

	Vector3 move;
	Vector3 rotate;
};