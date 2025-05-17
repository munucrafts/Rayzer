#pragma once
#include "Primitive.h"

class Cube : public Primitive
{
public:
	Cube() = default;
	virtual HitResult Intersection(Ray& ray) override;

public:
	float length, width, height;
};

