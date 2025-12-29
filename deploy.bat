@echo off
chcp 65001 >nul
REM ============================================
REM  智慧農業課程技術文件中心 - 部署腳本
REM ============================================

cd /d "%~dp0"

REM 載入設定檔
if exist deploy_config.bat (
    call deploy_config.bat
) else (
    echo.
    echo [錯誤] 找不到設定檔 deploy_config.bat
    echo.
    echo 請執行以下步驟：
    echo   1. 複製 deploy_config.example.bat 為 deploy_config.bat
    echo   2. 編輯 deploy_config.bat 填入你的 VPS 資訊
    echo.
    pause
    exit /b 1
)

REM 檢查設定是否正確
if "%VPS_USER%"=="your-username" (
    echo [錯誤] 請先編輯 deploy_config.bat，設定 VPS_USER
    pause
    exit /b 1
)

echo.
echo ╔══════════════════════════════════════════════╗
echo ║   智慧農業課程技術文件中心 - 部署工具        ║
echo ╠══════════════════════════════════════════════╣
echo ║  目標: %VPS_USER%@%VPS_HOST%
echo ║  路徑: %VPS_PATH%
echo ╚══════════════════════════════════════════════╝
echo.

:menu
echo ┌──────────────────────────────────────────────┐
echo │  請選擇操作：                                │
echo ├──────────────────────────────────────────────┤
echo │  [1] 快速同步 HTML 文件（最常用）            │
echo │  [2] 完整部署（上傳所有檔案 + 重建容器）     │
echo │  [3] 只上傳 Python 程式碼                    │
echo │  [4] 重啟遠端服務                            │
echo │  [5] 查看遠端日誌                            │
echo │  [6] SSH 連線到 VPS                          │
echo │  [7] 檢查遠端服務狀態                        │
echo │  [0] 離開                                    │
echo └──────────────────────────────────────────────┘
echo.
set /p choice=請輸入選項:

if "%choice%"=="1" goto sync_html
if "%choice%"=="2" goto full_deploy
if "%choice%"=="3" goto sync_python
if "%choice%"=="4" goto restart_service
if "%choice%"=="5" goto view_logs
if "%choice%"=="6" goto connect_ssh
if "%choice%"=="7" goto check_status
if "%choice%"=="0" goto end

echo.
echo [!] 無效選項，請重新選擇。
echo.
goto menu

:sync_html
echo.
echo [*] 正在同步 HTML 文件...
echo.
scp -r static\*.html %VPS_USER%@%VPS_HOST%:%VPS_PATH%/static/
if %errorlevel% equ 0 (
    echo.
    echo [OK] HTML 文件同步完成！
) else (
    echo.
    echo [!] 同步失敗，請檢查網路連線和 SSH 設定
)
echo.
pause
goto menu

:full_deploy
echo.
echo [*] 開始完整部署...
echo.
echo [1/3] 上傳所有檔案...
scp -r app.py wsgi.py config.py requirements.txt Dockerfile docker-compose.yml nginx.conf %VPS_USER%@%VPS_HOST%:%VPS_PATH%/
echo.
echo [2/3] 上傳 HTML 文件...
scp -r static\*.html %VPS_USER%@%VPS_HOST%:%VPS_PATH%/static/
echo.
echo [3/3] 重新建置 Docker 容器...
ssh %VPS_USER%@%VPS_HOST% "cd %VPS_PATH% && docker-compose up -d --build"
echo.
echo [OK] 完整部署完成！
echo.
pause
goto menu

:sync_python
echo.
echo [*] 正在同步 Python 程式碼...
echo.
scp app.py wsgi.py config.py requirements.txt %VPS_USER%@%VPS_HOST%:%VPS_PATH%/
if %errorlevel% equ 0 (
    echo.
    echo [OK] Python 程式碼同步完成！
    echo [*] 正在重啟服務...
    ssh %VPS_USER%@%VPS_HOST% "cd %VPS_PATH% && docker-compose restart"
    echo [OK] 服務已重啟！
)
echo.
pause
goto menu

:restart_service
echo.
echo [*] 正在重啟服務...
if "%DEPLOY_MODE%"=="systemd" (
    ssh %VPS_USER%@%VPS_HOST% "sudo systemctl restart smart-agri-docs"
) else (
    ssh %VPS_USER%@%VPS_HOST% "cd %VPS_PATH% && docker-compose restart"
)
echo.
echo [OK] 服務已重啟！
echo.
pause
goto menu

:view_logs
echo.
echo [*] 正在查看日誌（按 Ctrl+C 離開）...
echo.
if "%DEPLOY_MODE%"=="systemd" (
    ssh %VPS_USER%@%VPS_HOST% "sudo journalctl -u smart-agri-docs -f --tail=50"
) else (
    ssh %VPS_USER%@%VPS_HOST% "cd %VPS_PATH% && docker-compose logs -f --tail=50"
)
goto menu

:connect_ssh
echo.
echo [*] 正在連線到 VPS...
echo.
ssh %VPS_USER%@%VPS_HOST%
echo.
pause
goto menu

:check_status
echo.
echo [*] 檢查遠端服務狀態...
echo.
if "%DEPLOY_MODE%"=="systemd" (
    ssh %VPS_USER%@%VPS_HOST% "sudo systemctl status smart-agri-docs"
) else (
    ssh %VPS_USER%@%VPS_HOST% "cd %VPS_PATH% && docker-compose ps"
)
echo.
pause
goto menu

:end
echo.
echo 再見！
echo.
exit /b 0
