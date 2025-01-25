# SandBox项目
project(SandBox)

# 查找源文件
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/*.h")

# 添加 SandBox 可执行文件
add_executable(${PROJECT_NAME}
	${SOURCES}
)

# 包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/Hazel/src"
    "${CMAKE_SOURCE_DIR}/Hazel/vendor/spdlog/include"
)

# 连接 Hazel 库
target_link_libraries(${PROJECT_NAME} PRIVATE Hazel)

# 设置 SandBox 输出路径
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR}/${PROJECT_NAME}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR}/${PROJECT_NAME}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${OUTPUT_DIR}/${PROJECT_NAME}
)

# Windows 平台特定设置
if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()