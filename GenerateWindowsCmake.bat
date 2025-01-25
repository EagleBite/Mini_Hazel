@echo off
:: ���ù���Ŀ¼����
set BUILD_DIR=build

:: ��鲢����ɵĹ���Ŀ¼
if exist "%BUILD_DIR%" (
    echo ��⵽�ɵĹ���Ŀ¼ "%BUILD_DIR%"����������...
    rd /s /q "%BUILD_DIR%"
    echo ������ɵĹ���Ŀ¼��
)

:: �����µĹ���Ŀ¼
mkdir "%BUILD_DIR%"
echo �Ѵ�������Ŀ¼ "%BUILD_DIR%"��

:: ���빹��Ŀ¼
cd "%BUILD_DIR%" || (echo ���빹��Ŀ¼ʧ�ܣ� && PAUSE && exit /b 1)

:: ������Ŀ������ Visual Studio �����ļ�
echo ����������Ŀ...
cmake -G "Visual Studio 17 2022" .. || (echo ������Ŀʧ�ܣ� && PAUSE && exit /b 1)

:: ������Ŀ��Debug ���ã�
echo ���ڹ�����Ŀ��Debug ���ã�...
cmake --build . --config Debug || (echo ����ʧ�ܣ� && PAUSE && exit /b 1)

:: �����ɹ���ʾ
echo ������ɣ�
PAUSE