#include"WorldTransform.h"

////�ړ��v�Z
//worldTransforms_[PartId::kRoot].translation_ += move;
//worldTransforms_[PartId::kChest].rotation_ += chestAngle;
//worldTransforms_[PartId::kHip].rotation_ += hipAngle;
//for (int i = 0; i < PartId::kNumPartId; i++)
//{
//	//�g�k
//	Matrix4 scaleMat;
//	scaleMat.SetScale(1.0f, 1.0f, 1.0f);
//	//��]
//	Matrix4 rotaMat;
//	rotaMat.SetRotate(
//		TORADIAN(0),
//		worldTransforms_[i].rotation_.y,
//		TORADIAN(0));
//	//���s�ړ�
//	Matrix4 transMat = MathUtility::Matrix4Identity();
//	transMat.SetTranslate(
//		worldTransforms_[i].translation_.x,
//		worldTransforms_[i].translation_.y,
//		worldTransforms_[i].translation_.z);
//	//�|�����킹
//	if (worldTransforms_[i].parent_ == nullptr)
//	{
//		worldTransforms_[i].matWorld_.SetMultiple(scaleMat, rotaMat, transMat);
//	}
//	else
//	{
//		worldTransforms_[i].matWorld_.SetMultiple(scaleMat, rotaMat, transMat);
//		worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
//	}
//	//�]��
//	worldTransforms_[i].TransferMatrix();

	//void Worldtransform::Update()
	//{

	//}