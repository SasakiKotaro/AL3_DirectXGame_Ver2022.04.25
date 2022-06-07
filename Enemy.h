#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include <list>

using namespace std;

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

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
};