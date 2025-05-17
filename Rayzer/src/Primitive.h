#pragma once
#include "Ray.h"

class Primitive
{
public:
	Primitive() = default;
	virtual HitResult Intersection(Ray& ray);

public:
	glm::vec3 origin;
	Material mat;
};
