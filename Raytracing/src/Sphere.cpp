#include "Sphere.h"

Sphere::Sphere(float radius, glm::vec3 position, glm::vec4 color)
	:Hittable::Hittable(position, color), radius(radius)
{
}

glm::vec4 Sphere::intersect(glm::vec2 coords, DirectLight light, glm::vec3 rayOrigin)
{
	glm::vec3 rayDirection(coords.x, coords.y, -1.f);
	// Calculate a, b, c
	// a = (bx^2 + by^2 + bz^2)
	// b = 2axbx + 2ayby + 2azbz - bxa - byb - bzc
	// c = ax^2 + ay^2 + az^2 - axa - ayb - azc + a^2 + b^2 + c^2 - r^2

	float a = glm::dot(rayDirection, rayDirection);
	///*float b = 2.f * glm::dot(rayOrigin, rayDirection) - glm::dot(rayDirection, position);
	//float c = glm::dot(rayOrigin, rayOrigin) - glm::dot(rayOrigin, position) + glm::dot(position, position) - pow(radius, 2.f);*/

	glm::vec3 oc = rayOrigin - position;
	float b = glm::dot(oc, rayDirection) * 1.f;
	float c = glm::dot(oc, oc) - pow(radius, 2.f);

	//// Determine solutions
	///*float disc = pow(b, 2) - 4 * a * c;*/
	float disc = pow(b, 2) - a * c;

	if(disc < 0.f) 
		return glm::vec4(-1.f, 1.f, 1.f, 1.f);		// why so slow?

	//float t1 = (-b + sqrt(disc)) / (2 * a);
	//float lowerT = std::min(t1, t2);

	float t0 = (-b - glm::sqrt(disc)) / (2 * a);
	if(t0 < 0) return glm::vec4(-1.f, 1.f, 1.f, 1.f);

	glm::vec3 p = rayOrigin + rayDirection * t0;
	glm::vec3 normal = p - position;
	normal = glm::normalize(normal);

	bool front_face;
	if (glm::dot(rayDirection, normal) > 0.f) {
		front_face = false;
		normal = -normal;
	}
	else {
		front_face = true;
	}

	glm::vec3 color(normal * 0.5f + 0.5f);
	//color = this->color;
	float lightIntens = glm::max(glm::dot(normal, -light.direction), 1.f - light.intensity);

	return glm::vec4(color * lightIntens, 1.f);
	return glm::vec4(0.5f);
}
