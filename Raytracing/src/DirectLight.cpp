#include "DirectLight.h"

DirectLight::DirectLight(glm::vec3 direction)
{
	// normalize?
	this->direction = direction;
}
