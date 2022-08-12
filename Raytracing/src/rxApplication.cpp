#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

#include "Renderer.h"

#include <math.h>

#define FOV 60

using namespace Walnut;

class Ray {
private:
	ImVec4 source;
	ImVec4 direction;

public:
	Ray(ImVec4 source, ImVec4 direction)
		:source(source), direction(direction)
	{
	}

	ImVec2 screenPos;

	ImVec4 getSource() { return source; }
	ImVec4 getDirection() { return direction; }
};

class Sphere {
private:
	float radius;
	ImVec4 position;

public:
	Sphere(float radius, ImVec4 position)
		:radius(radius), position(position)
	{
	}

	ImVec4 getIntersection(Ray ray) {
		ImVec4 rayPos = ray.getSource();
		ImVec4 rayDir = ray.getDirection();

		// Calculate a, b, c
		// a = (bx^2 + by^2 + bz^2)
		float a = pow(rayDir.x, 2) + pow(rayDir.y, 2) + pow(rayDir.z, 2);

		// b = 1axbx + 2ayby + 2azbz - bxa - byb - bzc
		float b = (2 * rayPos.x * rayDir.x) + (2 * rayPos.y * rayDir.y) + (2 * rayPos.z * rayDir.z) - rayDir.x * position.x - rayDir.y * position.y - rayDir.z * position.z;

		// c = ax^2 + ay^2 + az^2 - axa - ayb - azc
		float c = pow(rayPos.x, 2) + pow(rayPos.y, 2) + pow(rayPos.z, 2) - rayPos.x * position.x - rayPos.y * position.y - rayPos.z * position.z;

		// Determine solutions
		float disc = pow(b, 2) - 4 * a * c;
		if (disc > 0) {
			float t1 = (-b + sqrt(disc)) / (2 * a);
			float t2 = (-b - sqrt(disc)) / (2 * a);

			float lowerT = std::min(t1, t2);

			ImVec4 p;
			p.x = rayPos.x + rayDir.x * lowerT;
			p.y = rayPos.y + rayDir.y * lowerT;
			p.z = rayPos.z + rayDir.z * lowerT;

			return p;
		}
		else if (disc == 0) {
			float t1 = -b / (2 * a);

			ImVec4 p;
			p.x = rayPos.x + rayDir.x * t1;
			p.y = rayPos.y + rayDir.y * t1;
			p.z = rayPos.z + rayDir.z * t1;

			return p;
		}
		else {
			return{};
		}
	}
};

class ExampleLayer : public Walnut::Layer
{
private:
	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeight = 0;

	Renderer m_Renderer;

	float m_LastRenderTime = 0.f;

	void Render() {
		Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render();

		//float viewPlane = tan((120 / 2 * (180 / M_PI))) * 1 * 2;
		//float distPerPixelX = viewPlane / m_ViewportWidth;
		//float distPerPixelY = (m_ViewportHeight / m_ViewportWidth) * distPerPixelX;
		//float distPerPixelY = distPerPixelX / 2;

		//ImVec2 startPos = { -viewPlane / 2, distPerPixelY * m_ViewportHeight / 2 };
		//ImVec4 dir;

		//for (int y = 0; y < m_ViewportHeight; y++) {
		//	for (int x = 0; x < m_ViewportWidth; x++) {
		//		dir.x = -(startPos.x + x * distPerPixelX - position.x);
		//		dir.y = -(startPos.y + y * distPerPixelY - position.y);
		//		dir.z = -1;

		//		Ray* ray = new Ray(position, dir);
		//		ray->screenPos = ImVec2(x, y);
		//		rays.push_back(ray);
		//	}
		//}
		
		m_LastRenderTime = timer.ElapsedMillis();
	}

public:
	virtual void OnUIRender() override
	{
		// Render sidepanel
		ImGui::Begin("Sidepanel");
		ImGui::Text("Frametime: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render")) {
			Render();
		}
		ImGui::End();

		// Render viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = (int)ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = (int)ImGui::GetContentRegionAvail().y;

		auto image = m_Renderer.getImageRendered();
		if (image) {
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() }, ImVec2(0, 1), ImVec2(1, 0));
		}

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Raytracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}