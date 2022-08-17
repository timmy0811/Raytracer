#pragma once

#include "Walnut/Image.h"

#include <memory>
#include <vector>
#include "glm/glm.hpp"

#include "Sphere.h"
#include "DirectLight.h"

class Renderer
{
private:
	std::shared_ptr<Walnut::Image> m_ImageRendered;
	uint32_t* m_ImageData = nullptr;

	glm::vec4 PerPixel(glm::vec2 coord);

	DirectLight light = DirectLight(glm::vec3(-1.f, -1.f, -0.5f));
	std::vector<Hittable*> hittables = { new Sphere() };

public:
	Renderer() = default;
	~Renderer();

	void OnResize(uint32_t width, uint32_t height);
	void Render();

	std::shared_ptr<Walnut::Image> getImageRendered() const;
};

