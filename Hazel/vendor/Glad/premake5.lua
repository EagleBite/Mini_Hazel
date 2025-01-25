project "Glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")  -- ����ļ�Ŀ¼
	objdir("bin-int/" .. outputdir .. "/%{prj.name}") -- �м��ļ�Ŀ¼

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs
	{
		"include"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"