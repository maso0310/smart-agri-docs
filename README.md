# 智慧農業課程技術文件中心

提供智慧農業課程所有技術文件的 Web 服務。

**URL 路徑前綴**：`/smart-agri-docs`

---

## 路由說明

此應用使用 `DispatcherMiddleware` 掛載於 `/smart-agri-docs` 路徑：

- **生產環境**：`http://your-server.com/smart-agri-docs/`
- **開發環境**：`http://localhost:5000/`（直接訪問，無前綴）

---

## 專案結構

```
10_技術說明文件/
├── app.py                      # Flask 主程式（使用 DispatcherMiddleware）
├── wsgi.py                     # WSGI 入口點
├── config.py                   # 應用設定
├── requirements.txt            # Python 依賴
├── static/                     # HTML 文件目錄
│   ├── 文件索引.html           # 首頁
│   ├── 關於Arduino程式常用語法.html
│   ├── 關於基礎電子學與Arduino.html
│   └── ...（其他文件）
├── Dockerfile                  # Docker 映像設定
├── docker-compose.yml          # Docker Compose 設定
├── nginx.conf                  # Nginx 反向代理設定
├── smart-agri-docs.service     # Systemd 服務設定
├── .env.example                # 環境變數範例
└── README.md                   # 本說明文件
```

---

## 快速開始

### 本地開發

```bash
# 1. 建立虛擬環境
python -m venv venv
source venv/bin/activate  # Linux/Mac
# 或 venv\Scripts\activate  # Windows

# 2. 安裝依賴
pip install -r requirements.txt

# 3. 執行開發伺服器
python app.py

# 4. 開啟瀏覽器訪問
# http://localhost:5000
```

---

## VPS 部署方式

### 方式一：Docker 部署（推薦）

**優點**：簡單快速、環境隔離、容易更新

```bash
# 1. 上傳專案到 VPS
scp -r . user@your-vps:/var/www/smart-agri-docs/

# 2. SSH 連線到 VPS
ssh user@your-vps

# 3. 進入專案目錄
cd /var/www/smart-agri-docs

# 4. 建置並啟動
docker-compose up -d

# 5. 查看狀態
docker-compose ps
docker-compose logs -f
```

**更新文件：**
```bash
# 只更新 HTML 檔案
scp -r static/*.html user@your-vps:/var/www/smart-agri-docs/static/

# 重啟服務（可選，純靜態檔案不需重啟）
docker-compose restart
```

**使用 Nginx 反向代理：**
```bash
docker-compose --profile with-nginx up -d
```

---

### 方式二：傳統部署（使用 Gunicorn + Systemd）

適合沒有 Docker 的環境。

```bash
# 1. 上傳專案
scp -r . user@your-vps:/var/www/smart-agri-docs/

# 2. SSH 連線到 VPS
ssh user@your-vps
cd /var/www/smart-agri-docs

# 3. 建立虛擬環境並安裝依賴
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt

# 4. 設定環境變數
cp .env.example .env
nano .env  # 編輯設定

# 5. 安裝 Systemd 服務
sudo cp smart-agri-docs.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable smart-agri-docs
sudo systemctl start smart-agri-docs

# 6. 查看狀態
sudo systemctl status smart-agri-docs
```

**設定 Nginx：**
```bash
# 複製 Nginx 設定
sudo cp nginx.conf /etc/nginx/sites-available/smart-agri-docs
sudo ln -s /etc/nginx/sites-available/smart-agri-docs /etc/nginx/sites-enabled/
sudo nginx -t
sudo systemctl reload nginx
```

---

### 方式三：簡易 Python 伺服器

適合臨時測試，不建議用於生產環境。

```bash
cd /var/www/smart-agri-docs
python3 -m http.server 8000 --directory static
```

---

## 更新文件

### 從本地同步到 VPS

```bash
# 使用 rsync（推薦，只傳輸變更的檔案）
rsync -avz --delete static/ user@your-vps:/var/www/smart-agri-docs/static/

# 或使用 scp
scp -r static/*.html user@your-vps:/var/www/smart-agri-docs/static/
```

### 自動化部署腳本

建立 `deploy.sh`：

```bash
#!/bin/bash
# 部署腳本

VPS_USER="your-username"
VPS_HOST="your-vps-ip"
VPS_PATH="/var/www/smart-agri-docs"

echo "正在同步文件到 VPS..."
rsync -avz --delete static/ ${VPS_USER}@${VPS_HOST}:${VPS_PATH}/static/

echo "完成！"
```

---

## 設定 HTTPS（可選）

### 使用 Let's Encrypt

```bash
# 安裝 Certbot
sudo apt install certbot python3-certbot-nginx

# 取得證書
sudo certbot --nginx -d your-domain.com

# 自動更新
sudo certbot renew --dry-run
```

---

## 常見問題

### Q1: 網站無法訪問？

1. 檢查防火牆是否開放 5000/80/443 埠
   ```bash
   sudo ufw allow 5000
   sudo ufw allow 80
   sudo ufw allow 443
   ```

2. 檢查服務狀態
   ```bash
   docker-compose ps
   # 或
   sudo systemctl status smart-agri-docs
   ```

### Q2: 如何查看日誌？

```bash
# Docker
docker-compose logs -f

# Systemd
sudo journalctl -u smart-agri-docs -f
```

### Q3: 更新後沒有生效？

清除瀏覽器快取，或強制重新整理（Ctrl+F5）

---

## 技術規格

- **Python**: 3.8+
- **Flask**: 2.3+
- **WSGI Server**: Gunicorn
- **容器**: Docker（可選）
- **反向代理**: Nginx（可選）

---

## 授權

智慧農業課程教材 © 2024-2025
