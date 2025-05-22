#include "Renderer.h"
#include <iostream>
#include "Cube.h"

Renderer::Renderer()
{
	bounces = 4;
	backgroundColor = { 0.53f, 0.81f, 0.98f, 1.0f };
	imageData = 0;
	finalImage = nullptr;
	accumulationData = nullptr;
	shouldAccumulate = true;
	frameIndex = 1;
}

void Renderer::Render(int width, int height)
{
	Resize(width, height);

	if (frameIndex == 1)
		memset(accumulationData, 0, sizeof(glm::vec4) * finalImage->GetWidth() * finalImage->GetHeight());

	for (int y = 0; y < finalImage->GetHeight(); y++)
	{
		for (int x = 0; x < finalImage->GetWidth(); x++)
		{
			glm::vec2 coord = glm::vec2(x / (float)finalImage->GetWidth(), y / (float)finalImage->GetHeight());
			coord = coord * 2.0f - 1.0f;
			coord.x *= (float)finalImage->GetWidth() / (float)finalImage->GetHeight();

			glm::vec4 color = RenderPixel(coord);
			accumulationData[x + y * finalImage->GetWidth()] += color;

			glm::vec4 accumulatedColor = accumulationData[x + y * finalImage->GetWidth()];
			accumulatedColor /= (float)frameIndex;
			accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));

			imageData[x + y * finalImage->GetWidth()] = ColorToRgba(accumulatedColor);
		}
	}

	finalImage->SetData(imageData);

	frameIndex = shouldAccumulate ? frameIndex + 1 : 1;
}


void Renderer::Resize(int width, int height)
{
	if (finalImage)
	{
		if (finalImage->GetWidth() == width && finalImage->GetHeight() == height)
			return;

		finalImage->Resize(width, height);
	}
	else
	{
		finalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] imageData;
	imageData = new uint32_t[width * height];

	delete[] accumulationData;
	accumulationData = new glm::vec4[width * height];
}

uint32_t Renderer::ColorToRgba(glm::vec4 color)
{
	glm::vec4 clampedColor = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
	clampedColor *= 255.f;

	uint8_t r = (int)clampedColor.x;
	uint8_t g = (int)clampedColor.y;
	uint8_t b = (int)clampedColor.z;
	uint8_t a = (int)clampedColor.a;

	uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;

	return result;
}

std::shared_ptr<Walnut::Image> Renderer::GetFinalImage()
{
	return finalImage;
}

glm::vec4 Renderer::RenderPixel(glm::vec2 coordinate)
{
	Ray ray;
	ray.origin = { 0.0f, 0.0f, -1.0f };
	ray.direction = glm::normalize(glm::vec3(coordinate, -1.0f));

	return TraceRay(ray, bounces);
}

