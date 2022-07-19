#include "RailCamera.h"

void RailCamera::Init(Vector3 pos, Vector3 angle)
{
	debugText_ = DebugText::GetInstance();
	worldTransform_.Initialize();
	viewProjection_.farZ = 100;
	//worldTransform_.translation_ = translation;
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = angle;
	viewProjection_.Initialize();
}

void RailCamera::Update()
{
	worldTransform_.translation_.z += 0.2;
	worldTransform_.rotation_.z += 0.02;
	worldTransform_.Update();

	viewProjection_.eye = worldTransform_.translation_;
	Vector3 forward(0, 0, 1);
	forward = multiV3M4(worldTransform_.matWorld_, forward);
	viewProjection_.target = viewProjection_.eye + forward;
	Vector3 up(0, 1, 0);
	viewProjection_.up = multiV3M4(worldTransform_.matWorld_, up);
	viewProjection_.UpdateMatrix();

	//debug
	debugText_->SetPos(50, 150);
	float x = viewProjection_.eye.x
		, y = viewProjection_.eye.y
		, z = viewProjection_.eye.z;
	debugText_->Printf("%f,%f,%f", x, y, z);
}