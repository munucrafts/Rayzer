#pragma once
#include "Ray.h"
#include "Primitive.h"

class Sphere : public Primitive
{
public:
	Sphere() = default;
	virtual HitResult Intersection(Ray& ray) override;

public:
	float radius;
};

