#include "Matrix.h"

Matrix3::Matrix3() {
	memset(d, 0, sizeof(d)*9);
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
	return ;
}
Matrix4::Matrix4() {
	memset(d, 0, sizeof(d)*16);
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
	return ;
}
Vector3::Vector3() {
	memset(d, 0, sizeof(d) * 3);
	return;
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
Vector4::Vector4() {
	memset(d, 0, sizeof(d) * 4);
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
