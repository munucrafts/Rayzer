#pragma once
#include "Primitive.h"

class Cube : public Primitive
{
public:
	Cube() = default;
	virtual HitResult Intersection(Ray& ray) override;

public:
	glm::vec3 size;
};

