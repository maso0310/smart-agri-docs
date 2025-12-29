# -*- coding: utf-8 -*-
"""
為所有 HTML 文件加入淺色/深色主題切換按鈕
"""

import os
import re

STATIC_DIR = os.path.join(os.path.dirname(__file__), 'static')

# 主題切換按鈕的 CSS 樣式
THEME_TOGGLE_CSS = '''
        /* 主題切換按鈕 */
        .theme-toggle {
            position: fixed;
            top: 20px;
            right: 20px;
            width: 50px;
            height: 50px;
            border-radius: 50%;
            border: none;
            cursor: pointer;
            z-index: 10000;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 24px;
            transition: all 0.3s ease;
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.3);
        }

        /* 深色模式下的按鈕（顯示太陽圖示） */
        [data-theme="dark"] .theme-toggle,
        .theme-toggle {
            background: linear-gradient(135deg, #4fd1c5, #63b3ed);
            color: #1a1a2e;
        }

        [data-theme="dark"] .theme-toggle:hover,
        .theme-toggle:hover {
            transform: scale(1.1) rotate(15deg);
            box-shadow: 0 6px 20px rgba(79, 209, 197, 0.5);
        }

        /* 淺色模式下的按鈕（顯示月亮圖示） */
        [data-theme="light"] .theme-toggle {
            background: linear-gradient(135deg, #667eea, #764ba2);
            color: #ffffff;
        }

        [data-theme="light"] .theme-toggle:hover {
            transform: scale(1.1) rotate(-15deg);
            box-shadow: 0 6px 20px rgba(102, 126, 234, 0.5);
        }

        /* 圖示切換動畫 */
        .theme-toggle .icon {
            transition: transform 0.3s ease;
        }

        .theme-toggle:active .icon {
            transform: rotate(360deg);
        }

        /* ===== 淺色主題樣式 ===== */
        [data-theme="light"] body {
            background: linear-gradient(135deg, #f5f7fa 0%, #e4e8ec 50%, #d1d5db 100%) !important;
            color: #2d3748 !important;
        }

        [data-theme="light"] .container {
            color: #2d3748;
        }

        [data-theme="light"] .header {
            background: linear-gradient(135deg, rgba(79, 209, 197, 0.15) 0%, rgba(99, 179, 237, 0.15) 100%) !important;
            border: 1px solid rgba(79, 209, 197, 0.3) !important;
        }

        [data-theme="light"] .header p,
        [data-theme="light"] .stat-label {
            color: #4a5568 !important;
        }

        [data-theme="light"] .stat-item {
            background: rgba(255, 255, 255, 0.8) !important;
            border: 1px solid rgba(79, 209, 197, 0.4) !important;
        }

        [data-theme="light"] .category-nav {
            background: rgba(255, 255, 255, 0.6) !important;
        }

        [data-theme="light"] .category-btn {
            background: rgba(255, 255, 255, 0.9) !important;
            color: #2d3748 !important;
            border: 1px solid rgba(79, 209, 197, 0.4) !important;
        }

        [data-theme="light"] .category-btn:hover,
        [data-theme="light"] .category-btn.active {
            background: linear-gradient(135deg, #4fd1c5, #63b3ed) !important;
            color: #ffffff !important;
        }

        [data-theme="light"] .doc-card {
            background: rgba(255, 255, 255, 0.9) !important;
            border: 1px solid rgba(79, 209, 197, 0.3) !important;
        }

        [data-theme="light"] .doc-card:hover {
            background: rgba(255, 255, 255, 1) !important;
            border-color: #4fd1c5 !important;
            box-shadow: 0 10px 30px rgba(79, 209, 197, 0.3) !important;
        }

        [data-theme="light"] .doc-card h3 {
            color: #2b6cb0 !important;
        }

        [data-theme="light"] .doc-card:hover h3 {
            color: #319795 !important;
        }

        [data-theme="light"] .doc-card p {
            color: #4a5568 !important;
        }

        [data-theme="light"] .learning-path {
            background: rgba(255, 255, 255, 0.8) !important;
            border: 1px solid rgba(79, 209, 197, 0.3) !important;
        }

        [data-theme="light"] .path-item {
            background: rgba(245, 247, 250, 0.9) !important;
        }

        [data-theme="light"] .path-content p {
            color: #4a5568 !important;
        }

        [data-theme="light"] .footer {
            color: #4a5568 !important;
            border-top: 1px solid rgba(79, 209, 197, 0.3) !important;
        }

        /* 其他頁面通用淺色樣式 */
        [data-theme="light"] h1,
        [data-theme="light"] h2,
        [data-theme="light"] h3,
        [data-theme="light"] h4,
        [data-theme="light"] h5,
        [data-theme="light"] h6 {
            color: #2d3748 !important;
        }

        [data-theme="light"] p,
        [data-theme="light"] li,
        [data-theme="light"] td,
        [data-theme="light"] th,
        [data-theme="light"] span {
            color: #4a5568 !important;
        }

        [data-theme="light"] a {
            color: #2b6cb0 !important;
        }

        [data-theme="light"] a:hover {
            color: #319795 !important;
        }

        [data-theme="light"] code {
            background: rgba(79, 209, 197, 0.15) !important;
            color: #2d3748 !important;
        }

        [data-theme="light"] pre {
            background: rgba(45, 55, 72, 0.05) !important;
            border: 1px solid rgba(79, 209, 197, 0.3) !important;
        }

        [data-theme="light"] pre code {
            background: transparent !important;
            color: #2d3748 !important;
        }

        [data-theme="light"] table {
            border-color: rgba(79, 209, 197, 0.3) !important;
        }

        [data-theme="light"] th {
            background: rgba(79, 209, 197, 0.2) !important;
            color: #2d3748 !important;
        }

        [data-theme="light"] tr:nth-child(even) {
            background: rgba(79, 209, 197, 0.05) !important;
        }

        [data-theme="light"] tr:hover {
            background: rgba(79, 209, 197, 0.1) !important;
        }

        [data-theme="light"] blockquote {
            background: rgba(79, 209, 197, 0.1) !important;
            border-left-color: #4fd1c5 !important;
        }

        /* 淺色模式下的提示框 */
        [data-theme="light"] .tip-box,
        [data-theme="light"] .info-box,
        [data-theme="light"] .warning-box {
            background: rgba(255, 255, 255, 0.9) !important;
        }

        /* 回首頁按鈕在淺色模式下 */
        [data-theme="light"] .home-btn {
            background: linear-gradient(135deg, #4fd1c5, #63b3ed) !important;
            color: #ffffff !important;
        }
'''

