#include "Renderer.h"

namespace Utils {

	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
}

glm::vec4 Renderer::PerPixel(glm::vec2 coord)
{
	glm::vec3 baseColor(1.f, 1.f, 0.f);
	glm::vec3 rayOrigin = glm::vec3(0.f, 0.f, 1.f);
	glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
	float radius = 0.5f;

	glm::vec3 rayDirection(coord.x, coord.y, -1.f);

	float a = glm::dot(rayDirection, rayDirection);
	glm::vec3 oc = rayOrigin - position;
	float b = glm::dot(oc, rayDirection) * 1.f;
	float c = glm::dot(oc, oc) - pow(radius, 2.f);

	float disc = pow(b, 2) - a * c;
	if (disc < 0.f) return glm::vec4(0, 0, 0, 1);

	float t0 = (-b - sqrt(disc)) / (2 * a);

	glm::vec3 p = rayOrigin + rayDirection * t0;
	glm::vec3 normal = p - position;
	normal = glm::normalize(normal);

	/*bool front_face;
	if (glm::dot(rayDirection, normal) > 0.f) {
		front_face = false;
		normal = -normal;
	}
	else {
		front_face = true;
	}*/

	//glm::vec4 color = glm::vec4(normal * 0.5f + 0.5f, 1.f);
	float lightIntens = glm::max(glm::dot(normal, -light.direction), 1.f - light.intensity);

	glm::vec4 color = glm::vec4(baseColor * lightIntens, 1.f);

	/*for (Hittable* hittable : hittables) {
		color = glm::clamp(hittable->intersect(coord, light, glm::vec3(0.f, 0.f, 1.f)), glm::vec4(-1.0f), glm::vec4(1.0f));
	}*/

	//Sky gradient
	/*if (color.x == -1.f) {
		color.r = 1.f - ((255 - 110) * (1.f / 255.f) * ((coord.y + 1.f) / 2.f));
		color.g = 1.f - ((255 - 202) * (1.f / 255.f) * ((coord.y + 1.f) / 2.f));
		color.b = 1.f;
	}*/

	return color;
}

Renderer::~Renderer()
{
	for (Hittable* hit : hittables) {
		delete hit;
	}
}


glm::vec3* Renderer::SpherePos()
{
	return hittables[0]->getPosition();
}

glm::vec3* Renderer::lightDir()
{
	return &(light.direction);
}

float* Renderer::lightInt()
{
	return &(light.intensity);
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_ImageRendered) {
		if (width == m_ImageRendered->GetWidth() && height == m_ImageRendered->GetHeight()) return;
		m_ImageRendered->Resize(width, height);
	}
	else {
		m_ImageRendered = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

void Renderer::Render()
{
	float formatFac = (float)m_ImageRendered->GetWidth() / (float)m_ImageRendered->GetHeight();
	for (unsigned int y = 0; y < m_ImageRendered->GetHeight(); y++) {
		for (unsigned int x = 0; x < m_ImageRendered->GetWidth(); x++) {
			glm::vec2 coord = { x / (float)m_ImageRendered->GetWidth(), y / (float)m_ImageRendered->GetHeight() };
			coord.x *= formatFac;
			coord = coord * 2.f - 1.f;
			m_ImageData[x + y * m_ImageRendered->GetWidth()] = Utils::ConvertToRGBA(PerPixel(coord));
		}
	}

	m_ImageRendered->SetData(m_ImageData);
}

std::shared_ptr<Walnut::Image> Renderer::getImageRendered() const
{
	return m_ImageRendered;
}
