#include "Cube.h"
#include <utility>

HitResult Cube::Intersection(Ray& ray)
{
    HitResult hitData;

    glm::vec3 halfSize = size * 0.5f;
    glm::vec3 boxMin = origin - halfSize;
    glm::vec3 boxMax = origin + halfSize;

    float tMin = 0.0f;
    float tMax = FLT_MAX;

    for (int i = 0; i < 3; ++i)
    {
        float origin_i = ray.origin[i];
        float direction_i = ray.direction[i];
        float min_i = boxMin[i];
        float max_i = boxMax[i];

        if (direction_i == 0.0f)
        {
            if (origin_i < min_i || origin_i > max_i)
                return hitData;

            continue;
        }

        float t0 = (min_i - origin_i) / direction_i;
        float t1 = (max_i - origin_i) / direction_i;

        if (direction_i < 0.0f)
            std::swap(t0, t1);

        tMin = std::max(tMin, t0);
        tMax = std::min(tMax, t1);

        if (tMax < tMin)
            return hitData;
    }

    glm::vec3 hitPoint = ray.origin + tMin * ray.direction;

    hitData.hit = true;
    hitData.hitDistance = tMin;
    hitData.hitLocation = hitPoint;

    glm::vec3 normal(0.0f);
    float bias = 0.001f;

    if (fabs(hitPoint.x - boxMin.x) < bias)
        normal = glm::vec3(-1, 0, 0);
    else if (fabs(hitPoint.x - boxMax.x) < bias)
        normal = glm::vec3(1, 0, 0);
    else if (fabs(hitPoint.y - boxMin.y) < bias)
        normal = glm::vec3(0, -1, 0);
    else if (fabs(hitPoint.y - boxMax.y) < bias)
        normal = glm::vec3(0, 1, 0);
    else if (fabs(hitPoint.z - boxMin.z) < bias)
        normal = glm::vec3(0, 0, -1);
    else if (fabs(hitPoint.z - boxMax.z) < bias)
        normal = glm::vec3(0, 0, 1);

    hitData.normal = normal;
    hitData.hitRay = &ray;


    hitData.hit = true;
    hitData.hitDistance = tMin;
    hitData.hitLocation = hitPoint;
    hitData.normal = normal;
    hitData.hitRay = &ray;

    return hitData;
}


