#include "Hazel.h"
#include "EntryPoint.h"
#include "EditorLayer.h"

namespace Hazel
{
	class HazelEditor : public Application
	{
	public:
		HazelEditor() : Application()
		{
			PushLayer(new EditorLayer());
		}
		~HazelEditor()
		{

		}
	};

	Hazel::Application* CreateApplication()
	{
		return new HazelEditor();
	}
}


