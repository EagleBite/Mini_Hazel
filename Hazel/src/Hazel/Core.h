#pragma once

#ifdef HZ_PLATFORM_WINDOWS
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)
	#else 
		#define HAZEL_API __declspec(dllimport)
	#endif
#else 
	#error 引擎当前仅支持Windows系统 
#endif