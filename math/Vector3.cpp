#include "Vector3.h"
#include "math.h"

/// <summary>
/// ベクトルの長さ
/// </summary>
/// <returns></returns>
float Vector3::length() const
{
	return sqrt(dot(*this));
}

/// <summary>
/// ベクトルの正規化
/// </summary>
/// <returns></returns>
Vector3& Vector3::norm()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

/// <summary>
/// 内積
/// </summary>
/// <param name="v">Vector3</param>
/// <returns></returns>
float Vector3::dot(const Vector3& v) const
{
	return x * x + y * y + z * z;
}

/// <summary>
/// 外積
/// </summary>
/// <param name="v">Vector3</param>
/// <returns></returns>
Vector3 Vector3::cross(const Vector3& v) const
{
	Vector3 result;
	result.x = y * v.z - z * v.y;
	result.y = x * v.z - z * v.x;
	result.z = x * v.y - y * v.z;

	return result;
}

//Vector3クラスに属さない関数
const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}