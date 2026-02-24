#pragma once
#include <vector>

struct Matrix3 {
	float d[3][3];
	Matrix3();
	Matrix3 operator*(const Matrix3& m3);
};
struct Matrix4 {
	float d[4][4];
	Matrix4();
	Matrix4 operator*(const Matrix4& m4);
};
struct Vector3 {
	float d[3];
	Vector3();
	Vector3 operator*(const Matrix3& m3);
};
struct Vector4 {
	float d[4];
	Vector4();
	Vector4 operator*(const Matrix4& m4);
};