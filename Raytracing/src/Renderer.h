#pragma once

#include "Walnut/Image.h"

#include <memory>
#include "glm/glm.hpp"

class Renderer
{
private:
	std::shared_ptr<Walnut::Image> m_ImageRendered;
	uint32_t* m_ImageData = nullptr;

	uint32_t PerPixel(glm::vec2 coord);

public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render();

	std::shared_ptr<Walnut::Image> getImageRendered() const;
};

