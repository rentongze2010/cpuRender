#include "Math.h"
#include <math.h>

Matrix3::Matrix3() {
	memset(d, 0, sizeof(d));
	return;
}
Matrix3 Matrix3::operator*(const Matrix3& m3){
	Matrix3 ans;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				ans.d[i][j]+=d[i][k]*m3.d[k][j];
			}
		}
	}
	return ans;
}
Matrix4::Matrix4() {
	memset(d, 0, sizeof(d));
	return;
}
Matrix4 Matrix4::operator*(const Matrix4& m4){
	Matrix4 ans;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				ans.d[i][j]+=d[i][k]*m4.d[k][j];
			}
		}
	}
	return ans;
}
Vector2::Vector2() {
	memset(d, 0, sizeof(d));
	return;
}
Vector2::Vector2(float x, float y) {
	d[0] = x;
	d[1] = y;
	return;
}
Vector2 Vector2::operator+(const Vector2& v) {
	return Vector2(d[0] + v.d[0], d[1] + v.d[1]);
}
Vector2 Vector2::operator*(const float& f) {
	return Vector2(d[0] * f, d[1] * f);
}
float Vector2::dot(const Vector2& v) {
	return d[0] * v.d[0] + d[1] * v.d[1];
}
float Vector2::det(const Vector2& v) {
	return d[0] * v.d[1] - d[1] * v.d[0];
}
Vector2 Vector2::cross(const Vector2& v) {
	return Vector2(d[0] * v.d[0] - d[1] * v.d[1], d[1] * v.d[0] + d[0] * v.d[1]);
}
Vector2 Vector2::normalize() const {
	float len = sqrt(d[0] * d[0] + d[1] * d[1]);
	if (len == 0) return Vector2(0, 0);
	return Vector2(d[0] / len, d[1] / len);
}
Vector2 Vector2::Proj(const Vector2& v) {
	return (v.normalize()) * this->dot(v.normalize());
}
Vector3::Vector3() {
	memset(d, 0, sizeof(d));
	return;
}
Vector3::Vector3(float x, float y, float z) {
	d[0] = x; d[1] = y; d[2] = z;
	return;
}
Vector3 Vector3::operator+(const Vector3& v) {
	return Vector3(d[0] + v.d[0], d[1] + v.d[1], d[2] + v.d[2]);
}
Vector3 Vector3::operator*(const Matrix3& m3) {
	Vector3 ans;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			ans.d[i] += d[j] * m3.d[i][j];
		}
	}
	return ans;
}
Vector3 Vector3::operator*(const float& f) {
	return Vector3(d[0] * f, d[1] * f, d[2] * f);
}
float Vector3::dot(const Vector3& v) {
	return d[0] * v.d[0] + d[1] * v.d[1] + d[2] * v.d[2];
}
Vector3 Vector3::cross(const Vector3& v) {
	return Vector3(d[1] * v.d[2] - d[2] * v.d[1], d[2] * v.d[0] - d[0] * v.d[2], d[0] * v.d[1] - d[1] * v.d[0]);
}
Vector3 Vector3::normalize() const {
	float len = sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
	if (len == 0) return Vector3(0, 0, 0);
	return Vector3(d[0] / len, d[1] / len, d[2] / len);
}
Vector3 Vector3::Proj(const Vector3& v) {
	return (v.normalize()) * this->dot(v.normalize());
}
Vector4::Vector4() {
	memset(d, 0, sizeof(d));
	return;
}
Vector4 Vector4::operator*(const Matrix4& m4) {
	Vector4 ans;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			ans.d[i] += d[j] * m4.d[i][j];
		}
	}
	return ans;
}
