#pragma once
#include "Ray.h"

struct Material
{
	glm::vec3 color = {1.0f, 1.0f, 1.0f};
	float roughness = 1.0f;

	float emmisivePower = 0.0f;
	glm::vec3 emmisiveColor = glm::vec3(1.0f);

	glm::vec3 GetEmmisiveness() { return emmisivePower * emmisiveColor; }
};

struct HitResult
{
	bool hit = false;
	Ray* hitRay = nullptr;
	glm::vec3 hitLocation = {};
	glm::vec3 normal = {};
	float hitDistance = -1;
};

class Sphere
{
public:
	Sphere() = default;
	HitResult SphereRayIntersection(Ray& ray);

public:
	float radius;
	glm::vec3 origin;
	Material mat;
};

