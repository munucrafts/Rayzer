#include "Renderer.h"

Renderer::Renderer()
{
	bounces = 2;
	backgroundColor = {0.6f, 0.7f, 0.9f, 1.0f};
	imageData = 0;
	finalImage = nullptr;
}

void Renderer::Render(int width, int height)
{
	Resize(width, height);

	for (int y = 0; y < finalImage->GetHeight(); y++)
	{
		for (int x = 0; x < finalImage->GetWidth(); x++)
		{
			glm::vec2 coord = glm::vec2(x / (float)finalImage->GetWidth(), y / (float)finalImage->GetHeight());
			coord = coord * 2.0f - 1.0f; 
			coord.x *= (float)finalImage->GetWidth() / (float)finalImage->GetHeight();
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
	Ray ray;
	ray.origin = { 0.0f, 0.0f, -1.0f };
	ray.direction = glm::normalize(glm::vec3(coordinate, -1.0f));

	return TraceRay(ray, bounces);
}

glm::vec4 Renderer::TraceRay(Ray& ray, int numBounces)
{
	Sphere sphere1;
	sphere1.origin = { -2.0f, 0.0f, 3.0f };
	sphere1.radius = 1.0f;
	sphere1.mat.color = glm::vec3(1.0f, 1.0f, 0.0f);
	sphere1.mat.roughness = 0.0f;
	sphere1.mat.metallic = 1.0f;

	Sphere sphere2;
	sphere2.origin = { 1.5f, 0.0f, 3.0f };
	sphere2.radius = 2.0f;
	sphere2.mat.color = glm::vec3(0.0f, 0.5f, 1.0f);
	sphere2.mat.roughness = 0.1f;
	sphere2.mat.metallic = 0.4f;

	Scene activeScene;

	activeScene.spheres.push_back(sphere1);
	activeScene.spheres.push_back(sphere2);

	if (numBounces <= 0)
		return backgroundColor;

	if (activeScene.spheres.size() == 0)
		return backgroundColor;

	float closestHitDistance = FLT_MAX;
	HitResult closestHitRes;
	Sphere* closestHitSphere = nullptr;

	for (int i = 0; i < activeScene.spheres.size(); i++)
	{
		HitResult hitRes = activeScene.spheres[i].SphereRayIntersection(ray);

		if (!hitRes.hit)
			continue;

		if (closestHitDistance > 0 && hitRes.hitDistance < closestHitDistance)
		{
			closestHitSphere = &activeScene.spheres[i];
			closestHitDistance = hitRes.hitDistance;
			closestHitRes = hitRes;
		}
	}

	if (closestHitSphere == nullptr)
		return backgroundColor;

	Light light;
	light.lightDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	light.intensity = 1.5f;

	float angle = light.GetLightIntensityAngle(closestHitRes.normal);
	glm::vec3 sphereColor = closestHitSphere->mat.color * angle;

	ray.origin = closestHitRes.hitLocation + closestHitRes.normal * 0.0001f;
	ray.direction = - glm::reflect(ray.direction, closestHitRes.normal 
					+ closestHitSphere->mat.roughness * closestHitSphere->mat.metallic
			        * Walnut::Random::Vec3(-0.5f, 0.5f));

	glm::vec4 rayColor = TraceRay(ray, numBounces - 1);
	glm::vec3 finalColor = glm::mix(sphereColor, glm::vec3(rayColor), 0.5f);
	finalColor = glm::mix(finalColor, glm::vec3(backgroundColor), 0.1f);

	return glm::vec4(finalColor, 1.0f);
}