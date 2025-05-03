#pragma once
#include "glm/glm.hpp"

class Light
{
public:
	Light() = default;
	float GetLightIntensityAngle(glm::vec3 normal);

public:
	glm::vec3 lightDirection;
	float intensity;
};

