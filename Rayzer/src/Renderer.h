#pragma once
#include "memory"
#include "Walnut/Image.h"
#include "Walnut/Random.h"


class Renderer
{
public:
	Renderer() = default;
	void Render(int width, int height);
	std::shared_ptr<Walnut::Image> GetFinalImage();

private:
	glm::vec4 RenderPixel(glm::vec2 coordinate);
	void Resize(int width, int height);
	uint32_t ColorToRgba(glm::vec4 color);

private:
	std::shared_ptr<Walnut::Image> finalImage;
	uint32_t* imageData = NULL;
};

