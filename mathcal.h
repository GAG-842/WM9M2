#pragma once
#include <cmath>
#define _USE_MATH_DEFINES
#include <memory.h>
#include <iostream>
#define max(a,b)( a > b ? a : b )
#define min(a,b)( a < b ? a : b )
#define SQ(x) (x * x)
using namespace std;

class Vec2 {
public:
	union
	{
		float v[2];
		struct { float x, y; };
	};
	Vec2() { x = 0; y = 0; }
	Vec2(float _x, float _y) { x = _x; y = _y; }
	Vec2 operator+(const Vec2& pVec) const
	{
		return Vec2(x + pVec.x, y + pVec.y);
	}
	Vec2 operator-(const Vec2& pVec) const
	{
		return Vec2(x - pVec.x, y - pVec.y);
	}

	Vec2& operator+=(const Vec2& pVec)
	{
		x += pVec.x;
		y += pVec.y;

		return *this;
	}
	Vec2& operator-=(const Vec2& pVec)
	{
		x -= pVec.x;
		y -= pVec.y;
		return *this;
	}

	Vec2 operator*(const Vec2& pVec) const
	{
		return Vec2(x * pVec.x, y * pVec.y);
	}
	Vec2 operator*(const float val) const
	{
		return Vec2(x * val, y * val);
	}

	Vec2 operator/(const Vec2& pVec) const
	{
		return Vec2(x / pVec.x, y / pVec.y);
	}

	Vec2 operator/(const float val) const
	{
		return Vec2(x / val, y / val);
	}

	Vec2 operator*=(const Vec2& pVec)
	{
		x *= pVec.x;
		y *= pVec.y;

		return *this;
	}
	Vec2 operator/=(const Vec2& pVec)
	{
		x /= pVec.x;
		y /= pVec.y;
		return *this;
	}
	Vec2 operator-() const
	{
		return Vec2(-x, -y);
	}
	float getLength() const {
		return sqrtf(x * x + SQ(y));
	}
	float getLengthSquare() const {
		return (v[0] * v[0] + v[1] * v[1]);
	}

	Vec2 normalize(void)
	{
		float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1]);
		return Vec2(x * len, y * len);
	}
};

class Vec3
{
public:

	union
	{
		float v[3];
		struct { float x, y, z; };
	};
	Vec3() { x = 0; y = 0; z = 0; }
	Vec3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

	Vec3 operator+(const Vec3& pVec) const
	{
		return Vec3(x + pVec.x, y + pVec.y, z + pVec.z);
	}
	Vec3 operator-(const Vec3& pVec) const
	{
		return Vec3(x - pVec.x, y - pVec.y, z - pVec.z);
	}

	Vec3& operator+=(const Vec3& pVec)
	{
		x += pVec.x;
		y += pVec.y;
		z += pVec.z;
		return *this;
	}
	Vec3& operator-=(const Vec3& pVec)
	{
		x -= pVec.x;
		y -= pVec.y;
		z -= pVec.z;
		return *this;
	}

	Vec3 operator*(const Vec3& pVec) const
	{
		return Vec3(x * pVec.x, y * pVec.y, z * pVec.z);
	}
	Vec3 operator*(const float val) const
	{
		return Vec3(x * val, y * val, z * val);
	}

	Vec3 operator/(const Vec3& pVec) const
	{
		return Vec3(x / pVec.x, y / pVec.y, z / pVec.z);
	}

	Vec3 operator/(const float val) const
	{
		return Vec3(x / val, y / val, z / val);
	}

