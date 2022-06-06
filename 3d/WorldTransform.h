#pragma once

#include "Vector3.h"
#include "Matrix4.h"
#include <d3d12.h>
#include <wrl.h>
#include "MathUtility.h"

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
	Matrix4 matWorld;           // ローカル → ワールド変換行列
};

/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	Vector3 scale_ = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0, 0, 0 };
	// ローカル座標
	Vector3 translation_ = { 0, 0, 0 };
	// ローカル → ワールド変換行列
	Matrix4 matWorld_;
	// 親となるワールド変換へのポインタ
	WorldTransform* parent_ = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();

	void Update()
	{
		Initialize();
		//拡縮
		Matrix4 scaleMat;
		scaleMat.SetScale(1.0f, 1.0f, 1.0f);
		//回転
		Matrix4 rotaMat;
		rotaMat.SetRotate(
			rotation_.x,
			rotation_.y,
			rotation_.z);
		//平行移動
		Matrix4 transMat = MathUtility::Matrix4Identity();
		transMat.SetTranslate(
			translation_.x,
			translation_.y,
			translation_.z);
		//掛け合わせ
		matWorld_.SetMultiple(scaleMat, rotaMat, transMat);
		//転送
		TransferMatrix();
	};
};