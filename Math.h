#pragma once
#include <vector>
#include <math.h>
#include <immintrin.h>

struct Matrix3;
struct Matrix4;
struct Vector2;
struct Vector3;
struct Vector4;

struct Matrix3 {
	float d[3][3];
	Matrix3();
	Matrix3 operator*(const Matrix3& m3) const;
	Vector3 operator*(const Vector3& v) const;
	bool operator==(const Matrix3& m3) const;
};
struct alignas(16) Matrix4 {
	float d[4][4];
	Matrix4();
	Matrix4(const Vector4& v);	//Quaternions
	Matrix4 operator*(const Matrix4& m4) const;
	Vector4 operator*(const Vector4& v) const;
	bool operator==(const Matrix4& m4) const;
};
struct Vector2 {
	float d[2];
	Vector2();
	Vector2(float x, float y);
	Vector2 operator+(const Vector2& v);
	Vector2 operator*(const float& f);
	bool operator==(const Vector2& v) const;
	float dot(const Vector2& v);
	float det(const Vector2& v);
	Vector2 cross(const Vector2& v);
	Vector2 normalize() const;
	Vector2 Proj(const Vector2& v);
};
struct Vector3 {
	float d[3];
	Vector3();
	Vector3(float x, float y, float z);
	Vector3 operator+(const Vector3& v);
	Vector3 operator*(const float& f);
	bool operator==(const Vector3& v) const;
	float dot(const Vector3& v);
	//Vector3 det(const Vector3& v);
	Vector3 cross(const Vector3& v);
	Vector3 normalize() const;
	Vector3 Proj(const Vector3& v);
};
struct Vector4 {
	float d[4];
	Vector4();
	Vector4(float x, float y, float z, float w);
	bool operator==(const Vector4& v) const;
	Vector4 normalize() const;
};