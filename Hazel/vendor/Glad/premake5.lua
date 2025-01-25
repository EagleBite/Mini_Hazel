project "Glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")  -- 输出文件目录
	objdir("bin-int/" .. outputdir .. "/%{prj.name}") -- 中间文件目录

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