	Vec3 operator*=(const Vec3& pVec)
	{
		x *= pVec.x;
		y *= pVec.y;
		z *= pVec.z;
		return *this;
	}
	Vec3 operator/=(const Vec3& pVec)
	{
		x /= pVec.x;
		y /= pVec.y;
		z /= pVec.z;
		return *this;
	}
	Vec3 operator-() const
	{
		return Vec3(-x, -y, -z);
	}
	float getLength() const {
		return sqrtf(x * x + SQ(y) + z * z);
	}
	float getLengthSquare() const {
		return (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}

	Vec3 normalize(void)
	{
		float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		return Vec3(x * len, y * len, z * len);
	}

	float normalize_GetLength()
	{
		float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		float len = 1.0f / length;
		v[0] *= len; v[1] *= len; v[2] *= len;
		return length;
	}

	float Dot(const Vec3& pVec) const
	{
		return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
	}
	Vec3 Cross(const Vec3& v1)
	{
		return Vec3(v1.y * z - v1.v[2] * y,
			v1.z * x - v1.x * z,
			v1.x * y - v1.y * x);
	}
	Vec3 Max(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(max(v1.v[0], v2.v[0]),
			max(v1.v[1], v2.v[1]),
			max(v1.v[2], v2.v[2]));
	}
	float Max() const
	{
		return max(x, max(y, z));
	}



};
float Dot(const Vec3& v1, const Vec3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
class Matrix
{
public:
	union
	{
		float a[4][4];
		float m[16];
	};
	Matrix() { identity(); }
	Matrix(float* otherMatrix) {
		memcpy(&m, otherMatrix, 16 * sizeof(float));
	}
	Matrix(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) {
		a[0][0] = m00;
		a[0][1] = m01;
		a[0][2] = m02;
		a[0][3] = m03;
		a[1][0] = m10;
		a[1][1] = m11;
		a[1][2] = m12;
		a[1][3] = m13;
		a[2][0] = m20;
		a[2][1] = m21;
		a[2][2] = m22;
		a[2][3] = m23;
		a[3][0] = m30;
		a[3][1] = m31;
		a[3][2] = m32;
		a[3][3] = m33;
	}
	void identity() {
		for (int i = 0; i < 16; ++i) {
			m[i] = 0.0f;
		}
		m[0] = 1.0f;   
		m[5] = 1.0f;   
		m[10] = 1.0f;  
		m[15] = 1.0f;  
	}
	Vec3 mulPoint(const Vec3& v, Matrix m)
	{
		Vec3 v1 = Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
			(v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
			(v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
		float w;
		w = (m[12] * v.x) + (m[13] * v.y) + (m[14] * v.z) + m[15];
		w = 1.0f / w;
		return (v1 * w);
	}
	// if not projection:
	//Vec3 mulPoint(const Vec3& v)
	//{
	//	return Vec3(
	//		(v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
	//		(v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
	//		(v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
	//}
	Vec3 mulVec(const Vec3& v)
	{
		return Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]),
			(v.x * m[4] + v.y * m[5] + v.z * m[6]),
			(v.x * m[8] + v.y * m[9] + v.z * m[10]));
	}

	Matrix mul(const Matrix& matrix) const  // matrix * ret ?? ret * matrix
	{
		Matrix ret;
		ret.m[0] = m[0] * matrix.m[0] + m[4] * matrix.m[1] + m[8] * matrix.m[2] + m[12] * matrix.m[3];
		ret.m[1] = m[1] * matrix.m[0] + m[5] * matrix.m[1] + m[9] * matrix.m[2] + m[13] * matrix.m[3];
		ret.m[2] = m[2] * matrix.m[0] + m[6] * matrix.m[1] + m[10] * matrix.m[2] + m[14] * matrix.m[3];
		ret.m[3] = m[3] * matrix.m[0] + m[7] * matrix.m[1] + m[11] * matrix.m[2] + m[15] * matrix.m[3];
		ret.m[4] = m[0] * matrix.m[4] + m[4] * matrix.m[5] + m[8] * matrix.m[6] + m[12] * matrix.m[7];
		ret.m[5] = m[1] * matrix.m[4] + m[5] * matrix.m[5] + m[9] * matrix.m[6] + m[13] * matrix.m[7];
		ret.m[6] = m[2] * matrix.m[4] + m[6] * matrix.m[5] + m[10] * matrix.m[6] + m[14] * matrix.m[7];
		ret.m[7] = m[3] * matrix.m[4] + m[7] * matrix.m[5] + m[11] * matrix.m[6] + m[15] * matrix.m[7];
		ret.m[8] = m[0] * matrix.m[8] + m[4] * matrix.m[9] + m[8] * matrix.m[10] + m[12] * matrix.m[11];
		ret.m[9] = m[1] * matrix.m[8] + m[5] * matrix.m[9] + m[9] * matrix.m[10] + m[13] * matrix.m[11];
		ret.m[10] = m[2] * matrix.m[8] + m[6] * matrix.m[9] + m[10] * matrix.m[10] + m[14] * matrix.m[11];
		ret.m[11] = m[3] * matrix.m[8] + m[7] * matrix.m[9] + m[11] * matrix.m[10] + m[15] * matrix.m[11];
		ret.m[12] = m[0] * matrix.m[12] + m[4] * matrix.m[13] + m[8] * matrix.m[14] + m[12] * matrix.m[15];
		ret.m[13] = m[1] * matrix.m[12] + m[5] * matrix.m[13] + m[9] * matrix.m[14] + m[13] * matrix.m[15];
		ret.m[14] = m[2] * matrix.m[12] + m[6] * matrix.m[13] + m[10] * matrix.m[14] + m[14] * matrix.m[15];
		ret.m[15] = m[3] * matrix.m[12] + m[7] * matrix.m[13] + m[11] * matrix.m[14] + m[15] * matrix.m[15];
		return ret;

		//for (int i = 0; i < N; ++i) {
		//	for (int j = 0; j < N; ++j) {
		//		for (int k = 0; k < N; ++k) {
		//			C[i][j] += A[i][k] * B[k][j];
		//		}
		//	}
		//}

	}

	Matrix operator*(const Matrix& matrix)
	{
		return mul(matrix);
	}

	float& operator[](int index) {
		return m[index];
	}

	Matrix Transpose(const Matrix& matrix) const {
		Matrix transposeM;
		transposeM.m[0] = matrix.m[0];
		transposeM.m[1] = matrix.m[4];
		transposeM.m[2] = matrix.m[8];
		transposeM.m[3] = matrix.m[12];
		transposeM.m[4] = matrix.m[1];
		transposeM.m[5] = matrix.m[5];
		transposeM.m[6] = matrix.m[9];
		transposeM.m[7] = matrix.m[13];
		transposeM.m[8] = matrix.m[2];
		transposeM.m[9] = matrix.m[6];
		transposeM.m[10] = matrix.m[10];
		transposeM.m[11] = matrix.m[14];
		transposeM.m[12] = matrix.m[3];
		transposeM.m[13] = matrix.m[7];
		transposeM.m[14] = matrix.m[11];
		transposeM.m[15] = matrix.m[15];
	}
	Matrix Translation(const Vec3& v) {
		identity();
		m[3] = v.x;
		m[7] = v.y;
		m[11] = v.z;
		return *this;
	}

	Vec3 Translation(const Vec3& v, float tx, float ty, float tz)
	{
		Matrix translationM;
		translationM.m[0] = 1;
		translationM.m[1] = 0;
		translationM.m[2] = 0;
		translationM.m[3] = tx;
		translationM.m[4] = 0;
		translationM.m[5] = 1;
		translationM.m[6] = 0;
		translationM.m[7] = ty;
		translationM.m[8] = 0;
		translationM.m[9] = 0;
		translationM.m[10] = 1;
		translationM.m[11] = tz;
		translationM.m[12] = 0;
		translationM.m[13] = 0;
		translationM.m[14] = 0;
		translationM.m[15] = 1;
		return mulPoint(v, translationM);
	}

	Vec3 RotationX(const Vec3& v, float angle)
	{
		Matrix rotationM;
		rotationM.m[0] = 1;
		rotationM.m[1] = 0;
		rotationM.m[2] = 0;
		rotationM.m[3] = 0;
		rotationM.m[4] = 0;
		rotationM.m[5] = cos(angle);
		rotationM.m[6] = -sin(angle);
		rotationM.m[7] = 0;
		rotationM.m[8] = 0;
		rotationM.m[9] = sin(angle);
		rotationM.m[10] = cos(angle);
		rotationM.m[11] = 0;
		rotationM.m[12] = 0;
		rotationM.m[13] = 0;
		rotationM.m[14] = 0;
		rotationM.m[15] = 1;
		return mulPoint(v, rotationM);
	}

	Vec3 RotationY(const Vec3& v, float angle)
	{
		Matrix rotationM;
		rotationM.m[0] = cos(angle);
		rotationM.m[1] = 0;
		rotationM.m[2] = sin(angle);
		rotationM.m[3] = 0;
		rotationM.m[4] = 0;
		rotationM.m[5] = 1;
		rotationM.m[6] = 0;
		rotationM.m[7] = 0;
		rotationM.m[8] = -sin(angle);
		rotationM.m[9] = 0;
		rotationM.m[10] = cos(angle);
		rotationM.m[11] = 0;
		rotationM.m[12] = 0;
		rotationM.m[13] = 0;
		rotationM.m[14] = 0;
		rotationM.m[15] = 1;
		return mulPoint(v, rotationM);
	}

	Vec3 RotationZ(const Vec3& v, float angle)
	{
		Matrix rotationM;
		rotationM.m[0] = cos(angle);
		rotationM.m[1] = -sin(angle);
		rotationM.m[2] = 0;
		rotationM.m[3] = 0;
		rotationM.m[4] = sin(angle);
		rotationM.m[5] = cos(angle);
		rotationM.m[6] = 0;
		rotationM.m[7] = 0;
		rotationM.m[8] = 0;
		rotationM.m[9] = 0;
		rotationM.m[10] = 1;
		rotationM.m[11] = 0;
		rotationM.m[12] = 0;
		rotationM.m[13] = 0;
		rotationM.m[14] = 0;
		rotationM.m[15] = 1;
		return mulPoint(v, rotationM);
	}

	Vec3 Scaling(const Vec3& v, float sx, float sy, float sz)
	{
		Matrix rotationM;
		rotationM.m[0] = sx;
		rotationM.m[1] = 0;
		rotationM.m[2] = 0;
		rotationM.m[3] = 0;
		rotationM.m[4] = 0;
		rotationM.m[5] = sy;
		rotationM.m[6] = 0;
		rotationM.m[7] = 0;
		rotationM.m[8] = 0;
		rotationM.m[9] = 0;
		rotationM.m[10] = sz;
		rotationM.m[11] = 0;
		rotationM.m[12] = 0;
		rotationM.m[13] = 0;
		rotationM.m[14] = 0;
		rotationM.m[15] = 1;
		return mulPoint(v, rotationM);
	}
	Matrix Scaling(const Vec3& v) {
		identity();
		m[0] = v.x;
		m[5] = v.y;
		m[10] = v.z;
		return* this;

	}

	Matrix invert()
	{
		Matrix inv;
		inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
		inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
		inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
		inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
		inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
		inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
		inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
		inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
		inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
		inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
		inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
		inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
		float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
		if (det == 0) {
			// Handle this case
		}
		det = 1.0f / det;
		for (int i = 0; i < 16; i++) {
			inv[i] = inv[i] * det;
		}
		return inv;
	}
	Matrix perspective(float fov, float aspect, float nearPlane, float farPlane) {
		Matrix proj;
		float tanHalfFov = tanf(fov / 2.0f); // 计算半视角的正切值
		proj.m[0] = 1.0f / (aspect * tanHalfFov);
		proj.m[5] = 1.0f / tanHalfFov;
		proj.m[10] = (-farPlane) / (farPlane - nearPlane);
		proj.m[11] = -(farPlane * nearPlane) / (farPlane - nearPlane);
		proj.m[14] = -1.0f;
		proj.m[15] = 0.0f;

		return proj;
	}
	Matrix lookAt(Vec3 from, Vec3 to, Vec3 up) {
		Vec3 dir = (from - to).normalize();
		Vec3 right = up.Cross(dir);
		Vec3 upPrime = dir.Cross(right);
		Matrix mlookat;

		mlookat.m[0] = right.x;
		mlookat.m[1] = right.y;
		mlookat.m[2] = right.z;
		mlookat.m[3] = -from.Dot(right);

		mlookat.m[4] = upPrime.x;
		mlookat.m[5] = upPrime.y;
		mlookat.m[6] = upPrime.z;
		mlookat.m[7] = -from.Dot(upPrime);

		mlookat.m[8] = dir.x;
		mlookat.m[9] = dir.y;
		mlookat.m[10] = dir.z;
		mlookat.m[11] = -from.Dot(dir);

		mlookat.m[12] = 0.f;
		mlookat.m[13] = 0.f;
		mlookat.m[14] = 0.f;
		mlookat.m[15] = 1.f;

		return mlookat;
	}
};

class Spherical {
public:

