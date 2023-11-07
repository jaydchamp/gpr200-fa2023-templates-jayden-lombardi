#pragma once
#include "../ew/mesh.h"

namespace myLib
{
	ew::MeshData createSphere(float radius, int numSegments);
	ew::MeshData createCylinderRing(ew::MeshData* meshData, float radius, int subdivisions, float y, bool sideFacing);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createPlane(float width, float height, int subDivisions);
}