#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"

namespace myLib {
	//Identity matrix
	inline ew::Mat4 Identity()
	{
		return ew::Mat4(
			//x axis, y axis, z axis, w axis
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};
	//Scale on x,y,z axes
	inline ew::Mat4 Scale(ew::Vec3 s)
	{
		return ew::Mat4(
			s.x, 0, 0, 0,
			0, s.y, 0, 0,
			0, 0, s.z, 0,
			0, 0, 0, 1
		);
	};
	//Rotation around X axis (pitch) in radians
	inline ew::Mat4 RotateX(float rad) 
	{
		float c = cos(rad);
		float s = sin(rad);
		return ew::Mat4(
			1, 0,  0, 0,
			0, c, -s, 0,
			0, s,  c, 0,
			0, 0,  0, 1
		);
	};
	//Rotation around Y axis (yaw) in radians
	inline ew::Mat4 RotateY(float rad) 
	{
		float c = cos(rad);
		float s = sin(rad);
		return ew::Mat4(
			c, 0, s, 0,
			0, 1, 0, 0,
		   -s, 0, c, 0,
			0, 0, 0, 1
		);
	};
	//Rotation around Z axis (roll) in radians
	inline ew::Mat4 RotateZ(float rad) 
	{
		float c = cos(rad);
		float s = sin(rad);
		return ew::Mat4(
			c, -s, 0, 0,
			s,  c, 0, 0,
			0,  0, 1, 0,
			0,  0, 0, 1
		);
	};
	//Translate x,y,z
	inline ew::Mat4 Translate(ew::Vec3 t) 
	{
		return ew::Mat4(
			1, 0, 0, t.x,
			0, 1, 0, t.y,
			0, 0, 1, t.z,
			0, 0, 0, 1
		);
	};

	//Creates a right handed view space
	//eye = eye (camera) position
	//target = position to look at
	//up = up axis, usually(0,1,0)
	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 up) 
	{
		//use ew::Cross for cross product!
		//step 1: constructing of the vectors: f r and u 
		//dividing by scaler (1) to normalize vectors
		//given 2 vetors you get one perpendicular to the both
		//using normalize and cross functions
		//step 2: plugging them into the matrix
	};
	//Orthographic projection
	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far) 
	{
		
	};
	//Perspective projection
	//fov = vertical aspect ratio (radians)
	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far)
	{
		
	};


	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f); //Euler Angles - Degrees
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
		ew::Mat4 getModelMatrix() const
		{
			ew::Mat4 modelMatrix = myLib::Identity(); 

			//apply scale matrix
			ew::Mat4 scaleMatrix = myLib::Scale(scale); 
			modelMatrix = modelMatrix * scaleMatrix; 

			//rotation transformations in order Z, X, Y
			ew::Mat4 rotationZMatrix = myLib::RotateZ(rotation.z); 
			ew::Mat4 rotationXMatrix = myLib::RotateX(rotation.x); 
			ew::Mat4 rotationYMatrix = myLib::RotateY(rotation.y);
			ew::Mat4 rotationMatrix = rotationZMatrix * rotationXMatrix * rotationYMatrix;
			//apply rotation matrix
			modelMatrix = modelMatrix * rotationMatrix; 

			//apply translation matrix
			ew::Mat4 translationMatrix = myLib::Translate(position); 
			modelMatrix = modelMatrix * translationMatrix; 

			return modelMatrix;
		}
	};
}
