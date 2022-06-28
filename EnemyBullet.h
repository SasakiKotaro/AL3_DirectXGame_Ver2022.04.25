#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
/// <summary>
/// ���L�����̒e
/// </summary>
class EnemyBullet
{
public:

	/// <summary>
	/// ������
	/// </summary>
	void Init(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead()const { return isDead_; }

	/// <summary>
	/// �Փ˂̊��m
	/// </summary>
	void onCollision();

	Vector3 GetWorldPosition();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_;

	//����
	static const int32_t kLifeTime = 60 * 5;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
};