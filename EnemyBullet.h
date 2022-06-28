#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
/// <summary>
/// 自キャラの弾
/// </summary>
class EnemyBullet
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead()const { return isDead_; }

	/// <summary>
	/// 衝突の感知
	/// </summary>
	void onCollision();

	Vector3 GetWorldPosition();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_;

	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};