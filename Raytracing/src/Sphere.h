#pragma once

#include "Hittable.h"

class Sphere : public Hittable 
{
private:
	float radius = 0.f;

public:
	Sphere(float radius = 0.5f, glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec4 color = glm::vec4(1.f));

	glm::vec4 intersect(glm::vec2 coords, DirectLight light, glm::vec3 rayOrigin) override;
};