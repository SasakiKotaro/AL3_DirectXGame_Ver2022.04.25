#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
//#include <list>
#include "EnemyBullet.h"
//���N���X�̑O���錾
class Player;

using namespace std;

enum class Phase
{
	Approach,	//�ڋ�
	Leave,		//���E
};

/// <summary>
/// �G
/// </summary>
class Enemy
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Init(Model* model, WorldTransform worldTransform);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// �U��
	/// </summary>
	void Fire();

	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPosition();

	/// <summary>
	/// �Փ˂̊��m
	/// </summary>
	void onCollision();

	/// <summary>
	/// �e�̃��X�g�擾
	/// </summary>
	const list<unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; };

	bool IsDead()const { return isDead_; }


private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	void(Enemy::* pCurrent)();

	static void(Enemy::* pFuncTable[])();

	//�e
	list<unique_ptr<EnemyBullet>> bullets_;

	Phase phase_;

	static const int kFireInterval = 300;
	int32_t timer = kFireInterval;

	void ApproachInit();

	/// <summary>
	/// �ڋߊ֐�
	/// </summary>
	void ApproachP();
	/// <summary>
	/// ���E�֐�
	/// </summary>
	void LeaveP();

	//�v���C���[
	Player* player_ = nullptr;

	bool isDead_ = false;
};