#include "procGen.h"
//#define PI 3.14159265359 

ew::MeshData myLib::createSphere(float radius, int numSegments)
{
	//Code Taken from assignment 7:
	//Still giving the same errors 
	//thought it was a problem in my functions but I changed and tested them

	ew::MeshData mesh;
	//VERTICES
	float thetaStep = ew::TAU / numSegments;
	float phiStep = ew::PI / numSegments;
	for (size_t row = 0; row <= numSegments; row++)
	{
		float phi = row * phiStep;
		for (size_t col = 0; col <= numSegments; col++)
		{
			float theta = thetaStep * col;
			ew::Vertex v;
			v.normal.x = cosf(theta) * sinf(phi);
			v.normal.y = cosf(phi);
			v.normal.z = sinf(theta) * sinf(phi);
			v.pos = v.normal * radius;
			v.uv.x = (float)col / numSegments;
			v.uv.y = 1.0 - ((float)row / numSegments);
			mesh.vertices.push_back(v);
		}
	}

	//INDICES
	unsigned int columns = numSegments + 1;
	unsigned int sideStart = columns;
	unsigned int poleStart = 0;
	//Top cap
	for (size_t i = 0; i < numSegments; i++)
	{
		mesh.indices.push_back(sideStart + i);
		mesh.indices.push_back(poleStart + i);
		mesh.indices.push_back(sideStart + i + 1);
	}
	//Rows of quads for sides
	for (size_t row = 1; row < numSegments - 1; row++)
	{
		for (size_t col = 0; col < numSegments; col++)
		{
			int start = row * columns + col;
			mesh.indices.push_back(start);
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns);
			mesh.indices.push_back(start + columns);
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns + 1);
		}
	}
	//Bottom cap
	poleStart = (columns * columns) - columns;
	sideStart = poleStart - columns;
	for (size_t i = 0; i < numSegments; i++)
	{
		mesh.indices.push_back(sideStart + i);
		mesh.indices.push_back(sideStart + i + 1);
		mesh.indices.push_back(poleStart + i);
	}
	return mesh;

	//My Original Code::

	//ew::MeshData sphereMeshData;

	//for (int i = 0; i <= numSegments; i++)
	//{
	//	float v = static_cast<float>(i) / static_cast<float>(numSegments);

	//	float polarAngle = v * PI; // angle from 'Pole' 0 to PI
	//	float sinPolar = sin(polarAngle);
	//	float cosPolar = cos(polarAngle);

	//	for (int j = 0; j <= numSegments; j++)
	//	{
	//		float u = static_cast<float>(j) / static_cast<float>(numSegments);

	//		float azimuthAngle = u * 2 * PI; // Angle around the sphere (0 to 2pi)
	//		float sinAzimuth = sin(azimuthAngle);
	//		float cosAzimuth = cos(azimuthAngle);

	//		ew::Vertex vertex;
	//		vertex.pos.x = radius * cosAzimuth * sinPolar; // x = radius * cos(a) * sin(p) 
	//		vertex.pos.y = radius * cosPolar; // y = r * cos(p) 
	//		vertex.pos.z = radius * sinAzimuth * sinPolar; // z = r * sin(a) * sin(p)

	//		//normalize vector
	//		float length = sqrt(vertex.pos.x * vertex.pos.x + vertex.pos.y * vertex.pos.y + vertex.pos.z * vertex.pos.z);
	//		if (length != 0)
	//		{
	//			vertex.normal.x = vertex.pos.x / length;
	//			vertex.normal.y = vertex.pos.y / length;
	//			vertex.normal.z = vertex.pos.z / length;
	//		}
	//		else
	//		{
	//			vertex.normal = ew::Vec3(0, 1, 0); // setting default normal if length is 0
	//		}
	//		sphereMeshData.vertices.push_back(vertex);
	//	}
	//}

	//for (int i = 0; i < numSegments; ++i) 
	//{
	//	for (int j = 0; j < numSegments; ++j) 
	//	{
	//		int topLeft = (i * (numSegments + 1)) + j;
	//		int topRight = topLeft + 1;
	//		int bottomLeft = topLeft + numSegments + 1;
	//		int bottomRight = bottomLeft + 1;

	//		sphereMeshData.indices.push_back(topLeft);
	//		sphereMeshData.indices.push_back(bottomLeft);
	//		sphereMeshData.indices.push_back(topRight);

	//		sphereMeshData.indices.push_back(topRight);
	//		sphereMeshData.indices.push_back(bottomLeft);
	//		sphereMeshData.indices.push_back(bottomRight);
	//	}
	//}

	//return sphereMeshData;
}

