#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
//core/myLib

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
			//Uses the function LookAt()
		}
		ew::Mat4 ProjectionMatrix() // go from View->Clip space
		{
			//Will use either Ortho or Perspective depending on the bool
		}
	}
}