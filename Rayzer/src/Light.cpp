#include "Light.h"

float Light::GetLightIntensityAngle(glm::vec3 normal)
{
	return (glm::max(glm::dot(normal, - glm::normalize(lightDirection)), 0.0f)) * intensity;
}
