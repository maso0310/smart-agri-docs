#!/bin/bash
# 智慧農業課程技術文件中心 - Linux/Mac 部署腳本
# =============================================
#
# 使用前請先設定以下變數：

VPS_USER="your-username"
VPS_HOST="your-vps-ip"
VPS_PATH="/var/www/smart-agri-docs"

# 顏色設定
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "========================================"
echo " 智慧農業課程技術文件中心 - 部署腳本"
echo "========================================"
echo

# 檢查是否有設定 VPS 資訊
if [ "$VPS_USER" == "your-username" ]; then
    echo -e "${RED}[錯誤] 請先編輯此腳本，設定 VPS_USER, VPS_HOST, VPS_PATH${NC}"
    exit 1
fi

echo "部署目標: ${VPS_USER}@${VPS_HOST}:${VPS_PATH}"
echo

show_menu() {
    echo "請選擇操作："
    echo "[1] 完整部署（上傳所有檔案）"
    echo "[2] 只更新 HTML 文件（使用 rsync）"
    echo "[3] 重啟遠端服務"
    echo "[4] 查看遠端日誌"
    echo "[5] SSH 連線到 VPS"
    echo "[6] 離開"
    echo
}

full_deploy() {
    echo -e "${YELLOW}正在上傳所有檔案...${NC}"
    rsync -avz --exclude 'venv' --exclude '__pycache__' --exclude '.git' \
        ./ ${VPS_USER}@${VPS_HOST}:${VPS_PATH}/

    echo -e "${YELLOW}正在重新建置 Docker 容器...${NC}"
    ssh ${VPS_USER}@${VPS_HOST} "cd ${VPS_PATH} && docker-compose up -d --build"

    echo -e "${GREEN}完整部署完成！${NC}"
}

update_html() {
    echo -e "${YELLOW}正在同步 HTML 文件...${NC}"
    rsync -avz --delete static/ ${VPS_USER}@${VPS_HOST}:${VPS_PATH}/static/
    echo -e "${GREEN}HTML 文件更新完成！${NC}"
}

restart_service() {
    echo -e "${YELLOW}正在重啟服務...${NC}"
    ssh ${VPS_USER}@${VPS_HOST} "cd ${VPS_PATH} && docker-compose restart"
    echo -e "${GREEN}服務已重啟！${NC}"
}

view_logs() {
    echo -e "${YELLOW}正在查看日誌（按 Ctrl+C 離開）...${NC}"
    ssh ${VPS_USER}@${VPS_HOST} "cd ${VPS_PATH} && docker-compose logs -f --tail=50"
}

connect_ssh() {
    echo -e "${YELLOW}正在連線到 VPS...${NC}"
    ssh ${VPS_USER}@${VPS_HOST}
}

# 主程式
while true; do
    show_menu
    read -p "請輸入選項 (1-6): " choice
    echo

    case $choice in
        1) full_deploy ;;
        2) update_html ;;
        3) restart_service ;;
        4) view_logs ;;
        5) connect_ssh ;;
        6) echo "再見！"; exit 0 ;;
        *) echo -e "${RED}無效選項，請重新選擇。${NC}" ;;
    esac
    echo
done