glm::vec4 Renderer::TraceRay(Ray& ray, int numBounces)
{
    Scene activeScene;

	// Spheres
	Sphere sphere1;
	sphere1.origin = glm::vec3(-5.0f, 0.0f, -20.0f); 
	sphere1.radius = 2.0f;
	sphere1.mat.color = glm::vec3(1.0f, 1.0f, 0.0f);
	sphere1.mat.roughness = 0.1f;
	sphere1.mat.emmisiveColor = glm::vec3(0.2f, 0.6f, 1.0f);
	sphere1.mat.emmisivePower = 0.0f;

	Sphere sphere2;
	sphere2.origin = glm::vec3(5.0f, 0.0f, -20.0f); 
	sphere2.radius = 2.0f;
	sphere2.mat.color = glm::vec3(0.6f, 0.0f, 1.0f); 
	sphere2.mat.roughness = 0.2f;
	sphere2.mat.emmisiveColor = glm::vec3(1.0f, 0.3f, 0.5f);
	sphere2.mat.emmisivePower = 1.5f;


	// Floor (Bottom)
	Cube cubeBottom;
	cubeBottom.origin = glm::vec3(0.0f, -8.0f, -10.0f);
	cubeBottom.size = glm::vec3(100.0f, 10.0f, 100.0f);
	cubeBottom.mat.color = glm::vec3(1.0f, 0.7f, 0.4f);
	cubeBottom.mat.roughness = 0.3f;
	cubeBottom.mat.emmisiveColor = glm::vec3(0.4f, 0.4f, 0.4f);
	cubeBottom.mat.emmisivePower = 0.0f;

	// Ceiling (Top)
	Cube cubeTop;
	cubeTop.origin = glm::vec3(0.0f, 42.0f, -10.0f);
	cubeTop.size = glm::vec3(100.0f, 10.0f, 100.0f);
	cubeTop.mat.color = glm::vec3(0.0f, 0.2f, 0.4f);  
	cubeTop.mat.roughness = 0.0f;
	cubeTop.mat.emmisiveColor = glm::vec3(0.0f, 0.3f, 0.6f);
	cubeTop.mat.emmisivePower = 1.0f;

	// Left Wall
	Cube cubeLeft;
	cubeLeft.origin = glm::vec3(-50.0f, 17.0f, -10.0f);
	cubeLeft.size = glm::vec3(10.0f, 50.0f, 100.0f);
	cubeLeft.mat.color = glm::vec3(0.0f, 1.0f, 0.2f); 
	cubeLeft.mat.roughness = 0.2f;
	cubeLeft.mat.emmisiveColor = glm::vec3(0.1f, 0.8f, 0.3f);
	cubeLeft.mat.emmisivePower = 1.0f;

	// Right Wall
	Cube cubeRight;
	cubeRight.origin = glm::vec3(50.0f, 17.0f, -10.0f);
	cubeRight.size = glm::vec3(10.0f, 50.0f, 100.0f);
	cubeRight.mat.color = glm::vec3(1.0f, 0.0f, 0.2f); 
	cubeRight.mat.roughness = 0.2f;
	cubeRight.mat.emmisiveColor = glm::vec3(0.8f, 0.2f, 0.3f);
	cubeRight.mat.emmisivePower = 1.0f;

	// Back Wall
	Cube cubeBack;
	cubeBack.origin = glm::vec3(0.0f, 17.0f, -60.0f);
	cubeBack.size = glm::vec3(100.0f, 50.0f, 10.0f);
	cubeBack.mat.color = glm::vec3(0.0f, 0.5f, 1.0f);  
	cubeBack.mat.roughness = 0.2f;
	cubeBack.mat.emmisiveColor = glm::vec3(0.1f, 0.6f, 1.0f);
	cubeBack.mat.emmisivePower = 1.0f;

	// Front Wall
	Cube cubeFront;
	cubeFront.origin = glm::vec3(0.0f, 17.0f, 40.0f);
	cubeFront.size = glm::vec3(100.0f, 50.0f, 10.0f);
	cubeFront.mat.color = glm::vec3(1.0f, 0.5f, 0.0f);  // Orange
	cubeFront.mat.roughness = 0.2f;
	cubeFront.mat.emmisiveColor = glm::vec3(1.0f, 0.6f, 0.1f);
	cubeFront.mat.emmisivePower = 0.0f;

	// Add all shapes to scene
	activeScene.shapes.push_back(&sphere1);
	activeScene.shapes.push_back(&sphere2);
	activeScene.shapes.push_back(&cubeBottom);
	activeScene.shapes.push_back(&cubeTop);
	activeScene.shapes.push_back(&cubeRight);
	activeScene.shapes.push_back(&cubeLeft);
	activeScene.shapes.push_back(&cubeBack);
	activeScene.shapes.push_back(&cubeFront);


    if (numBounces <= 0 || activeScene.shapes.empty())
        return backgroundColor;

    float closestHitDistance = FLT_MAX;
    HitResult closestHitRes;
    Primitive* closestHitShape = nullptr;

    for (Primitive* shape : activeScene.shapes)
    {
        HitResult hitRes = shape->Intersection(ray);

        if (!hitRes.hit)
            continue;

        if (hitRes.hitDistance < closestHitDistance)
        {
            closestHitShape = shape;
            closestHitDistance = hitRes.hitDistance;
            closestHitRes = hitRes;
        }
    }

    if (closestHitShape == nullptr)
        return backgroundColor;

    glm::vec3 shapeColor(1.0f);
    bool wantLight = true;

    if (wantLight)
    {
        Light light;
        light.lightDirection = glm::vec3(0.0f, -20.0f, -1.0f);
        light.intensity = 1.0f;
        float angle = light.GetLightIntensityAngle(closestHitRes.normal);
        shapeColor = closestHitShape->mat.color * angle;
    }
    else
    {
        shapeColor = closestHitShape->mat.color;
    }

    bool wantEmmision = true;
    glm::vec3 emmissiveness(0.0f);

    ray.origin = closestHitRes.hitLocation + closestHitRes.normal * 0.0001f;

    if (!wantEmmision)
    {
        ray.direction = -glm::reflect(ray.direction, closestHitRes.normal
            + closestHitShape->mat.roughness
            * Walnut::Random::Vec3(-0.5f, 0.5f));
    }
    else
    {
        ray.direction = glm::normalize(closestHitRes.normal + Walnut::Random::InUnitSphere());
        emmissiveness = closestHitShape->mat.GetEmmisiveness();
    }

    glm::vec4 rayColor = TraceRay(ray, numBounces - 1);
    glm::vec3 finalColor = glm::mix(shapeColor, glm::vec3(rayColor), 0.5f);
    finalColor = glm::mix(finalColor, glm::vec3(backgroundColor), 0.1f);
    finalColor += emmissiveness;


    return glm::vec4(finalColor, 1.0f);
}

