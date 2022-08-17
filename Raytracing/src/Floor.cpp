#include "Floor.h"

Floor::Floor(float height, glm::vec4 color)
    :Hittable::Hittable(glm::vec3(0.f, height, 0.f), color)
{
}

glm::vec4 Floor::intersect(glm::vec2 coords, DirectLight light, glm::vec3 rayOrigin)
{
    return glm::vec4();
}
