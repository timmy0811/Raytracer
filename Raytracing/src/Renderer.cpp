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

glm::vec4 Renderer::PerPixel(glm::vec2 coord, float toleranceX, float toleranceY)
{
	glm::vec4 average(0.f);

	for (uint16_t sample = 0; sample < samplesPR; sample++) {
		glm::vec4 color{1.f};
		for (Hittable* hittable : hittables) {
			coord.x += randomDouble(-toleranceX, toleranceX);
			coord.y += randomDouble(-toleranceY, toleranceY);
			color = glm::clamp(hittable->intersect(coord, light, glm::vec3(0.f, 0.f, 1.f)), glm::vec4(-1.0f), glm::vec4(1.0f));

			//Sky gradient
			if (color.x == -1.f) {
				float step = (1.f / 255.f) * ((coord.y + 1.f) / 2.f);
				color.r = 1.f - ((255 - 110) * step);
				color.g = 1.f - ((255 - 202) * step);
			}
		}
		average += color;
	}
	average /= (float)samplesPR;
	return average;
}

double Renderer::randomDouble(float min, float max)
{
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
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
	float toleranceX = (1.f / (float)m_ImageRendered->GetWidth()) / 2.f;
	float toleranceY = (1.f / (float)m_ImageRendered->GetHeight()) / 2.f;

	for (unsigned int y = 0; y < m_ImageRendered->GetHeight(); y++) {
		for (unsigned int x = 0; x < m_ImageRendered->GetWidth(); x++) {
			glm::vec2 coord = { x / (float)m_ImageRendered->GetWidth(), y / (float)m_ImageRendered->GetHeight() };
			coord.x *= formatFac;
			coord = coord * 2.f - 1.f;
			m_ImageData[x + y * m_ImageRendered->GetWidth()] = Utils::ConvertToRGBA(PerPixel(coord, toleranceX, toleranceY));
		}
	}

	m_ImageRendered->SetData(m_ImageData);
}

std::shared_ptr<Walnut::Image> Renderer::getImageRendered() const
{
	return m_ImageRendered;
}
