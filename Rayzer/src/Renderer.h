#pragma once
#include "memory"
#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Ray.h"
#include "Sphere.h"
#include "Light.h"
#include "Scene.h"


class Renderer
{
public:
	Renderer();
	void Render(int width, int height);
	std::shared_ptr<Walnut::Image> GetFinalImage();

private:
	glm::vec4 RenderPixel(glm::vec2 coordinate);
	glm::vec4 TraceRay(Ray& ray, int numBounces);
	void Resize(int width, int height);
	uint32_t ColorToRgba(glm::vec4 color);

private:
	std::shared_ptr<Walnut::Image> finalImage;
	uint32_t* imageData;
	int bounces;
	glm::vec4 backgroundColor;
};

