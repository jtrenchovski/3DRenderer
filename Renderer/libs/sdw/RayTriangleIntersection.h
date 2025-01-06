#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include "ModelTriangle.h"

struct RayTriangleIntersection {
	glm::vec3 intersectionPoint;
	float distanceFromCamera = std::numeric_limits<float>::max();
	ModelTriangle intersectedTriangle;
	size_t triangleIndex;
	glm::vec3 barycentric;

	RayTriangleIntersection();
	RayTriangleIntersection(const glm::vec3 &point, float distance, const ModelTriangle &triangle, size_t index, glm::vec3 barycentric);
	friend std::ostream &operator<<(std::ostream &os, const RayTriangleIntersection &intersection);
};
