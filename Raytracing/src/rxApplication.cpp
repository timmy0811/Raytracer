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

	ImVec4 getIntersection(Ray ray);
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