#pragma once
#include "Ray.h"

struct HitResult
{
	bool hit = false;
	Ray* hitRay = nullptr;
	glm::vec3 hitLocation = {};
	glm::vec3 normal = {};
	float hitDistance = NULL;
};

class Sphere
{
public:
	Sphere() = default;
	HitResult SphereRayIntersection(Ray& ray);

public:
	float radius;
	glm::vec3 color;
	glm::vec3 origin;
};

