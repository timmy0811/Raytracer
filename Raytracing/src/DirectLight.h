#pragma once

#include "glm/glm.hpp"

class DirectLight
{
public:
	DirectLight(glm::vec3 direction);

	glm::vec3 direction;
};

