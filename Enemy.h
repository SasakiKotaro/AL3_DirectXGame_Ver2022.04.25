#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include <list>
#include "EnemyBullet.h"
//©ƒNƒ‰ƒX‚Ì‘O•ûéŒ¾
class Player;

using namespace std;

enum class Phase
{
	Approach,	//Ú‹ß
	Leave,		//—£’E
};

/// <summary>
/// “G
/// </summary>
class Enemy
{
public:
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Init(Model* model, WorldTransform worldTransform);

	/// <summary>
	/// XV
	/// </summary>
	void Update();

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// UŒ‚
	/// </summary>
	void Fire();

	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPosition();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	void(Enemy::* pCurrent)();

	static void(Enemy::* pFuncTable[])();

	//’e
	list<unique_ptr<EnemyBullet>> bullets_;

	Phase phase_;

	static const int kFireInterval = 120;
	int32_t timer = 0;

	void ApproachInit();

	/// <summary>
	/// Ú‹ßŠÖ”
	/// </summary>
	void ApproachP();
	/// <summary>
	/// —£’EŠÖ”
	/// </summary>
	void LeaveP();

	//ƒvƒŒƒCƒ„[
	Player* player_ = nullptr;

};