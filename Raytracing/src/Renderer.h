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
	uint16_t samplesPR = 1;

	std::shared_ptr<Walnut::Image> m_ImageRendered;
	uint32_t* m_ImageData = nullptr;

	glm::vec4 PerPixel(glm::vec2 coord, float toleranceX, float toleranceY);

	DirectLight light = DirectLight(glm::vec3(-1.f, -1.f, -0.5f));
	std::vector<Hittable*> hittables = { new Sphere() };

	double randomDouble(float min, float max);

public:
	Renderer() = default;
	~Renderer();

	glm::vec3* SpherePos();
	glm::vec3* lightDir();
	float* lightInt();

	void OnResize(uint32_t width, uint32_t height);
	void Render();

	std::shared_ptr<Walnut::Image> getImageRendered() const;
};

