@echo off
:: 设置构建目录名称
set BUILD_DIR=build

:: 检查并清理旧的构建目录
if exist "%BUILD_DIR%" (
    echo 检测到旧的构建目录 "%BUILD_DIR%"，正在清理...
    rd /s /q "%BUILD_DIR%"
    echo 已清理旧的构建目录。
)

:: 创建新的构建目录
mkdir "%BUILD_DIR%"
echo 已创建构建目录 "%BUILD_DIR%"。

:: 进入构建目录
cd "%BUILD_DIR%" || (echo 进入构建目录失败！ && PAUSE && exit /b 1)

:: 配置项目，生成 Visual Studio 工程文件
echo 正在配置项目...
cmake -G "Visual Studio 17 2022" .. || (echo 配置项目失败！ && PAUSE && exit /b 1)

:: 构建项目（Debug 配置）
echo 正在构建项目（Debug 配置）...
cmake --build . --config Debug || (echo 构建失败！ && PAUSE && exit /b 1)

:: 构建成功提示
echo 构建完成！
PAUSE