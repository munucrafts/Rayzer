#include "Renderer.h"
#include "Ray.h"
#include "Sphere.h"
#include "Light.h"
#include "Scene.h"

void Renderer::Render(int width, int height)
{
	Resize(width, height);

	for (int y = 0; y < finalImage->GetHeight(); y++)
	{
		for (int x = 0; x < finalImage->GetWidth(); x++)
		{
			glm::vec2 coord = glm::vec2(x / (float)finalImage->GetWidth(), y / (float)finalImage->GetHeight());
			coord = coord * 2.0f - 1.0f; 
			imageData[x + y * finalImage->GetWidth()] = ColorToRgba(RenderPixel(coord));
		}
	}

	finalImage->SetData(imageData);
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
	coordinate.x *= (float)finalImage->GetWidth() / (float)finalImage->GetHeight();

	Ray ray;
	ray.origin = { 0.0f, 0.0f, -1.0f };
	ray.direction = glm::normalize(glm::vec3(coordinate, -1.0f));

	Sphere sphere1;
	sphere1.origin = { -0.5f, 0.0f, 1.0f };
	sphere1.radius = 0.5f;
	sphere1.color = glm::vec3(0.0f, 1.0f, 0.0f);

	Sphere sphere2;
	sphere2.origin = { 0.7f, 0.0f, 3.0f };
	sphere2.radius = 1.5f;
	sphere2.color = glm::vec3(1.0f, 0.0f, 0.0f);

	Scene scene;
	scene.spheres.push_back(sphere1);
	scene.spheres.push_back(sphere2);

	float closestHitDistance = FLT_MAX;
	HitResult closestHitRes;
	Sphere* closestHitSphere = nullptr;

	for (int i = 0; i < scene.spheres.size(); i++)
	{
		HitResult hitRes = scene.spheres[i].SphereRayIntersection(ray);

		if (!hitRes.hit)
			continue;

		if (hitRes.hitDistance < closestHitDistance)
		{
			closestHitSphere = &scene.spheres[i];
			closestHitDistance = hitRes.hitDistance;
			closestHitRes = hitRes;
		}
	}

	if (closestHitSphere == nullptr)
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	Light light1;
	light1.lightDirection = glm::vec3(1.0f, 1.0f, -1.0f);
	light1.intensity = 1.25f;

	float angle1 = light1.GetLightIntensityAngle(closestHitRes.normal);

	return glm::vec4(closestHitSphere->color * angle1, 1.0f);
}




