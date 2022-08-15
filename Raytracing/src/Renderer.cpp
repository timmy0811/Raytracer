#include "Renderer.h"

namespace Utils {

	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		if (color.r > 0) {
			int c = 0;
		}
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
	glm::vec3 sphere(0.f, 0.f, 0.f);
	glm::vec3 lightDir(0.5f, 0.7f, 0.f);

	glm::vec3 rayOrig(0.f, 0.f, 1.f);
	glm::vec3 rayDir(coord.x, coord.y, -1.f);
	rayDir = glm::normalize(rayDir);
	float radius = 0.5f;

	// Calculate a, b, c
	// a = (bx^2 + by^2 + bz^2)
	float a = glm::dot(rayDir, rayDir);

	// b = 2axbx + 2ayby + 2azbz - bxa - byb - bzc
	float b = 2 * glm::dot(rayOrig, rayDir) - rayDir.x * sphere.x - rayDir.y * sphere.y - rayDir.z * sphere.z;
	//float b = 2.f * glm::dot(rayOrig, rayDir);
	
	// c = ax^2 + ay^2 + az^2 - axa - ayb - azc - r^2
	float c = glm::dot(rayOrig, rayOrig) - pow(radius, 2.f) - rayOrig.x * sphere.x - rayOrig.y * sphere.y - rayOrig.z * sphere.z;
	//float c = glm::dot(rayOrig, rayOrig) - pow(radius, 2.f);

	// Determine solutions
	float disc = pow(b, 2) - 4 * a * c;
	if (disc > 0) {
		float t1 = (-b + sqrt(disc)) / (2 * a);
		float t2 = (-b - sqrt(disc)) / (2 * a);

		float lowerT = std::min(t1, t2);
		
		glm::vec3 p = rayOrig + rayDir * lowerT;

		glm::vec3 normal = p - sphere;
		glm::normalize(normal);

		//return glm::vec4(normal * 0.5f + 0.5f, 1.f);
		return glm::vec4(normal * 0.5f + 0.5f, 1.f);
	}
	else if (disc == 0) {
		float t1 = -b / (2 * a);

		glm::vec3 p = rayOrig + rayDir * t1;

		return glm::vec4(1.f, 1.f, 1.f, 1.f);
	}
	else {
		// Sky gradient
		float r = 1.f - ((255 - 110) * (1.f / 255.f) * ((coord.y + 1.f) / 2.f));
		float g = 1.f - ((255 - 202) * (1.f / 255.f) * ((coord.y + 1.f) / 2.f));

		return glm::vec4(r, g, 1.f, 1.f);
	}
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
	for (register uint32_t y = 0; y < m_ImageRendered->GetHeight(); y++) {
		for (register uint32_t x = 0; x < m_ImageRendered->GetWidth(); x++) {
			glm::vec2 coord = { x / (float)m_ImageRendered->GetWidth(), y / (float)m_ImageRendered->GetHeight() };
			coord.x *= formatFac;
			coord = coord * 2.f - 1.f;

			glm::vec4 color = PerPixel(coord);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_ImageRendered->GetWidth()] = Utils::ConvertToRGBA(color);
		}
	}

	m_ImageRendered->SetData(m_ImageData);
}

std::shared_ptr<Walnut::Image> Renderer::getImageRendered() const
{
	return m_ImageRendered;
}