	union
	{
		float v[3];
		struct { float t, o, r; };
	};
	Spherical() { t = 0; o = 0; r = 0; }
	Spherical(float _t, float _o, float _r) { t = _t; o = _o; r = _r; }
	Spherical angleExpresionZup(Vec3& v) {
		t = acos(v.z / r);
		o = atan(v.y / v.x);
		return *this;
	}
	Spherical angleExpresionYup(Vec3& v) {
		t = acos(v.y / r);
		o = atan(v.z / v.x);
		return *this;
	}
	Vec3 vecExpresionZup(Spherical& s) {
		Vec3 v;
		v.x = r * sin(s.t) * cos(s.o);
		v.y = r * sin(s.t) * sin(s.o);
		v.z = r * cos(s.t);
		return v;
	}
	Vec3 vecExpresionYup(Spherical& s) {
		Vec3 v;
		v.x = r * sin(s.t) * cos(s.o);
		v.y = r * sin(s.t);
		v.z = r * sin(s.t) * sin(s.o);
		return v;
	}
};
class Vec4
{
public:

	union
	{
		float v[4];
		struct { float x, y, z, w; };
	};
	Vec4() { x = 0; y = 0; z = 0, w = 0; }
	Vec4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z, w = _w; }

	Vec4 operator+(const Vec4& pVec) const
	{
		return Vec4(x + pVec.x, y + pVec.y, z + pVec.z, w + pVec.w);
	}
	Vec4 operator-(const Vec4& pVec) const
	{
		return Vec4(x - pVec.x, y - pVec.y, z - pVec.z, w - pVec.w);
	}

