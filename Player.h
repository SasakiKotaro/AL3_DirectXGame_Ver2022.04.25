#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include"DebugText.h"
#include "ViewProjection.h"
#include "PlayerBullet.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	//Player();
	//~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(Model* model, uint32_t textureHandle);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//入力処理
	Input* input_ = nullptr;
	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	Vector3 pos;

	PlayerBullet* bullet_ = nullptr;
	//速度
	//移動量
	const float moveSpeed = 0.1f;
	const float rotateSpeed = 0.02f;
	//視点移動の速さ
	const float zoomSpeed = 0.02f;
	const float clipSpeed = 0.2f;
};