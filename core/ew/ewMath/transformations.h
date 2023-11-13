/*
	Author: Eric Winebrenner
*/

#pragma once
#include "mat4.h"
#include "vec3.h"

namespace ew {
	// Identity matrix
	inline ew::Mat4 Identity() {
		return ew::Mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};

	// Scale on x,y,z axes
	inline ew::Mat4 Scale(const ew::Vec3& s) {
		return ew::Mat4(
			s.x, 0, 0, 0,
			0, s.y, 0, 0,
			0, 0, s.z, 0,
			0, 0, 0, 1
		);
	};

	// Rotation around X axis (pitch) in radians
	inline ew::Mat4 RotateX(float rad) {
		const float cosA = cosf(rad);
		const float sinA = sinf(rad);
		return Mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cosA, -sinA, 0.0f,
			0.0f, sinA, cosA, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};

	// Rotation around Y axis (yaw) in radians
	inline ew::Mat4 RotateY(float rad) {
		const float cosA = cosf(rad);
		const float sinA = sinf(rad);
		return Mat4(
			cosA, 0.0f, sinA, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sinA, 0.0f, cosA, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};

	// Rotation around Z axis (roll) in radians
	inline ew::Mat4 RotateZ(float rad) {
		const float cosA = cosf(rad);
		const float sinA = sinf(rad);
		return Mat4(
			cosA, -sinA, 0.0f, 0.0f,
			sinA, cosA, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};

	// Translate x,y,z
	inline ew::Mat4 Translate(const ew::Vec3& t) {
		return Mat4(
			1.0f, 0.0f, 0.0f, t.x,
			0.0f, 1.0f, 0.0f, t.y,
			0.0f, 0.0f, 1.0f, t.z,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};

	// Creates a right-handed view space
	// eye = eye (camera) position
	// target = position to look at
	// up = up axis, usually (0, 1, 0)
	inline ew::Mat4 LookAt(const ew::Vec3& eye, const ew::Vec3& target, const ew::Vec3& up) 
	{
		//step 1: constructing vectors: f r and u 
		ew::Vec3 f = ew::Normalize(target - eye); //forward/back
		ew::Vec3 r = ew::Normalize(ew::Cross(f, up)); //right/left
		ew::Vec3 u = ew::Cross(r, f); //up/down

		//dividing by scaler (1) to normalize vectors
		//given 2 vetors you get one perpendicular to the both
		//step 2: plugging them into the matrix
		ew::Mat4 viewMatrix;

		// [r0][c0] [r0][c1] [r0][c2] [r0][c3] 
		// [r1][c0] [r1][c1] [r1][c2] [r1][c3] 
		// [r2][c0] [r2][c1] [r2][c2] [r2][c3] 
		// [r3][c0] [r3][c1] [r3][c2] [r3][c3] 

		viewMatrix[0][0] = r.x;
		viewMatrix[1][0] = r.y;
		viewMatrix[2][0] = r.z;
		viewMatrix[3][0] = -ew::Dot(r, eye);

		viewMatrix[0][1] = u.x;
		viewMatrix[1][1] = u.y;
		viewMatrix[2][1] = u.z;
		viewMatrix[3][1] = -ew::Dot(u, eye);

		viewMatrix[0][2] = -f.x;
		viewMatrix[1][2] = -f.y;
		viewMatrix[2][2] = -f.z;
		viewMatrix[3][2] = ew::Dot(f, eye);

		viewMatrix[0][3] = 0.0f;
		viewMatrix[1][3] = 0.0f;
		viewMatrix[2][3] = 0.0f;
		viewMatrix[3][3] = 1.0f;

		return viewMatrix;
	};

	// Orthographic projection
	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far) 
	{
		float width = height * aspect;
		float left = -width / 2.0f;
		float right = width / 2.0f;
		float bottom = -height / 2.0f;
		float top = height / 2.0f;

		//ew::Mat4 result = ew::Mat4::Identity();
		return ew::Mat4(
			2.0f / (right - left), 0.0f					, 0.0f				  , -(right + left) / (right - left),
			0.0f				 , 2.0f / (top - bottom), 0.0f				  , -(top + bottom) / (top - bottom),
			0.0f				 , 0.0f					, -2.0f / (far - near), -(far + near) / (far - near),
			0.0f				 , 0.0f					, 0.0f				  , 1.0f
		);
	};

	// Perspective projection
	// fov = vertical aspect ratio (radians)
	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far) 
	{
		float tanHalfFOV = tan(fov * 0.5f);

		return ew::Mat4(
			1.0f / ((aspect * tanHalfFOV) * aspect), 0.0f			   , 0.0f					, 0.0f,
			0.0f								   , 1.0f / tanHalfFOV , 0.0f					, 0.0f,
			0.0f						   		   , 0.0f			   , (near+far) / (near-far), (2.0f * far * near) / (near-far),
			0.0f								   , 0.0f			   ,-1.0f				    , 0.0f
		);
	};

	//inline ew::Mat4 LookAt(const ew::Vec3& eyePos, const ew::Vec3& targetPos, const ew::Vec3& up) 
	//{
	//	ew::Vec3 f = ew::Normalize(eyePos - targetPos);
	//	ew::Vec3 r = ew::Normalize(ew::Cross(up, f));
	//	ew::Vec3 u = ew::Normalize(ew::Cross(f,r));
	//	ew::Mat4 m = ew::Mat4(
	//		r.x, r.y, r.z, -ew::Dot(r, eyePos),
	//		u.x, u.y, u.z, -ew::Dot(u, eyePos),
	//		f.x, f.y, f.z, -ew::Dot(f, eyePos),
	//		0.0, 0.0, 0.0, 1.0);
	//	return m;
	//}

	//inline ew::Mat4 Perspective(float fov, float a, float n, float f) {
	//	float c = tanf(fov / 2.0f);
	//	Mat4 m = Mat4(0);
	//	m[0][0] = 1.0f / (c * a); //Scale X
	//	m[1][1] = 1.0f / c; //Scale Y
	//	m[2][2] = (f + n) / (n-f); //Scale Z
	//	m[3][2] = (2 * f * n) / (n-f); //Translate Z
	//	m[2][3] = -1.0f; //Perspective divide (puts Z in W component of vector)
	//	return m;
	//}

	//inline ew::Mat4 Orthographic(float height, float a, float n, float f) {
	//	//Symmetrical bounds based on aspect ratio
	//	float t = height / 2;
	//	float b = -t;
	//	float r = (height * a) / 2;
	//	float l = -r;

	//	Mat4 m = Mat4(0);
	//	m[0][0] = 2 / (r - l);
	//	m[1][1] = 2 / (t - b);
	//	m[2][2] = -2 / (f - n);
	//	m[3][0] = -(r + l) / (r - l);
	//	m[3][1] = -(t + b) / (t - b);
	//	m[3][2] = -(f + n) / (f - n);
	//	m[3][3] = 1.0f;
	//	return m;
	//}
}
