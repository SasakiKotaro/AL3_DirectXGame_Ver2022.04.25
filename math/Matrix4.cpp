#include "Matrix4.h"
#include "math.h"

void Matrix4::SetId()
{
	m[0][0] = 1;
	m[1][1] = 1;
	m[2][2] = 1;
	m[3][3] = 1;
}

Matrix4 Matrix4::CalcId()
{
	Matrix4 mat;
	mat.SetId();
	return mat *= *this;
}

Matrix4 Matrix4::SetScale(float x, float y, float z)
{
	m[0][0] = x;
	m[1][0] = 0;
	m[2][0] = 0;
	m[3][0] = 0;
	m[0][1] = 0;
	m[1][1] = y;
	m[2][1] = 0;
	m[3][1] = 0;
	m[0][2] = 0;
	m[1][2] = 0;
	m[2][2] = z;
	m[3][2] = 0;
	m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0;
	m[3][3] = 1;
	CalcId();
	return *this;
}

Matrix4 Matrix4::SetRotate(float x, float y, float z)
{
	Matrix4 xr, yr, zr;
	SetId();

	*this *= xr.SetXRotate(x);
	*this *= yr.SetYRotate(y);
	*this *= zr.SetZRotate(z);
	return *this;
}

Matrix4 Matrix4::SetTranslate(float x, float y, float z)
{
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
	m[3][3] = 1;
	CalcId();
	return *this;
}

void Matrix4::SetMultiple(Matrix4 s, Matrix4 r, Matrix4 t)
{
	*this *= s;
	*this *= r;
	*this *= t;
}

Matrix4 Matrix4::SetXRotate(float x)
{
	m[0][0] = 1;
	m[1][1] = cos(x);
	m[1][2] = -sin(x);
	m[2][1] = sin(x);
	m[2][2] = cos(x);
	m[3][3] = 1;
	return *this;
}

Matrix4 Matrix4::SetYRotate(float y)
{
	m[0][0] = cos(y);
	m[1][1] = 1;
	m[0][2] = -sin(y);
	m[2][0] = sin(y);
	m[2][2] = cos(y);
	m[3][3] = 1;
	return *this;
}

Matrix4 Matrix4::SetZRotate(float z)
{
	m[0][0] = cos(z);
	m[0][1] = sin(z);
	m[1][0] = -sin(z);
	m[1][1] = cos(z);
	m[2][2] = 1;
	m[3][3] = 1;
	return *this;
}

Matrix4& operator*(const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 m = m1;
	m *= m2;
	return m;
}