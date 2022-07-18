#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DebugText.h"

class RailCamera
{
public:

	/// <summary>
	/// èâä˙âª
	/// </summary>
	void Init(Vector3 pos, Vector3 angle);

	/// <summary>
	/// çXêV
	/// </summary>
	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }
	WorldTransform* GetWorldTransform() { return &worldTransform_; }

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	DebugText* debugText_;
};