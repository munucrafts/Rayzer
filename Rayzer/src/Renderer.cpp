#include "Renderer.h"

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

	// ==============================
	// Raytracing Core Equations:
	//
	// Ray Equation:
	//    => hitPoint = rayOrigin + hitDistance * rayDirection 
	//	  => P = O + t * D
	//
	// Sphere Equation:
	//    => (x)^2 + (y)^2 + (z)^2 = r^2
	// 
	// Substituting Ray into Sphere:
	//    => (Ox + t * Dx)^2 + (Oy + t * Dy)^2 + (Oz + t * Dz)^2 = r^2
	//    => (Dx^2 + Dy^2 + Dz^2) t^2 + 2 (OxDx + OyDy + OzDz) t + (Ox^2 + Oy^2 + Oz^2 - r^2) = 0
	//    => (a) t^2 + (b) t + (c) = 0
	//
	// Where:
	//    a = dot(rayDirection, rayDirection)
	//    b = 2 * dot(rayOrigin, rayDirection)
	//    c = dot(rayOrigin, rayOrigin) - r^2
	// 
	// Discriminant:
	//    discriminant = b^2 - 4ac
	//
	//    If discriminant < 0 → No real solutions → Ray misses the sphere
	//    If discriminant = 0 → One solution → Ray grazes (touches) the sphere
	//    If discriminant > 0 → Two solutions → Ray hits sphere at two points (entry and exit)
	//
	// Solution for t (hit distance):
	//    t = (-b ± sqrt(discriminant)) / (2a)
	//
	// ==============================

	glm::vec3 rayOrigin(0.0f, 0.0f, -1.0f);
	glm::vec3 rayDirection(coordinate.x, coordinate.y, -1.0f);

	float radius = 0.5f;

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	float discriminant = b * b - 4.0f * a * c;

	if (discriminant < 0.0f)
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float t =  (-b - glm::sqrt(discriminant)) / (2.0f * a);
	glm::vec3 hitPoint = rayDirection * t + rayOrigin;
	glm::vec3 normal = glm::normalize(hitPoint);

	glm::vec3 lightDirection(-1.0f, -1.0f, -1.0f);
	lightDirection = glm::normalize(lightDirection);
	float angle = glm::max(glm::dot(normal, -lightDirection), 0.0f);

	glm::vec3 sphereColor = glm::vec3(1.0f, 0.0f, 1.0f);
	sphereColor *= angle;

	return glm::vec4(sphereColor, 1.0f);
}


