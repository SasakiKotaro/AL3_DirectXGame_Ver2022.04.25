#pragma once
#include "Vector4.h"

/// <summary>
/// 行列
/// </summary>
class Matrix4 {
public:
	// 行x列
	float m[4][4];

	// コンストラクタ
	Matrix4();
	// 成分を指定しての生成
	Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	Matrix4(Vector4 v)
	{
		m[0][0] = v.x;
		m[1][0] = v.y;
		m[2][0] = v.z;
		m[3][0] = v.w;
		m[0][1] = 0;
		m[1][1] = 0;
		m[2][1] = 0;
		m[3][1] = 0;
		m[0][2] = 0;
		m[1][2] = 0;
		m[2][2] = 0;
		m[3][2] = 0;
		m[0][3] = 0;
		m[1][3] = 0;
		m[2][3] = 0;
		m[3][3] = 0;
	}
	Matrix4(Vector3 v)
	{
		m[0][0] = v.x;
		m[1][0] = 0;
		m[2][0] = 0;
		m[3][0] = 0;
		m[0][1] = 0;
		m[1][1] = v.y;
		m[2][1] = 0;
		m[3][1] = 0;
		m[0][2] = 0;
		m[1][2] = 0;
		m[2][2] = v.z;
		m[3][2] = 0;
		m[0][3] = 0;
		m[1][3] = 0;
		m[2][3] = 0;
		m[3][3] = 1;
	}

	/// <summary>
	/// スケーリング倍率の設定
	/// </summary>
	/// <param name="x">X軸の拡大率</param>
	/// <param name="y">Y軸の拡大率</param>
	/// <param name="z">Z軸の拡大率</param>
	Matrix4 SetScale(float x, float y, float z);

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="x">X軸の角度</param>
	/// <param name="y">Y軸の角度</param>
	/// <param name="z">Z軸の角度</param>
	Matrix4 SetRotate(float x, float y, float z);

	/// <summary>
	/// 拡縮の倍率設定
	/// </summary>
	/// <param name="x">x移動</param>
	/// <param name="y">y移動</param>
	/// <param name="z">z移動</param>
	/// <returns></returns>
	Matrix4 SetTranslate(float x, float y, float z);

	/// <summary>
	/// 組み合わせ
	/// </summary>
	/// <param name="s">scale</param>
	/// <param name="r">rotate</param>
	/// <param name="t">transrate</param>
	/// <returns></returns>
	void SetMultiple(Matrix4 s, Matrix4 r, Matrix4 t);


private:
	/// <summary>
	/// 単位行列の設定
	/// </summary>
	void SetId();

	/// <summary>
	/// 単位行列との掛け算
	/// </summary>
	/// <returns>計算結果</returns>
	Matrix4 CalcId();

	/// <summary>
	/// X軸の回転倍率を設定
	/// </summary>
	/// <param name="x">Xの回転角</param>
	Matrix4 SetXRotate(float x);

	/// <summary>
	/// Y軸の回転倍率を設定
	/// </summary>
	/// <param name="y">Yの回転角</param>
	Matrix4 SetYRotate(float y);

	/// <summary>
	/// Z軸の回転倍率を設定
	/// </summary>
	/// <param name="z">Zの回転角</param>
	Matrix4 SetZRotate(float z);

public:
	// 代入演算子オーバーロード
	Matrix4& operator*=(const Matrix4& m2);

	const Matrix4 matrixScale(Vector3 v3);
	const Matrix4 matrixUpdate(Vector3 v3, Matrix4 mat, Matrix4 iM);
};

Matrix4& operator*(const Matrix4& m1, const Matrix4& m2);