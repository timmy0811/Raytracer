#pragma once

#include "Hittable.h"

class Floor : public Hittable
{
public:
	Floor(float height = 0.f, glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 1.f));

	glm::vec4 intersect(glm::vec2 coords, DirectLight light, glm::vec3 rayOrigin) override;
};

