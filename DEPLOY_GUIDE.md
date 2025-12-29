# 部署快速指南

## 一、首次設定（只需做一次）

### 步驟 1：設定 SSH 金鑰（免密碼登入）

在 Windows PowerShell 或 CMD 執行：

```powershell
# 1. 產生 SSH 金鑰（如果還沒有）
ssh-keygen -t rsa -b 4096

# 2. 將公鑰複製到 VPS
type %USERPROFILE%\.ssh\id_rsa.pub | ssh user@your-vps "cat >> ~/.ssh/authorized_keys"

# 或者手動複製：
# - 開啟 %USERPROFILE%\.ssh\id_rsa.pub
# - 複製內容
# - SSH 到 VPS，貼到 ~/.ssh/authorized_keys
```

### 步驟 2：建立設定檔

```batch
# 在專案目錄中
copy deploy_config.example.bat deploy_config.bat

# 編輯 deploy_config.bat，填入你的 VPS 資訊：
# SET VPS_USER=你的使用者名稱
# SET VPS_HOST=你的VPS_IP或網域
# SET VPS_PATH=/var/www/smart-agri-docs
```

### 步驟 3：VPS 端準備

SSH 連線到 VPS 後執行：

```bash
# 建立專案目錄
sudo mkdir -p /var/www/smart-agri-docs/static
sudo chown -R $USER:$USER /var/www/smart-agri-docs

# 安裝 Docker（如果還沒有）
curl -fsSL https://get.docker.com | sh
sudo usermod -aG docker $USER

# 重新登入讓 docker 群組生效
exit
# 重新 SSH 連線
```

---

## 二、日常同步（超簡單！）

### 方法 1：雙擊快速同步（推薦）

直接雙擊 `quick_sync.bat`，自動同步所有 HTML 文件到 VPS。

### 方法 2：使用部署選單

雙擊 `deploy.bat`，選擇需要的操作：

```
┌──────────────────────────────────────────────┐
│  [1] 快速同步 HTML 文件（最常用）            │
│  [2] 完整部署（上傳所有檔案 + 重建容器）     │
│  [3] 只上傳 Python 程式碼                    │
│  [4] 重啟遠端服務                            │
│  [5] 查看遠端日誌                            │
│  [6] SSH 連線到 VPS                          │
│  [7] 檢查遠端服務狀態                        │
└──────────────────────────────────────────────┘
```

### 方法 3：命令列快速同步

```batch
cd D:\Course\課程準備工作區\10_技術說明文件
scp -r static\*.html user@your-vps:/var/www/smart-agri-docs/static/
```

---

## 三、常見操作

### 只更新了 HTML 文件

```
雙擊 quick_sync.bat
```
或
```
deploy.bat → 選 1
```

### 修改了 Python 程式碼

```
deploy.bat → 選 3（會自動重啟服務）
```

### 首次部署或大更新

```
deploy.bat → 選 2（完整部署）
```

### 查看服務是否正常

```
deploy.bat → 選 7（檢查狀態）
```

### 服務有問題，想看日誌

```
deploy.bat → 選 5（查看日誌）
```

---

## 四、檔案說明

| 檔案 | 說明 |
|------|------|
| `deploy_config.example.bat` | 設定檔範例（請複製為 deploy_config.bat） |
| `deploy_config.bat` | 你的 VPS 設定（已加入 .gitignore） |
| `deploy.bat` | 完整部署選單 |
| `quick_sync.bat` | 一鍵同步 HTML 文件 |

---

## 五、疑難排解

### Q: 連線時一直要求輸入密碼？

設定 SSH 金鑰免密碼登入：
```powershell
ssh-keygen -t rsa -b 4096
type %USERPROFILE%\.ssh\id_rsa.pub | ssh user@vps "mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys"
```

### Q: 同步後網頁沒有更新？

1. 清除瀏覽器快取（Ctrl+Shift+R）
2. 如果還是沒更新，重啟服務：`deploy.bat → 選 4`

### Q: 連線被拒絕？

檢查 VPS 防火牆：
```bash
sudo ufw allow 22    # SSH
sudo ufw allow 80    # HTTP
sudo ufw allow 443   # HTTPS
```

### Q: Docker 容器沒有啟動？

```bash
cd /var/www/smart-agri-docs
docker-compose up -d
docker-compose logs
```

---

## 六、自動化部署（進階）

如果你想要更自動化，可以考慮：

1. **GitHub Actions**：推送到 GitHub 時自動部署
2. **Webhook**：VPS 監聽 webhook，收到通知時自動拉取更新
3. **Cron Job**：定時從 GitHub 拉取最新版本

需要設定這些進階功能，請告訴我！
