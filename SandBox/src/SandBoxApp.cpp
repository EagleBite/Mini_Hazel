#include "Hazel.h"

class SandBoxApplication : public Hazel::Application
{
public:
	SandBoxApplication()
	{

	}
	~SandBoxApplication()
	{

	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new SandBoxApplication();
}
