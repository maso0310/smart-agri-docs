@echo off
REM ============================================
REM  VPS 部署設定檔 - 範例
REM ============================================
REM
REM 使用方式：
REM   1. 複製此檔案為 deploy_config.bat
REM   2. 填入你的 VPS 資訊
REM   3. 執行 deploy.bat 或 quick_sync.bat
REM
REM 注意：deploy_config.bat 已加入 .gitignore，不會被上傳

REM === VPS 連線資訊 ===
SET VPS_USER=your-username
SET VPS_HOST=your-vps-ip-or-domain
SET VPS_PATH=/var/www/smart-agri-docs

REM === SSH 金鑰路徑（可選）===
REM 如果你的 SSH 金鑰不在預設位置，請設定此項
REM SET SSH_KEY=%USERPROFILE%\.ssh\id_rsa

REM === 使用 Docker 還是 Systemd ===
REM 設定為 docker 或 systemd
SET DEPLOY_MODE=docker
