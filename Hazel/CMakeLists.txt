# Hazel库
project(Hazel)

# 查找源文件
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/*.h")

# 添加Hazel动态库
add_library(${PROJECT_NAME} SHARED
	${SOURCES}
)

# 包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/vendor/spdlog/include"
)

# 定义编译选项
target_compile_definitions(${PROJECT_NAME} PUBLIC
    HZ_PLATFORM_WINDOWS
    HZ_BUILD_DLL
)

# 设置 Hazel 输出路径
set_target_properties(${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR}/${PROJECT_NAME}
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR}/${PROJECT_NAME}
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${OUTPUT_DIR}/${PROJECT_NAME}
)

# Windows 平台特定配置
if(WIN32)
	target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
	target_link_libraries(${PROJECT_NAME} PUBLIC)
endif()

# Post-build复制命令
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:${PROJECT_NAME}>
    ${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR}/SandBox
)
