workspace "Hazel"
	architecture "x64"
	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	startproject "SandBox"


-- 输出路径:Debug-windows-x64
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- 包含文件路径
IncludeDir = {}
IncludeDir["spdlog"] = "Hazel/vendor/spdlog/include"
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/imgui"

include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"

project "Hazel"
	location "Hazel"
	kind "SharedLib"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	-- On:代码生成的运行库选项是MTD,静态链接MSVCRT.lib库;
	-- Off:代码生成的运行库选项是MDD,动态链接MSVCRT.dll库;打包后的exe放到另一台电脑上若无这个dll会报错
	staticruntime "Off"	

	pchheader "pch.h"
	pchsource "Hazel/src/pch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links {
		"GLFW",
		"Glad",-- Glad.lib库链接到Hazel项目中
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		buildoptions { "/utf-8" }

		defines {
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"HZ_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE" -- 让GLFW不包含OpenGL
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/SandBox")	
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "On"


project "SandBox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	staticruntime "Off"

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Hazel/src",
		"%{IncludeDir.spdlog}"
	}

	links {
		"Hazel"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		buildoptions { "/utf-8" }

		defines {
			"HZ_PLATFORM_WINDOWS"
		}

	-- 不同配置下的预定义不同
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "On"