	Vec4& operator+=(const Vec4& pVec)
	{
		x += pVec.x;
		y += pVec.y;
		z += pVec.z;
		w += pVec.w;
		return *this;
	}
	Vec4& operator-=(const Vec4& pVec)
	{
		x -= pVec.x;
		y -= pVec.y;
		z -= pVec.z;
		w -= pVec.w;
		return *this;
	}

	Vec4 operator*(const Vec4& pVec) const
	{
		return Vec4(x * pVec.x, y * pVec.y, z * pVec.z, w * pVec.w);
	}
	Vec4 operator*(const float val) const
	{
		return Vec4(x * val, y * val, z * val, w * val);
	}

	Vec4 operator/(const Vec4& pVec) const
	{
		return Vec4(x / pVec.x, y / pVec.y, z / pVec.z, w / pVec.w);
	}

	Vec4 operator/(const float val) const
	{
		return Vec4(x / val, y / val, z / val, w / val);
	}

	Vec4 operator*=(const Vec4& pVec)
	{
		x *= pVec.x;
		y *= pVec.y;
		z *= pVec.z;
		w *= pVec.w;
		return *this;
	}
	Vec4 operator/=(const Vec4& pVec)
	{
		x /= pVec.x;
		y /= pVec.y;
		z /= pVec.z;
		w /= pVec.w;
		return *this;
	}
	Vec4 operator-() const
	{
		return Vec4(-x, -y, -z, -w);
	}
	float getLength() const {
		return sqrtf(x * x + SQ(y) + z * z + w * w);
	}
	float getLengthSquare() const {
		return (v[0] * v[0] + v[1] * v[1] + v[2] * v[0] + w * w);
	}

