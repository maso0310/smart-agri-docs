# t-SNE 快速參考卡

## 📌 一頁速查指南

---

## 🎯 核心概念

**t-SNE (t-Distributed Stochastic Neighbor Embedding)**
- 非線性降維技術
- 將高維資料視覺化為 2D/3D
- 保留局部結構（相似點聚在一起）

---

## ⚙️ 關鍵參數

### 1️⃣ Perplexity（困惑度）⭐ 最重要

| 資料量 | 建議值 |
|--------|--------|
| < 100 | 5-20 |
| 100-1K | 20-50 |
| 1K-10K | 30-100 |
| > 10K | 50-200 |

**重要**：必須測試多個值（如 5, 10, 30, 50, 100）

### 2️⃣ 其他參數

```python
TSNE(
    n_components=2,        # 降到 2 維
    perplexity=30,         # 困惑度（試多個值！）
    n_iter=1000,           # 迭代次數（至少 250）
    learning_rate=200,     # 學習率（10-1000）
    random_state=42,       # 固定隨機種子（重要！）
    init='pca'             # 初始化方法（推薦 PCA）
)
```

---

## ✅ 標準使用流程

```python
from sklearn.manifold import TSNE
from sklearn.preprocessing import StandardScaler
from sklearn.decomposition import PCA

# 1. 標準化（必須！）
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# 2. 如果特徵多，先 PCA
if X_scaled.shape[1] > 50:
    pca = PCA(n_components=50)
    X_scaled = pca.fit_transform(X_scaled)

# 3. 執行 t-SNE
tsne = TSNE(
    n_components=2,
    perplexity=30,
    n_iter=1000,
    random_state=42
)
X_tsne = tsne.fit_transform(X_scaled)

# 4. 視覺化
plt.scatter(X_tsne[:, 0], X_tsne[:, 1], c=y, cmap='tab10')
plt.show()
```

---

## ⚠️ 五大常見誤解

| ❌ 錯誤理解 | ✅ 正確理解 |
|------------|------------|
| t-SNE 揭示「真實」結構 | 只是探索性工具，一種視角 |
| 群聚大小 = 樣本數 | 不保留密度資訊 |
| 群聚間距有意義 | 只保留局部，不保留全域距離 |
| 可用於預測新資料 | 無法 transform 新資料 |
| 一個困惑度就夠 | **必須測試多個值** |

---

## ✅ t-SNE 能做的事

- ✅ 探索高維資料的群聚結構
- ✅ 視覺化複雜資料集
- ✅ 發現潛在模式（需驗證）
- ✅ 輔助理解資料分布

## ❌ t-SNE 不能做的事

- ❌ 用於機器學習特徵工程（無法轉換新資料）
- ❌ 比較群聚間的精確距離
- ❌ 推斷樣本密度
- ❌ 跨資料集比較 KL 散度

---

## 🔍 評估結果

### 觀察重點

1. **KL 散度**：越小越好（典型值 0.5-3.0）
2. **群聚清晰度**：同類別是否聚集？
3. **穩定性**：多次執行是否一致？
4. **領域知識**：結果是否合理？

### 量化評估（選用）

```python
from sklearn.metrics import silhouette_score

score = silhouette_score(X_tsne, labels)
print(f'輪廓係數: {score:.3f}')  # 接近 1 較好
```

---

## 🚀 快速除錯

| 症狀 | 原因 | 解決方法 |
|------|------|----------|
| 所有點擠成一團 | 學習率過高 | 降低 learning_rate (試 10-100) |
| 奇怪的對稱形狀 | 未標準化 | 使用 StandardScaler |
| 執行太慢 | 資料太大 | 先用 PCA 降維 |
| 結果差異很大 | 未固定種子 | 設定 random_state=42 |
| 群聚過於碎片化 | 困惑度太小 | 增加 perplexity (30-100) |

---

## 📊 與其他方法比較

| 方法 | 速度 | 可轉換新資料 | 保留結構 | 用途 |
|------|------|--------------|----------|------|
| PCA | ⚡ 快 | ✅ | 線性 | 初步探索 |
| t-SNE | 🐢 慢 | ❌ | 局部 | 視覺化 |
| UMAP | ⚡ 較快 | ✅ | 局部+全域 | 大資料 |

---

## 💡 專家建議

### DO（應該做）

✅ **總是標準化資料**
✅ **測試多個困惑度值**
✅ **固定 random_state**
✅ **高維資料先 PCA**
✅ **結合領域知識解讀**
✅ **用其他方法驗證發現**

### DON'T（不應做）

❌ 只用一個困惑度
❌ 忘記標準化
❌ 過度解讀群聚大小
❌ 比較遠離群聚的距離
❌ 僅憑 t-SNE 下結論
❌ 用於特徵工程

---

## 📚 相關檔案

### 文件

- `t-SNE降維技術完整指南.html` - 完整 HTML 文件（推薦！）
- `t-SNE降維技術完整指南.md` - Markdown 版本
- `t-SNE使用指南README.md` - 使用指南
- `HTML文件使用說明.md` - HTML 開啟方式

### 程式

- `A25_t-SNE困惑度調整分析.py` - 自動化困惑度分析
- `verify_tsne_perplexity.py` - 驗證困惑度支援
- `A22_t-SNE特徵分布圖_範例.py` - 基礎範例

---

## 🔗 延伸學習

### 必讀文章

**[How to Use t-SNE Effectively](https://distill.pub/2016/misread-tsne/)**
- 互動式教學
- 視覺化參數影響
- 常見誤用案例

### 替代工具

```bash
# 多核心加速
pip install MulticoreTSNE

# 現代化實作
pip install opentsne

# 替代方案（更快）
pip install umap-learn
```

---

## 📞 快速支援

遇到問題？檢查：

1. ✅ 資料是否標準化？
2. ✅ 有測試多個困惑度嗎？
3. ✅ random_state 有設定嗎？
4. ✅ 檢查 KL 散度是否合理（0.5-3.0）？
5. ✅ 結果是否與領域知識一致？

---

**提示**：將此頁面列印或儲存為 PDF，作為隨身參考！

**版本**：1.0 | **日期**：2025-12-30
