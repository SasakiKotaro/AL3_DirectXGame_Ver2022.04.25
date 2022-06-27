#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include <memory>
#include <list>

#include "Player.h"
#include "Enemy.h"

using namespace std;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void checkAllCollisions();


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	uint32_t textureHandle_ = 0;	//テクスチャハンドル
	Model* model_ = nullptr;		//3Dモデル
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	DebugCamera* debugCamera_ = nullptr;

	const int MAXARR = 20;

	float distance = (float)MAXARR * 2;
	float interval = (float)MAXARR / (MAXARR / 2);

	Vector3 xStart_ = { distance, -interval, distance / 2 };
	Vector3 xEnd_ = { -distance, -interval, distance / 2 };

	Vector3 yStart_ = { -interval, distance , distance / 2 };
	Vector3 yEnd_ = { -interval, -distance , distance / 2 };

	Vector3 zStart_ = { distance, distance , -interval };
	Vector3 zEnd_ = { distance, -distance , -interval };

	Vector4 xColor_ = { 1,0,0,0.5 };
	Vector4 yColor_ = { 0,1,0,0.5 };
	Vector4 zColor_ = { 0,0,1,0.5 };

	Player* player_ = nullptr;

	list<unique_ptr<Enemy>> enemys_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};