	Vec4 normalize(void)
	{
		float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + w * w);
		return Vec4(x * len, y * len, z * len, w * len);
	}

	float normalize_GetLength()
	{
		float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + w * w);
		float len = 1.0f / length;
		v[0] *= len; v[1] *= len; v[2] *= len; v[3] *= len;
		return length;
	}

	float Dot(const Vec4& pVec) const
	{
		return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2] + w * pVec.w;
	}
	Vec4 Max(const Vec4& v1, const Vec4& v2)
	{
		return Vec4(max(v1.v[0], v2.v[0]),
			max(v1.v[1], v2.v[1]),
			max(v1.v[2], v2.v[2]),
			max(v1.w, v2.w));
	}
	float Max() const
	{
		return max(x, max(y, max(z, w)));
	}
};

class Quaternion {
public:
	union {
		struct {
			float a;
			float b;
			float c;
			float d;
		};
		float q[4];
	};

	Quaternion() : a(1), b(0), c(0), d(0) {}

	Quaternion(float _a, float _b, float _c, float _d) : a(_a), b(_b), c(_c), d(_d) {}

	Quaternion operator+(const Quaternion& q) const {
		return Quaternion(a + q.a, b + q.b, c + q.c, d + q.d);
	}

	Quaternion operator-(const Quaternion& q) const {
		return Quaternion(a - q.a, b - q.b, c - q.c, d - q.d);
	}

