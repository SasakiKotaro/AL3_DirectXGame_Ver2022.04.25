#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
//#include <list>
#include "EnemyBullet.h"
//自クラスの前方宣言
class Player;

using namespace std;

enum class Phase
{
	Approach,	//接近
	Leave,		//離脱
};

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

	/// <summary>
	/// 攻撃
	/// </summary>
	void Fire();

	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPosition();

	/// <summary>
	/// 衝突の感知
	/// </summary>
	void onCollision();

	/// <summary>
	/// 弾のリスト取得
	/// </summary>
	const list<unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; };

	bool IsDead()const { return isDead_; }


private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	void(Enemy::* pCurrent)();

	static void(Enemy::* pFuncTable[])();

	//弾
	list<unique_ptr<EnemyBullet>> bullets_;

	Phase phase_;

	static const int kFireInterval = 300;
	int32_t timer = kFireInterval;

	void ApproachInit();

	/// <summary>
	/// 接近関数
	/// </summary>
	void ApproachP();
	/// <summary>
	/// 離脱関数
	/// </summary>
	void LeaveP();

	//プレイヤー
	Player* player_ = nullptr;

	bool isDead_ = false;
};