# 主題切換按鈕的 HTML
THEME_TOGGLE_HTML = '''
    <!-- 主題切換按鈕 -->
    <button class="theme-toggle" onclick="toggleTheme()" title="切換淺色/深色主題">
        <span class="icon" id="theme-icon">☀️</span>
    </button>
'''

# 主題切換的 JavaScript
THEME_TOGGLE_JS = '''
    <script>
        // 主題切換功能
        function toggleTheme() {
            const html = document.documentElement;
            const icon = document.getElementById('theme-icon');
            const currentTheme = html.getAttribute('data-theme') || 'dark';

            if (currentTheme === 'dark') {
                html.setAttribute('data-theme', 'light');
                icon.textContent = '🌙';
                localStorage.setItem('theme', 'light');
            } else {
                html.setAttribute('data-theme', 'dark');
                icon.textContent = '☀️';
                localStorage.setItem('theme', 'dark');
            }
        }

        // 頁面載入時讀取儲存的主題設定
        (function() {
            const savedTheme = localStorage.getItem('theme') || 'dark';
            const html = document.documentElement;
            const icon = document.getElementById('theme-icon');

            html.setAttribute('data-theme', savedTheme);
            if (icon) {
                icon.textContent = savedTheme === 'light' ? '🌙' : '☀️';
            }
        })();
    </script>
'''

def add_theme_toggle_to_html(filepath):
    """為單一 HTML 檔案加入主題切換功能"""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()

        # 檢查是否已經有主題切換按鈕
        if 'theme-toggle' in content:
            print(f"  [跳過] {os.path.basename(filepath)} - 已有主題切換功能")
            return False

        # 1. 在 </style> 前加入主題切換 CSS
        if '</style>' in content:
            content = content.replace('</style>', THEME_TOGGLE_CSS + '\n    </style>', 1)

        # 2. 在 <body> 後加入主題切換按鈕 HTML
        if '<body>' in content:
            content = content.replace('<body>', '<body>\n' + THEME_TOGGLE_HTML, 1)

        # 3. 在 </body> 前加入主題切換 JavaScript
        if '</body>' in content:
            content = content.replace('</body>', THEME_TOGGLE_JS + '\n</body>', 1)

        # 寫回檔案
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)

        print(f"  [完成] {os.path.basename(filepath)}")
        return True

    except Exception as e:
        print(f"  [錯誤] {os.path.basename(filepath)}: {e}")
        return False

def main():
    print("=" * 60)
    print("為所有 HTML 文件加入主題切換功能")
    print("=" * 60)
    print()

    # 取得所有 HTML 檔案
    html_files = [f for f in os.listdir(STATIC_DIR) if f.endswith('.html')]

    print(f"找到 {len(html_files)} 個 HTML 檔案")
    print()

    success_count = 0
    skip_count = 0

    for filename in sorted(html_files):
        filepath = os.path.join(STATIC_DIR, filename)
        if add_theme_toggle_to_html(filepath):
            success_count += 1
        else:
            skip_count += 1

    print()
    print("=" * 60)
    print(f"處理完成！成功: {success_count}, 跳過: {skip_count}")
    print("=" * 60)

if __name__ == '__main__':
    main()