	Quaternion operator*(const Quaternion& q) const {
		return Quaternion(
			a * q.a - b * q.b - c * q.c - d * q.d,
			a * q.b + b * q.a + c * q.d - d * q.c,
			a * q.c - b * q.d + c * q.a + d * q.b,
			a * q.d + b * q.c - c * q.b + d * q.a
		);
	}

	Quaternion operator*(float scalar) const {
		return Quaternion(a * scalar, b * scalar, c * scalar, d * scalar);
	}


	static float dot(const Quaternion& q1, const Quaternion& q2) {
		return q1.a * q2.a + q1.b * q2.b + q1.c * q2.c + q1.d * q2.d;
	}

	void normalize() {
		float magnitude = std::sqrt(a * a + b * b + c * c + d * d);
		if (magnitude > 0.0f) {
			a /= magnitude;
			b /= magnitude;
			c /= magnitude;
			d /= magnitude;
		}
	}

	static Quaternion lerp(const Quaternion& q1, const Quaternion& q2, float t) {
		return (q1 * (1 - t) + q2 * t).normalized();
	}

	static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t) {
		float dot = Quaternion::dot(q1, q2);
		if (dot < 0.0f) {
			return slerp(q1, Quaternion(-q2.a, -q2.b, -q2.c, -q2.d), t);
		}
		dot = std::fmax(-1.0f, std::fmin(1.0f, dot));
		float theta = std::acos(dot);
		if (theta < 1e-6) {
			return lerp(q1, q2, t);
		}
		float sinTheta = std::sin(theta);
		float weight1 = std::sin((1 - t) * theta) / sinTheta;
		float weight2 = std::sin(t * theta) / sinTheta;
		return (q1 * weight1 + q2 * weight2).normalized();
	}

	Matrix toMatrix() const {
		float xx = q[0] * q[0];
		float xy = q[0] * q[1];
		float xz = q[0] * q[2];
		float yy = q[1] * q[1];
		float zz = q[2] * q[2];
		float yz = q[1] * q[2];
		float wx = q[3] * q[0];
		float wy = q[3] * q[1];
		float wz = q[3] * q[2];
		Matrix matrix;
		matrix.m[0] = 1.0f - 2.0f * (yy + zz);
		matrix.m[1] = 2.0f * (xy - wz);
		matrix.m[2] = 2.0f * (xz + wy);
		matrix.m[3] = 0.0;
		matrix.m[4] = 2.0f * (xy + wz);
		matrix.m[5] = 1.0f - 2.0f * (xx + zz);
		matrix.m[6] = 2.0f * (yz - wx);
		matrix.m[7] = 0.0;
		matrix.m[8] = 2.0f * (xz - wy);
		matrix.m[9] = 2.0f * (yz + wx);
		matrix.m[10] = 1.0f - 2.0f * (xx + yy);
		matrix.m[11] = 0.0;
		matrix.m[12] = 0;
		matrix.m[13] = 0;
		matrix.m[14] = 0;
		matrix.m[15] = 1;
		return matrix;
	}

	Quaternion normalized() const {
		Quaternion q(*this);
		q.normalize();
		return q;
	}


	friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
		os << "(" << q.a << ", " << q.b << ", " << q.c << ", " << q.d << ")";
		return os;
	}
};

