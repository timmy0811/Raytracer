#include "Renderer.h"

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
	uint8_t r = (uint8_t)(coord.x * 255.f);
	uint8_t g = (uint8_t)(coord.y * 255.f);

	return 0xff000000 | ( g << 8 ) | r;
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
	for (register uint32_t y = 0; y < m_ImageRendered->GetHeight(); y++) {
		for (register uint32_t x = 0; x < m_ImageRendered->GetWidth(); x++) {
			glm::vec2 coord = { x / (float)m_ImageRendered->GetWidth(), y / (float)m_ImageRendered->GetHeight() };
			m_ImageData[x + y * m_ImageRendered->GetWidth()] = PerPixel(coord);
		}
	}

	m_ImageRendered->SetData(m_ImageData);
}

std::shared_ptr<Walnut::Image> Renderer::getImageRendered() const
{
	return m_ImageRendered;
}
