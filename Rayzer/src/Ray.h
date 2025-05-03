#pragma once
#include "glm/glm.hpp"

class Ray
{
public:
	glm::vec3 origin;
	glm::vec3 direction;

public:
	Ray() = default;
};

