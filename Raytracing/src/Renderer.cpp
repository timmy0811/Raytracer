#include "Renderer.h"

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
	/*uint8_t r = (uint8_t)(coord.x * 255.f);
	uint8_t g = (uint8_t)(coord.y * 255.f);

	return 0xff000000 | ( g << 8 ) | r;*/

	glm::vec3 sphere(0.f, 0.f, 0.f);

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

		glm::vec3 p;
		p.x = rayOrig.x + rayDir.x * lowerT;
		p.y = rayOrig.y + rayDir.y * lowerT;
		p.z = rayOrig.z + rayDir.z * lowerT;

		float growthZ = abs(- ((p.x - sphere.x) / std::sqrt(-pow(p.x - sphere.x, 2.f) - pow(p.y - sphere.y, 2.f) + radius * radius)));
		float growthY = abs(-((p.x - sphere.x) / std::sqrt(-pow(p.x - sphere.x, 2.f) - pow(p.z - sphere.z, 2.f) + radius * radius)));

		if (growthZ >= 1) growthZ = 1;
		if (growthY >= 1) growthY = 1;

		uint8_t b = 255 * growthZ;
		uint8_t r = 255 * growthY;

		return 0xff00ff00;
		//return p;
	}
	else if (disc == 0) {
		float t1 = -b / (2 * a);

		glm::vec3 p;
		p.x = rayOrig.x + rayDir.x * t1;
		p.y = rayOrig.y + rayDir.y * t1;
		p.z = rayOrig.z + rayDir.z * t1;

		return 0xffffff00;
		//return p;
	}
	else {
		return{};
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
			m_ImageData[x + y * m_ImageRendered->GetWidth()] = PerPixel(coord);
		}
	}

	m_ImageRendered->SetData(m_ImageData);
}

std::shared_ptr<Walnut::Image> Renderer::getImageRendered() const
{
	return m_ImageRendered;
}
