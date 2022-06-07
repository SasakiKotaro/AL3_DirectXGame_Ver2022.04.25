#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include <list>

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



private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Phase phase_ = Phase::Approach;
	/// <summary>
	/// �ڋߊ֐�
	/// </summary>
	void ApproachP();
	/// <summary>
	/// ���E�֐�
	/// </summary>
	void LeaveP();
};