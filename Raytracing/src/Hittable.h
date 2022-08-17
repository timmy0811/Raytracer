#pragma once

#include "glm/glm.hpp"
#include <memory>

#include "DirectLight.h"

class Hittable
{
protected:
	glm::vec3 position;
	glm::vec4 color;

public:
	Hittable(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 1.f));

	virtual glm::vec4 intersect(glm::vec2 coords, DirectLight light, glm::vec3 rayOrigin) = 0;
	glm::vec3* getPosition();
};

