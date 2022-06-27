#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include"DebugText.h"
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

using namespace std;

/// <summary>
/// ���L����
/// </summary>
class Player
{
public:
	//Player();
	//~Player();

	/// <summary>
	/// ������
	/// </summary>
	void Init(Model* model, uint32_t textureHandle);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// ���[���h���W�̎擾
	/// </summary>
	Vector3 GetWorldPosition();

	/// <summary>
	/// �Փˊ��m
	/// </summary>
	void onCollision();

	/// <summary>
	/// �e�̃��X�g�擾
	/// </summary>
	const list<unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; };

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���͏���
	Input* input_ = nullptr;
	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	Vector3 pos;

	list<unique_ptr<PlayerBullet>> bullets_;
	//���x
	//�ړ���
	const float moveSpeed = 0.1f;
	const float rotateSpeed = 0.02f;
	//���_�ړ��̑���
	const float zoomSpeed = 0.02f;
	const float clipSpeed = 0.2f;
};