class shadingFrame {
public:
	union
	{
		float fa[3][3];
		float fm[9];
	};
	shadingFrame() {
		for (int i = 0; i < 9; i++) {
			fm[i] = 0;
		}
	}
	void Forward(Vec3& n) {
		Vec3 u;
		Vec3 u1;
		Vec3 v;
		if (n.x != 1 && n.y != 0 && n.z != 0) {
			u1.x = 1; u1.y = 0; u1.z = 0;
		}
		else {
			u1.x = 0; u1.y = 1; u1.z = 0;
		}

		float ux = n.x * u1.x + n.x * u1.y + n.x * u1.z;
		float uy = n.y * u1.x + n.y * u1.y + n.y * u1.z;
		float uz = n.z * u1.x + n.z * u1.y + n.z * u1.z;
		float nnu = sqrtf(SQ(ux) + SQ(uy) + SQ(uz));
		u.x = (n.x * u1.x + n.x * u1.y + n.x * u1.z) / nnu;
		u.y = (n.y * u1.x + n.y * u1.y + n.y * u1.z) / nnu;
		u.z = (n.z * u1.x + n.z * u1.y + n.z * u1.z) / nnu;
		v.x = n.x * u.x + n.x * u.y + n.x * u.z;
		v.y = n.y * u.x + n.y * u.y + n.y * u.z;
		v.z = n.z * u.x + n.z * u.y + n.z * u.z;
		fm[0] = u.x;
		fm[1] = v.x;
		fm[2] = n.x;
		fm[3] = u.y;
		fm[4] = v.y;
		fm[5] = n.y;
		fm[6] = u.z;
		fm[7] = v.z;
		fm[8] = n.z;
	}
	shadingFrame inverse() {
		shadingFrame ifm;
		ifm[0] = fm[0];
		ifm[1] = fm[3];
		ifm[2] = fm[6];
		ifm[3] = fm[1];
		ifm[4] = fm[4];
		ifm[5] = fm[7];
		ifm[6] = fm[2];
		ifm[7] = fm[5];
		ifm[8] = fm[8];
		return *this;
	}
	float& operator[](int index) {
		return fm[index];
	}
};

class Colour {
public:
	float r, g, b, a;
	Colour() {
		r = 0; g = 0; b = 0; a = 0;
	}
	Colour(float _r, float _g, float _b, float _a) {
		r = _r; g = _g; b = _b; a = _a;
	}
	Colour operator+(const Colour& colour) {
		r += colour.r;
		g += colour.g;
		b += colour.b;
		a += colour.a;
	}
	Colour operator*(const Colour& colour) {
		r *= colour.r;
		g *= colour.g;
		b *= colour.b;
		a *= colour.a;
	}
	Colour operator*(const float x) {
		r *= x;
		g *= x;
		b *= x;
		a *= x;
	}
	Colour operator/(const float x) {
		r /= x;
		g /= x;
		b /= x;
		a /= x;
	}

};

class Rasterization {
public:
	Vec2 v0;
	Vec2 v1;
	Vec2 v2;
	Rasterization() {
		v0.x = 0; v0.y = 0;
		v1.x = 0; v1.y = 0;
		v2.x = 0; v2.y = 0;
	}
	Rasterization(Vec2& _v0, Vec2& _v1, Vec2& _v2) {
		v0.x = _v0.x; v0.y = _v0.y;
		v1.x = _v1.x; v1.y = _v1.y;
		v2.x = _v2.x; v2.y = _v2.y;
	}
	bool isInTrig(Vec2& tv) {
		Vec2 q0;
		Vec2 q1;
		Vec2 q2;
		q0 = tv - v0;
		q1 = tv - v1;
		q2 = tv - v2;
		Vec2 e0 = v1 - v0;
		Vec2 e1 = v2 - v1;
		Vec2 e2 = v0 - v2;
		float tf0 = q0.x * e0.y - q0.y * e0.x;
		float tf1 = q1.x * e1.y - q1.y * e1.x;
		float tf2 = q2.x * e2.y - q2.y * e2.x;
		if (tf0 >= 0 && tf1 >= 0 && tf2 >= 0) {
			return true;
		}
		else {
			return false;
		}
	}

};
