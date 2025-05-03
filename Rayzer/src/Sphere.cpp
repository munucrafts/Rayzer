#include "Sphere.h"
#include <iostream>

HitResult Sphere::SphereRayIntersection(Ray& ray)
{
    HitResult hitData;

    glm::vec3 translatedOrigin = ray.origin - origin;

    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(translatedOrigin, ray.direction);
    float c = glm::dot(translatedOrigin, translatedOrigin) - radius * radius;

    float discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0.0f)
        return hitData;

    float t = (-b - glm::sqrt(discriminant)) / (2.0f * a);

    glm::vec3 localHitPoint = translatedOrigin + ray.direction * t;
    glm::vec3 worldHitPoint = origin + localHitPoint;

    hitData.hit = true;
    hitData.hitLocation = worldHitPoint;
    hitData.normal = glm::normalize(worldHitPoint - origin);
    hitData.hitDistance = t;
    hitData.hitRay = &ray;

    return hitData;
}

