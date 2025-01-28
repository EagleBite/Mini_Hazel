#include "Hazel.h"
#include "EntryPoint.h"
#include "SandBox2D.h"

class SandBoxApplication : public Hazel::Application
{
public:
	SandBoxApplication() : Application()
	{
		PushLayer(new SandBox2D());
	}
	~SandBoxApplication()
	{

	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new SandBoxApplication();
}
