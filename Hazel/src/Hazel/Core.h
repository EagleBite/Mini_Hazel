#pragma once

#ifdef HZ_PLATFORM_WINDOWS
	#if HZ_DYNAMIC_LINK
		#ifdef HZ_BUILD_DLL
			#define HAZEL_API __declspec(dllexport)
		#else 
			#define HAZEL_API __declspec(dllimport)
		#endif
	#else 
		#define HAZEL_API
	#endif
#else 
	#error Hazel Engine only support Windows Now!
#endif

#ifdef NB_ENABLE_ASSERTS
	#define HZ_ASSERT(x, ...) { if(!x) {NB_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define HZ_CORE_ASSERT(x, ...) { if(!x) {NB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...) 
#endif

#define BIT(x) 1 << x

#define HZ_BIND_EVENT_FN(fun) std::bind(&fun, this, std::placeholders::_1)