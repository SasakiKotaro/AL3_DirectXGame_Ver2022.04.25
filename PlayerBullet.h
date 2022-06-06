#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
/// <summary>
/// ���L�����̒e
/// </summary>
class PlayerBullet
{
public:

	/// <summary>
	/// ������
	/// </summary>
	void Init(Model* model, WorldTransform worldtransform);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 move;
	Vector3 rotate;
};