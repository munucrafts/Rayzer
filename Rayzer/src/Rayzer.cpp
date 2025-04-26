#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Timer.h"
#include "Renderer.h"

class Rayzer : public Walnut::Layer
{
private:
	Renderer renderer;
	float renderTime = 0.0f;
	float viewWidth = 0.0f;
	float viewHeight = 0.0f;

public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Rendered in %.3fms", renderTime);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport Window");

		viewWidth = ImGui::GetContentRegionAvail().x;
		viewHeight = ImGui::GetContentRegionAvail().y;

		std::shared_ptr<Walnut::Image> image = renderer.GetFinalImage();

		if (image)
			ImGui::Image(image->GetDescriptorSet(), {(float)image->GetWidth(), (float)image->GetHeight()}, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();

		RenderApp();
	}

	void RenderApp()
	{
		Walnut::Timer timer;
		renderer.Render(viewWidth, viewHeight);
		renderTime = timer.ElapsedMillis();
	}
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Rayzer - Ray Tracer";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<Rayzer>();

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