ew::MeshData myLib::createCylinder(float height, float radius, int numSegments)
{

	//My Original Code::

	//ew::MeshData cylinderMeshData;

	//float segmentHeight = height / static_cast<float>(numSegments);
	//float segmentAngle = (2 * PI) / static_cast<float>(numSegments);

	//for (int i = 0; i < numSegments; ++i) 
	//{
	//	float polarAngle = segmentAngle * i;
	//	float x = radius * cos(polarAngle);
	//	float z = radius * sin(polarAngle);

	//	for (int j = 0; j < 2; ++j) //twice for top and bottom circles
	//	{
	//		float y; 
	//		if (j == 0) 
	//		{
	//			y = -height / 2.0f;
	//		}
	//		else 
	//		{
	//			y = height / 2.0f;
	//		}

	//		ew::Vertex vertex;
	//		vertex.pos = { x, y, z };
	//		vertex.normal = { x, 0.0f, z }; // Normal is same as position for a simple example
	//		vertex.uv = { static_cast<float>(i) / numSegments, static_cast<float>(j) }; // Adjust UVs as needed

	//		cylinderMeshData.vertices.push_back(vertex);
	//	}
	//}

	//for (int i = 0; i < numSegments; ++i) 
	//{
	//	int next = (i + 1) % numSegments;
	//	cylinderMeshData.indices.push_back(i * 2);
	//	cylinderMeshData.indices.push_back(next * 2);
	//	cylinderMeshData.indices.push_back(next * 2 + 1);

	//	cylinderMeshData.indices.push_back(i * 2);
	//	cylinderMeshData.indices.push_back(next * 2 + 1);
	//	cylinderMeshData.indices.push_back(i * 2 + 1);
	//}

	//return cylinderMeshData;
}

ew::MeshData myLib::createPlane(float width, float height, int subDivisions)
{

	//My Original Code::

	//ew::MeshData planeMeshData;

	//float stepX = width / subDivisions;
	//float stepY = height / subDivisions;

	//for (int i = 0; i <= subDivisions; ++i) 
	//{
	//	for (int j = 0; j <= subDivisions; ++j) 
	//	{
	//		ew::Vertex vertex;
	//		vertex.pos = { j * stepX - (width / 2.0f), 0.0f, i * stepY - (height / 2.0f) }; 
	//		vertex.normal = { 0.0f, 1.0f, 0.0f };
	//		vertex.uv = { static_cast<float>(j) / subDivisions, static_cast<float>(i) / subDivisions }; 

	//		planeMeshData.vertices.push_back(vertex);
	//	}
	//}

	//for (int i = 0; i < subDivisions; ++i) 
	//{
	//	for (int j = 0; j < subDivisions; ++j) 
	//	{
	//		int topLeft = i * (subDivisions + 1) + j; 
	//		int topRight = topLeft + 1;  
	//		int bottomLeft = (i + 1) * (subDivisions + 1) + j; 
	//		int bottomRight = bottomLeft + 1; 

	//		planeMeshData.indices.push_back(topLeft); 
	//		planeMeshData.indices.push_back(bottomLeft); 
	//		planeMeshData.indices.push_back(topRight); 

	//		planeMeshData.indices.push_back(topRight); 
	//		planeMeshData.indices.push_back(bottomLeft); 
	//		planeMeshData.indices.push_back(bottomRight); 
	//	}
	//}

	//return planeMeshData;
}
