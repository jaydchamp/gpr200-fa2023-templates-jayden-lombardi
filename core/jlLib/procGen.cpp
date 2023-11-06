#include "procGen.h"
#define PI 3.14159265359 

ew::MeshData myLib::createSphere(float radius, int numSegments)
{
	ew::MeshData meshData;

	for (int i = 0; i <= numSegments; i++)
	{
		float v = static_cast<float>(i) / static_cast<float>(numSegments);

		float polarAngle = v * PI; // angle from 'Pole' 0 to PI
		float sinPolar = sin(polarAngle);
		float cosPolar = cos(polarAngle);

		for (int j = 0; j <= numSegments; j++)
		{
			float u = static_cast<float>(j) / static_cast<float>(numSegments);

			float azimuthAngle = u * 2 * PI; // Angle around the sphere (0 to 2pi)
			float sinAzimuth = sin(azimuthAngle);
			float cosAzimuth = cos(azimuthAngle);

			ew::Vertex vertex;
			vertex.pos.x = radius * cosAzimuth * sinPolar; // x = radius * cos(a) * sin(p) 
			vertex.pos.y = radius * cosPolar; // y = r * cos(p) 
			vertex.pos.z = radius * sinAzimuth * sinPolar; // z = r * sin(a) * sin(p)

			//normalize vector
			float length = sqrt(vertex.pos.x * vertex.pos.x + vertex.pos.y * vertex.pos.y + vertex.pos.z * vertex.pos.z);
			if (length != 0)
			{
				vertex.normal.x = vertex.pos.x / length;
				vertex.normal.y = vertex.pos.y / length;
				vertex.normal.z = vertex.pos.z / length;
			}
			else
			{
				vertex.normal = ew::Vec3(0, 1, 0); // setting default normal if length is 0
			}
			meshData.vertices.push_back(vertex); 
		}
	}

	for (int i = 0; i < numSegments; ++i) 
	{
		for (int j = 0; j < numSegments; ++j) 
		{
			int topLeft = (i * (numSegments + 1)) + j;
			int topRight = topLeft + 1;
			int bottomLeft = topLeft + numSegments + 1;
			int bottomRight = bottomLeft + 1;

			meshData.indices.push_back(topLeft);
			meshData.indices.push_back(bottomLeft); 
			meshData.indices.push_back(topRight); 

			meshData.indices.push_back(topRight); 
			meshData.indices.push_back(bottomLeft); 
			meshData.indices.push_back(bottomRight); 
		}
	}

	return meshData;
}

ew::MeshData myLib::createCylinder(float height, float radius, int numSegments)
{
	//Jayden Code
	return ew::MeshData();
}

ew::MeshData myLib::createPlane(float width, float height, int subDivisions)
{
	//Jayden Code
	return ew::MeshData();
}
