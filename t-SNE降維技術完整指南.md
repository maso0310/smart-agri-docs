# t-SNE 降維技術完整指南

## 📚 目錄
1. [t-SNE 簡介](#tsne-簡介)
2. [核心概念](#核心概念)
3. [關鍵參數說明](#關鍵參數說明)
4. [如何有效使用 t-SNE](#如何有效使用-tsne)
5. [常見誤解與陷阱](#常見誤解與陷阱)
6. [實際應用範例](#實際應用範例)
7. [最佳實踐建議](#最佳實踐建議)

---

## t-SNE 簡介

### 什麼是 t-SNE?

**t-SNE (t-Distributed Stochastic Neighbor Embedding)** 是一種強大的降維技術,專門用於將高維度資料視覺化為 2D 或 3D 空間。它由 Laurens van der Maaten 和 Geoffrey Hinton 在 2008 年提出。

### 為什麼需要 t-SNE?

在現代機器學習與資料科學中,我們經常處理高維度資料(如影像、文字、基因序列等)。人類難以直接理解超過 3 維的空間,因此需要降維技術來:

- 🔍 **視覺化探索**: 在 2D/3D 空間中觀察資料分布
- 🎯 **發現模式**: 識別資料中的群聚(cluster)與異常點
- 📊 **品質評估**: 檢查特徵提取或分類結果的合理性
- 🧪 **研究溝通**: 製作易於理解的視覺化圖表

### t-SNE vs 其他降維方法

| 方法 | 優點 | 缺點 | 適用情境 |
|------|------|------|----------|
| **PCA** | 快速、線性、可解釋 | 只能捕捉線性關係 | 初步探索、特徵壓縮 |
| **t-SNE** | 保留局部結構、視覺效果好 | 計算慢、非線性、不保留全域結構 | 視覺化、探索性分析 |
| **UMAP** | 速度快、保留全域與局部結構 | 參數敏感 | 大規模資料視覺化 |

---

## 核心概念

### 1. 工作原理

t-SNE 透過以下步驟進行降維:

#### 步驟 1: 計算高維空間的相似度
- 對於每個資料點,計算它與其他點的**條件機率**
- 使用高斯分布來定義相似度
- 距離越近的點,相似度越高

#### 步驟 2: 定義低維空間的相似度
- 在目標 2D/3D 空間中隨機初始化點
- 使用 **t 分布**(而非高斯分布)來計算相似度
- t 分布的長尾特性可以避免"擁擠問題"

#### 步驟 3: 最小化兩個分布的差異
- 使用 **KL 散度 (Kullback-Leibler divergence)** 衡量差異
- 透過梯度下降法最佳化低維空間的點位置
- 目標:讓低維空間的相似度分布盡可能接近高維空間

### 2. 數學表示

**高維空間相似度:**
```
p(j|i) = exp(-||x_i - x_j||² / 2σ_i²) / Σ_k≠i exp(-||x_i - x_k||² / 2σ_i²)
```

**低維空間相似度:**
```
q(j|i) = (1 + ||y_i - y_j||²)⁻¹ / Σ_k≠i (1 + ||y_i - y_k||²)⁻¹
```

**目標函數 (KL散度):**
```
C = Σ_i KL(P_i || Q_i) = Σ_i Σ_j p(j|i) log(p(j|i) / q(j|i))
```

---

## 關鍵參數說明

### 1. 困惑度 (Perplexity) ⭐⭐⭐⭐⭐

**最重要的參數!** 它決定了 t-SNE 如何平衡局部與全域結構的關注程度。

#### 定義
- 困惑度可以理解為"每個點的有效近鄰數量"
- 數學上,它是條件機率分布熵的指數

#### 如何選擇困惑度?

| 資料量 | 建議困惑度範圍 | 說明 |
|--------|----------------|------|
| < 100 樣本 | 5 - 20 | 小資料集用小困惑度 |
| 100 - 1000 | 20 - 50 | 一般資料集 |
| 1000 - 10000 | 30 - 100 | 中大型資料集 |
| > 10000 | 50 - 200 | 大型資料集 |

#### 困惑度的影響

**困惑度過低 (如 5):**
- ✅ 強調非常局部的結構
- ❌ 可能將資料分割成過多小群聚
- ❌ 忽略全域結構
- 📊 **視覺特徵**: 許多分散的小群聚

**困惑度適中 (如 30-50):**
- ✅ 平衡局部與全域結構
- ✅ 群聚清晰可辨
- ✅ 保留合理的空間關係
- 📊 **視覺特徵**: 清楚的群聚分離

**困惑度過高 (如 200):**
- ✅ 關注全域結構
- ❌ 可能喪失局部細節
- ❌ 群聚可能融合在一起
- 📊 **視覺特徵**: 較少的大群聚,邊界模糊

#### 💡 實務建議
```python
# 建議測試多個困惑度值
perplexity_values = [5, 10, 30, 50, 100]

for perp in perplexity_values:
    tsne = TSNE(n_components=2, perplexity=perp, random_state=42)
    embedding = tsne.fit_transform(X)
    plot_embedding(embedding, title=f'Perplexity={perp}')
```

### 2. 學習率 (Learning Rate / Epsilon)

控制梯度下降的步長大小。

**典型範圍**: 10 - 1000
- 太小: 收斂慢,可能卡在局部最優解
- 太大: 可能不收斂,點分布混亂
- **預設值**: 200(scikit-learn),通常已足夠

**自動調整建議**:
```python
learning_rate = max(200, n_samples / 12)
```

### 3. 迭代次數 (Iterations / n_iter)

決定優化過程執行多少步。

**建議值**:
- 最少: 250 次
- 標準: 1000 次
- 複雜資料: 3000 - 5000 次

**判斷是否收斂**:
- 觀察 KL 散度是否趨於穩定
- 視覺化結果是否不再明顯變化

### 4. 初始化方法 (Initialization)

**選項**:
1. **'random'**: 隨機初始化(預設)
2. **'pca'**: 使用 PCA 結果初始化(更穩定,推薦)

```python
tsne = TSNE(init='pca', random_state=42)
```

### 5. 隨機種子 (Random State)

**關鍵重要!** t-SNE 結果具有隨機性:

```python
# ❌ 錯誤: 每次執行結果不同
tsne = TSNE(n_components=2)

# ✅ 正確: 可重現結果
tsne = TSNE(n_components=2, random_state=42)
```

---

## 如何有效使用 t-SNE

### ✅ 正確的使用方式

#### 1. 探索性資料分析
```python
from sklearn.manifold import TSNE
import matplotlib.pyplot as plt

# 標準化資料(重要!)
from sklearn.preprocessing import StandardScaler
X_scaled = StandardScaler().fit_transform(X)

# 執行 t-SNE
tsne = TSNE(n_components=2, perplexity=30,
            random_state=42, init='pca', n_iter=1000)
X_tsne = tsne.fit_transform(X_scaled)

# 視覺化
plt.scatter(X_tsne[:, 0], X_tsne[:, 1], c=labels, cmap='viridis')
plt.title('t-SNE Visualization')
plt.show()
```

#### 2. 檢查分群結果
```python
from sklearn.cluster import KMeans

# 先進行分群
kmeans = KMeans(n_clusters=5, random_state=42)
cluster_labels = kmeans.fit_predict(X)

# 用 t-SNE 視覺化分群結果
X_tsne = TSNE(n_components=2, random_state=42).fit_transform(X)

plt.scatter(X_tsne[:, 0], X_tsne[:, 1],
            c=cluster_labels, cmap='tab10')
plt.title('K-means Clustering (t-SNE Projection)')
plt.show()
```

#### 3. 比較不同特徵提取方法
```python
features_methods = {
    '原始特徵': X_raw,
    '自編碼器特徵': X_autoencoder,
    'CNN特徵': X_cnn
}

fig, axes = plt.subplots(1, 3, figsize=(15, 5))

for ax, (name, features) in zip(axes, features_methods.items()):
    X_tsne = TSNE(random_state=42).fit_transform(features)
    ax.scatter(X_tsne[:, 0], X_tsne[:, 1], c=labels, s=1)
    ax.set_title(name)

plt.show()
```

### ❌ 常見錯誤用法

#### 錯誤 1: 將 t-SNE 用於降維後的特徵工程
```python
# ❌ 錯誤: t-SNE 不保留距離關係,不適合用於下游任務
X_tsne = TSNE(n_components=2).fit_transform(X)
model.fit(X_tsne, y)  # 不要這樣做!

# ✅ 正確: 使用 PCA 或自編碼器進行特徵降維
from sklearn.decomposition import PCA
X_pca = PCA(n_components=50).fit_transform(X)
model.fit(X_pca, y)
```

#### 錯誤 2: 過度解讀距離與大小
```python
# ❌ 不要認為:
# - 群聚間的距離有意義
# - 群聚的大小反映真實數量
# - 兩次運行的結果可以直接比較

# ✅ 應該關注:
# - 資料點的局部鄰居關係
# - 是否形成清晰的群聚
# - 群聚內部的結構
```

---

## 常見誤解與陷阱

### 誤解 1: 群聚間距離有意義

**真相**: t-SNE **只保留局部結構**,不保留全域距離關係。

```
高維空間:  A群 ←10→ B群 ←100→ C群
t-SNE後:   A群 ←3→  B群 ←5→   C群  (距離被扭曲!)
```

**影響**:
- 不能透過群聚間距離判斷相似度
- 可能產生視覺上誤導的分離

### 誤解 2: 群聚大小反映資料量

**真相**: t-SNE 可能讓同樣大小的群聚看起來大小不同。

**示例**:
```python
# 兩個大小相同的群聚
cluster_A = 1000 個點  →  t-SNE後看起來很大
cluster_B = 1000 個點  →  t-SNE後看起來很小
```

**原因**:
- 密度較低的群聚會被拉伸
- 密度較高的群聚會被壓縮

### 誤解 3: 隨機噪點可能形成假性群聚

**實驗**:
```python
# 完全隨機的資料
X_random = np.random.randn(1000, 50)

# t-SNE 可能顯示出"群聚"!
X_tsne = TSNE(perplexity=30, random_state=42).fit_transform(X_random)
```

**真相**: t-SNE 傾向於將資料分成群聚,即使資料本身沒有結構。

**解決方案**:
1. 使用領域知識驗證群聚合理性
2. 搭配其他分析方法(如 PCA、階層式分群)
3. 檢查統計顯著性

### 陷阱 4: 不同運行結果無法比較

**問題**:
```python
# 第一次運行
tsne1 = TSNE(random_state=1).fit_transform(X)

# 第二次運行(不同資料或參數)
tsne2 = TSNE(random_state=2).fit_transform(X)

# ❌ tsne1 和 tsne2 的座標系統完全不同!
```

**注意事項**:
- 每次運行產生獨立的嵌入空間
- 不能直接比較兩次運行的點位置
- 群聚的位置、朝向都是隨機的

### 陷阱 5: 過度依賴單一困惑度

**建議**: **永遠測試多個困惑度值!**

```python
# 測試多個困惑度
perplexities = [5, 10, 30, 50, 100]

fig, axes = plt.subplots(1, 5, figsize=(25, 5))

for ax, perp in zip(axes, perplexities):
    tsne = TSNE(perplexity=perp, random_state=42)
    X_embedded = tsne.fit_transform(X)
    ax.scatter(X_embedded[:, 0], X_embedded[:, 1], c=labels, s=5)
    ax.set_title(f'Perplexity = {perp}')
    ax.axis('off')

plt.suptitle('不同困惑度下的 t-SNE 結果比較', fontsize=16)
plt.show()
```

**觀察重點**:
- 群聚是否在多個困惑度下都穩定出現?
- 哪個困惑度顯示最清晰的結構?
- 是否有隨困惑度改變的異常現象?

---

## 實際應用範例

### 範例 1: MNIST 手寫數字視覺化

```python
from sklearn.datasets import load_digits
from sklearn.manifold import TSNE
import matplotlib.pyplot as plt

# 載入資料
digits = load_digits()
X, y = digits.data, digits.target

# 執行 t-SNE
tsne = TSNE(n_components=2, perplexity=30,
            random_state=42, init='pca', n_iter=1000)
X_tsne = tsne.fit_transform(X)

# 視覺化
plt.figure(figsize=(10, 8))
scatter = plt.scatter(X_tsne[:, 0], X_tsne[:, 1],
                     c=y, cmap='tab10', s=5, alpha=0.6)
plt.colorbar(scatter, label='Digit Class')
plt.title('MNIST Digits - t-SNE Visualization')
plt.xlabel('t-SNE Component 1')
plt.ylabel('t-SNE Component 2')
plt.tight_layout()
plt.show()
```

### 範例 2: 農業資料期作分析

```python
# 載入農業特徵資料
import pandas as pd
from C01_設定檔 import df, X_variable_list

X = df[X_variable_list].values
periods = df['Period'].values

# 標準化
from sklearn.preprocessing import StandardScaler
X_scaled = StandardScaler().fit_transform(X)

# t-SNE 降維
tsne = TSNE(n_components=2, perplexity=50,
            random_state=42, init='pca')
X_tsne = tsne.fit_transform(X_scaled)

# 按期作別上色
unique_periods = np.unique(periods)
colors = plt.cm.jet(np.linspace(0, 1, len(unique_periods)))

plt.figure(figsize=(12, 8))
for period, color in zip(unique_periods, colors):
    mask = periods == period
    plt.scatter(X_tsne[mask, 0], X_tsne[mask, 1],
               c=[color], label=period, s=10, alpha=0.7)

plt.title('農業資料期作分布 (t-SNE)', fontsize=16)
plt.xlabel('t-SNE Component 1')
plt.ylabel('t-SNE Component 2')
plt.legend(title='Period', bbox_to_anchor=(1.05, 1))
plt.tight_layout()
plt.show()
```

### 範例 3: 影像特徵視覺化

```python
# CNN 特徵提取後的視覺化
from tensorflow.keras.applications import ResNet50
from tensorflow.keras.preprocessing import image
from tensorflow.keras.applications.resnet50 import preprocess_input

# 載入預訓練模型(不包含頂層)
model = ResNet50(weights='imagenet', include_top=False,
                pooling='avg')

# 提取特徵
features = []
for img_path in image_paths:
    img = image.load_img(img_path, target_size=(224, 224))
    x = image.img_to_array(img)
    x = np.expand_dims(x, axis=0)
    x = preprocess_input(x)
    feat = model.predict(x)
    features.append(feat.flatten())

X_features = np.array(features)

# t-SNE 視覺化
tsne = TSNE(n_components=2, perplexity=30, random_state=42)
X_tsne = tsne.fit_transform(X_features)

plt.scatter(X_tsne[:, 0], X_tsne[:, 1], c=labels)
plt.title('CNN Features - t-SNE')
plt.show()
```

---

## 最佳實踐建議

### 1. 資料前處理 ⭐⭐⭐⭐⭐

```python
# 必做: 標準化或正規化
from sklearn.preprocessing import StandardScaler
X_scaled = StandardScaler().fit_transform(X)

# 可選: 先用 PCA 降維(加速 + 降噪)
from sklearn.decomposition import PCA
if X.shape[1] > 50:
    pca = PCA(n_components=50)
    X_pca = pca.fit_transform(X_scaled)
    X_for_tsne = X_pca
else:
    X_for_tsne = X_scaled
```

### 2. 參數調整策略

**步驟 1: 快速探索**
```python
# 使用較少迭代次數快速測試
tsne_fast = TSNE(n_iter=250, perplexity=30)
```

**步驟 2: 困惑度網格搜尋**
```python
perplexities = [5, 10, 20, 30, 50, 100]
# 比較結果,選擇最清晰的
```

**步驟 3: 精細調整**
```python
# 使用最佳困惑度,增加迭代次數
tsne_final = TSNE(n_iter=5000, perplexity=best_perplexity,
                  init='pca', random_state=42)
```

### 3. 結果驗證

**檢查清單**:
- [ ] 是否測試了多個困惑度?
- [ ] 是否設定了 random_state?
- [ ] 群聚是否有領域意義?
- [ ] 是否與其他方法(PCA/UMAP)一致?
- [ ] 是否檢查了收斂性?

### 4. 視覺化技巧

```python
# 技巧 1: 使用半透明點(大數據集)
plt.scatter(X_tsne[:, 0], X_tsne[:, 1],
           alpha=0.3, s=5)

# 技巧 2: 使用密度圖(超大數據集)
from scipy.stats import gaussian_kde
density = gaussian_kde(X_tsne.T)(X_tsne.T)
plt.scatter(X_tsne[:, 0], X_tsne[:, 1],
           c=density, cmap='viridis', s=5)

# 技巧 3: 標註代表性點
for i, label in enumerate(important_labels):
    plt.annotate(label, X_tsne[i], fontsize=12)
```

### 5. 報告與解釋

**好的 t-SNE 圖應包含**:
- ✅ 參數資訊(困惑度、迭代次數)
- ✅ 資料前處理說明
- ✅ 顏色/標記的圖例
- ✅ 座標軸標籤("t-SNE Component 1/2")
- ✅ 樣本數量資訊

**解釋時應強調**:
- ✅ 局部結構(哪些點聚在一起)
- ✅ 群聚的合理性解釋
- ✅ 與領域知識的一致性
- ❌ 不要過度解讀群聚間距離

---

## 進階主題

### 1. t-SNE 的計算複雜度

**時間複雜度**: O(N²) → 對於大數據集很慢

**解決方案**:
1. **Barnes-Hut 近似**: O(N log N)
   ```python
   tsne = TSNE(method='barnes_hut')  # scikit-learn 預設
   ```

2. **先用 PCA 降維**:
   ```python
   pca = PCA(n_components=50)
   X_pca = pca.fit_transform(X)
   tsne = TSNE().fit_transform(X_pca)
   ```

3. **使用 MulticoreTSNE**:
   ```python
   from MulticoreTSNE import MulticoreTSNE as TSNE
   tsne = TSNE(n_jobs=4)
   ```

### 2. t-SNE 變體

**OpenTSNE** (推薦大規模資料):
```bash
pip install opentsne
```

```python
from openTSNE import TSNE

tsne = TSNE(n_components=2, perplexity=30,
            n_jobs=-1, random_state=42)
embedding = tsne.fit(X)
```

### 3. 與 UMAP 比較

**何時選擇 UMAP**:
- 資料量 > 10000
- 需要保留全域結構
- 需要更快的運算速度
- 需要支援新資料轉換

**何時選擇 t-SNE**:
- 視覺化效果是首要考量
- 資料量適中(< 10000)
- 只需要探索性分析

---

## 參考資源

### 論文
1. **原始論文**: van der Maaten & Hinton (2008) - "Visualizing Data using t-SNE"
2. **實用指南**: Wattenberg et al. (2016) - "How to Use t-SNE Effectively"

### 實作工具
- **scikit-learn**: 標準實作
- **OpenTSNE**: 效能優化版本
- **MulticoreTSNE**: 多核心加速版本

### 互動式視覺化
- [Distill.pub t-SNE Guide](https://distill.pub/2016/misread-tsne/)
- [t-SNE 線上互動示範](https://distill.pub/2016/misread-tsne/)

---

## 總結

### ✅ 記住這些要點

1. **t-SNE 只保留局部結構** - 群聚間距離無意義
2. **困惑度是最重要的參數** - 務必測試多個值
3. **結果具有隨機性** - 必須設定 random_state
4. **僅用於視覺化** - 不要用於下游機器學習任務
5. **需要搭配領域知識** - 不要盲目相信視覺結果

### 🎯 快速檢查表

執行 t-SNE 前:
- [ ] 資料已標準化
- [ ] 選擇合適的困惑度範圍
- [ ] 設定足夠的迭代次數(≥1000)
- [ ] 固定隨機種子
- [ ] 考慮先用 PCA 降維

解讀結果時:
- [ ] 測試了多個困惑度
- [ ] 群聚有領域意義
- [ ] 與其他方法結果一致
- [ ] 不過度解讀距離與大小

---

**最後更新**: 2025-12-30
**作者**: Claude AI for Smart Agriculture Course
**版本**: 1.0

---

## 附錄: 完整程式碼範例

請參考:
- `A22_t-SNE特徵分布圖_範例.py` - 基礎範例
- `A25_t-SNE特徵分布實際應用.py` - 實際應用
- `A25_t-SNE困惑度調整分析.py` - 困惑度比較(新增)
