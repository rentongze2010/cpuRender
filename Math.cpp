#include "Math.h"

auto changeM34 = [](Matrix3 m) {
	Matrix4 ans4;
	ans4.d[0][0] = m.d[0][0], ans4.d[0][1] = m.d[0][1], ans4.d[0][2] = m.d[0][2], ans4.d[0][3] = 0;
	ans4.d[1][0] = m.d[1][0], ans4.d[1][1] = m.d[1][1], ans4.d[1][2] = m.d[1][2], ans4.d[1][3] = 0;
	ans4.d[2][0] = m.d[2][0], ans4.d[2][1] = m.d[2][1], ans4.d[2][2] = m.d[2][2], ans4.d[2][3] = 0;
	ans4.d[3][0] = 0, ans4.d[3][1] = 0, ans4.d[3][2] = 0, ans4.d[3][3] = 1;
	return ans4;
	};
auto changeM43 = [](Matrix4 m) {
	Matrix3 ans3;
	ans3.d[0][0] = m.d[0][0], ans3.d[0][1] = m.d[0][1], ans3.d[0][2] = m.d[0][2];
	ans3.d[1][0] = m.d[1][0], ans3.d[1][1] = m.d[1][1], ans3.d[1][2] = m.d[1][2];
	ans3.d[2][0] = m.d[2][0], ans3.d[2][1] = m.d[2][1], ans3.d[2][2] = m.d[2][2];
	return ans3;
	};
auto changeV34 = [](Vector3 v) {
	return Vector4(v.d[0], v.d[1], v.d[2], 1);
	};
auto changeV43 = [](Vector4 v) {
	return Vector3(v.d[0], v.d[1], v.d[2]);
	};

Matrix3::Matrix3() {
	memset(d, 0, sizeof(d));
	return;
}
Matrix3 Matrix3::operator*(const Matrix3& m3) const {
	return changeM43(changeM34(m3)*changeM34(*this));
	/*Matrix3 ans;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				ans.d[i][j]+=d[i][k]*m3.d[k][j];
			}
		}
	}
	return ans;*/
}
Vector3 Matrix3::operator*(const Vector3& v) const {
	return changeV43(changeM34(*this) * changeV34(v));
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
Matrix4 Matrix4::operator*(const Matrix4& m4) const {
	Matrix4 ans;
	__m128 l[4], t[4], temp;
	for(int i=0;i<4;i++)l[i]=_mm_load_ps(m4.d[i]);
	for (int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++)t[j]=_mm_set1_ps(d[i][j]);
		temp = _mm_mul_ps(l[0], t[0]);
		for (int j = 1; j < 4; j++)temp = _mm_fmadd_ps(l[j], t[j], temp);
		_mm_store_ps(ans.d[i], temp);
	}
	/*for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				ans.d[i][j]+=d[i][k]*m4.d[k][j];
			}
		}
	}*/
	return ans;
}
Vector4 Matrix4::operator*(const Vector4& v) const {
	Vector4 ans;
	__m128 r0, r1, r2, r3;
	__m128 vec, res;

	// 1. 加载矩阵的 4 行 (假设 Matrix4 已 alignas(16))
	// 如果不确定对齐，请改用 _mm_loadu_ps
	r0 = _mm_load_ps(d[0]);
	r1 = _mm_load_ps(d[1]);
	r2 = _mm_load_ps(d[2]);
	r3 = _mm_load_ps(d[3]);

	// 2. 加载向量 (建议用 loadu 防止栈对齐问题，除非 Vector4 也强制对齐)
	vec = _mm_loadu_ps(v.d);

	// 3. 核心优化：将行转置为列
	// 转置后：r0 变为第 0 列 (x 系数), r1 变为第 1 列 (y 系数)...
	_MM_TRANSPOSE4_PS(r0, r1, r2, r3);

	// 4. 计算结果 = Col0*v.x + Col1*v.y + Col2*v.z + Col3*v.w
	// _mm_shuffle_ps(vec, vec, 0x00) 将 v.x 广播到 4 个通道
	res = _mm_mul_ps(r0, _mm_shuffle_ps(vec, vec, 0x00));
	res = _mm_fmadd_ps(r1, _mm_shuffle_ps(vec, vec, 0x55), res); // v.y
	res = _mm_fmadd_ps(r2, _mm_shuffle_ps(vec, vec, 0xAA), res); // v.z
	res = _mm_fmadd_ps(r3, _mm_shuffle_ps(vec, vec, 0xFF), res); // v.w

	// 5. 存储结果
	_mm_storeu_ps(ans.d, res);
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
	float len = (float)sqrt(d[0] * d[0] + d[1] * d[1]);
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
	float len = (float)sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
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
Vector4::Vector4(float x, float y, float z, float w) {
	d[0] = x; d[1] = y; d[2] = z; d[3] = w;
	return;
}
bool Vector4::operator==(const Vector4& v) const {
	return memcmp(d, v.d, sizeof(d));
}
Vector4 Vector4::normalize() const {
	float len = (float)sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2] + d[3] * d[3]);
	if(len <= 0.000001f)return Vector4(0, 0, 0, 0);
	return Vector4(d[0] / len, d[1] / len, d[2] / len, d[3] / len);
}
