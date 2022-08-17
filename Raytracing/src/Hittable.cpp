#include "Hittable.h"

Hittable::Hittable(glm::vec3 position, glm::vec4 color)
	:position(position), color(color)
{

}

glm::vec3* Hittable::getPosition()
{
	return &position;
}
