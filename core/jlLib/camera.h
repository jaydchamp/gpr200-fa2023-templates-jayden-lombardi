#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/transformations.h"
//#include "transformations.h" 

namespace myLib
{
	struct Camera
	{
		ew::Vec3 position; //camera body position
		ew::Vec3 target; //position to look at

		float fov; //vertical field of view (degrees)
		float aspectRatio; // screen width divided by screen height
		float nearPlane; //near plane istance (+Z)
		float farPlane; //far plane distance (positive Z value)(+Z)
		float orthoSize; //height of orthographic frustum
		bool orthographic; //perspective or orthographic?

		ew::Mat4 ViewMatrix()//go from World->View space
		{
			return ew::LookAt(position, target, ew::Vec3(0.0f, 1.0f, 0.0f)); 
		}
		ew::Mat4 ProjectionMatrix() // go from View->Clip space
		{
			if (orthographic) {
				//Orthographic projection
				return ew::Orthographic(orthoSize, aspectRatio, nearPlane, farPlane); 
			}
			else {
				//Perspective projection
				float fovRadians = fov * 3.14159265358979323846f / 180.0f;
				return ew::Perspective(fovRadians, aspectRatio, nearPlane, farPlane);  
			}
		}
	};
}