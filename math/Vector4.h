﻿#pragma once
#include "Vector3.h"

/// <summary>
/// 4次元ベクトル
/// </summary>
class Vector4 {
public:
	float x; // x成分
	float y; // y成分
	float z; // z成分
	float w; // w成分

public:
	// コンストラクタ
	Vector4();                                   // 零ベクトルとする
	Vector4(float x, float y, float z, float w); // x成分, y成分, z成分 を指定しての生成
	Vector4(Vector3 v3, float w) {
		x = v3.x;
		y = v3.y;
		z = v3.z;
		this->w = w;
	};
};