#include "Hazel.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer() : Layer("ExampleLayer") {}
	void OnUpdate() override 
	{

	}

	void OnEvent(Hazel::Event& event) override
	{

	}
};

class SandBoxApplication : public Hazel::Application
{
public:
	SandBoxApplication()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::ImGuiLayer());
	}
	~SandBoxApplication()
	{

	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new SandBoxApplication();
}
