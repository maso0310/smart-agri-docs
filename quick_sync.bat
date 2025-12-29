@echo off
chcp 65001 >nul
REM ============================================
REM  快速同步 HTML 文件到 VPS
REM  雙擊此檔案即可一鍵同步
REM ============================================

cd /d "%~dp0"

REM 載入設定檔
if exist deploy_config.bat (
    call deploy_config.bat
) else (
    echo.
    echo [錯誤] 找不到設定檔 deploy_config.bat
    echo 請先複製 deploy_config.example.bat 為 deploy_config.bat 並填入 VPS 資訊
    echo.
    pause
    exit /b 1
)

echo.
echo ╔══════════════════════════════════════════════╗
echo ║        快速同步 HTML 文件到 VPS              ║
echo ╚══════════════════════════════════════════════╝
echo.
echo 目標: %VPS_USER%@%VPS_HOST%:%VPS_PATH%/static/
echo.
echo [*] 正在同步 HTML 文件...
echo.

scp -r static\*.html %VPS_USER%@%VPS_HOST%:%VPS_PATH%/static/

if %errorlevel% equ 0 (
    echo.
    echo ╔══════════════════════════════════════════════╗
    echo ║  [OK] 同步完成！                             ║
    echo ╚══════════════════════════════════════════════╝
    echo.
    echo 網址: http://%VPS_HOST%/smart-agri-docs/
    echo.
) else (
    echo.
    echo [!] 同步失敗，請檢查：
    echo     1. 網路連線是否正常
    echo     2. SSH 金鑰是否已設定
    echo     3. VPS 資訊是否正確
    echo.
)

timeout /t 5
