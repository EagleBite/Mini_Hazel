#include "Hazel.h"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer() : Layer("ExampleLayer")
	{
		camera(3.0f, { 2.0f, 1.0f });
	}
	void OnUpdate() override 
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
		{
			HZ_TRACE("Key<A> Is Pressed!");
		}
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
	}
	~SandBoxApplication()
	{

	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new SandBoxApplication();
}
