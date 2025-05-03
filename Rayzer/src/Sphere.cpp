#include "Sphere.h"
#include <iostream>

HitResult Sphere::SphereRayIntersection(Ray& ray)
{
    HitResult hitData;

    // We create translatedOrigin to move out sphere back to the origin mathematically when the sphere origin is something other than vec3(0.0f)
    glm::vec3 translatedOrigin = ray.origin - origin;

    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(translatedOrigin, ray.direction);
    float c = glm::dot(translatedOrigin, translatedOrigin) - radius * radius;

    float discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0.0f) 
        return hitData;

    float t = (- b - glm::sqrt(discriminant)) / (2.0f * a);

    hitData.hit = true;
    hitData.hitLocation = ray.direction * t + ray.origin;
    hitData.normal = glm::normalize(hitData.hitLocation - origin);
    hitData.hitRay = &ray;

    return hitData;
}
