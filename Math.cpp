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
bool Matrix3::operator==(const Matrix3& m3) const {
	return !memcmp(d, m3.d, sizeof(d));
}
Matrix4::Matrix4() {
	memset(d, 0, sizeof(d));
	return;
}
Matrix4::Matrix4(const Vector4& v) {
	d[0][0]=1, d[0][1]=0, d[0][2]=0, d[0][3]=0;
	d[1][0]=0, d[1][1]=(1-2*v.d[2]* v.d[2]-2*v.d[3]* v.d[3]), d[1][2] = (2*v.d[1]*v.d[2]-2*v.d[3]*v.d[0]), d[1][3] = (2*v.d[1]*v.d[3]+2*v.d[2]*v.d[0]);
	d[2][0]=0, d[2][1]=(2*v.d[1]*v.d[2]+2*v.d[3]*v.d[0]), d[2][2] = (1-2*v.d[1]*v.d[1]-2*v.d[3]*v.d[3]), d[2][3] = (2*v.d[2]*v.d[3]-2*v.d[1]*v.d[0]);
	d[3][0]=0, d[3][1]=(2*v.d[1]*v.d[3]-2*v.d[2]*v.d[0]), d[3][2] = (2*v.d[2]*v.d[3]+2*v.d[1]*v.d[0]), d[3][3] = (1-2*v.d[1]*v.d[1]-2*v.d[2]*v.d[2]);
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
bool Matrix4::operator==(const Matrix4& m4) const {
	return memcmp(d, m4.d, sizeof(d));
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
bool Vector2::operator==(const Vector2& v) const {
	return memcmp(d, v.d, sizeof(d));
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
	if (len <= 0.000001f) return Vector2(0, 0);
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
bool Vector3::operator==(const Vector3& v) const {
	return memcmp(d, v.d, sizeof(d));
}
float Vector3::dot(const Vector3& v) {
	return d[0] * v.d[0] + d[1] * v.d[1] + d[2] * v.d[2];
}
Vector3 Vector3::cross(const Vector3& v) {
	return Vector3(d[1] * v.d[2] - d[2] * v.d[1], d[2] * v.d[0] - d[0] * v.d[2], d[0] * v.d[1] - d[1] * v.d[0]);
}
Vector3 Vector3::normalize() const {
	float len = sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
	if (len <= 0.000001f) return Vector3(0, 0, 0);
	return Vector3(d[0] / len, d[1] / len, d[2] / len);
}
Vector3 Vector3::Proj(const Vector3& v) {
	return (v.normalize()) * this->dot(v.normalize());
}
Vector4::Vector4() {
	memset(d, 0, sizeof(d));
	return;
}
Vector4::Vector4(float w, float x, float y, float z) {
	d[0] = x; d[1] = y; d[2] = z; d[3] = w;
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
bool Vector4::operator==(const Vector4& v) const {
	return memcmp(d, v.d, sizeof(d));
}
Vector4 Vector4::normalize() const {
	float len = sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2] + d[3] * d[3]);
	if(len <= 0.000001f)return Vector4(0, 0, 0, 0);
	return Vector4(d[0] / len, d[1] / len, d[2] / len, d[3] / len);
}
