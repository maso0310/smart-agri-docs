# -*- coding: utf-8 -*-
"""
統一所有 HTML 文件的主題樣式
- 返回首頁按鈕移到右下角
- 主題切換按鈕在右上角
- 統一淺色/深色主題樣式
"""

import os
import re

STATIC_DIR = os.path.join(os.path.dirname(__file__), 'static')

# 新的返回首頁按鈕樣式（右下角）
HOME_BUTTON_CSS = '''
        /* 返回首頁按鈕 - 右下角 */
        .floating-home {
            position: fixed;
            bottom: 25px;
            right: 25px;
            z-index: 9999;
            display: flex;
            align-items: center;
            justify-content: center;
            width: 55px;
            height: 55px;
            background: linear-gradient(135deg, #4fd1c5 0%, #63b3ed 100%);
            color: #1a1a2e;
            text-decoration: none;
            border-radius: 50%;
            box-shadow: 0 4px 15px rgba(79, 209, 197, 0.4);
            font-size: 24px;
            transition: all 0.3s ease;
        }
        .floating-home:hover {
            transform: scale(1.1);
            box-shadow: 0 6px 25px rgba(79, 209, 197, 0.6);
        }
        .floating-home:active {
            transform: scale(0.95);
        }
        /* 淺色模式下的返回首頁按鈕 */
        [data-theme="light"] .floating-home {
            background: linear-gradient(135deg, #4fd1c5 0%, #63b3ed 100%);
            color: #ffffff;
            box-shadow: 0 4px 15px rgba(79, 209, 197, 0.5);
        }
'''

# 統一的主題切換按鈕樣式
THEME_TOGGLE_CSS = '''
        /* 主題切換按鈕 - 右上角 */
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
        [data-theme="light"] .theme-toggle {
            background: linear-gradient(135deg, #667eea, #764ba2);
            color: #ffffff;
        }
        [data-theme="light"] .theme-toggle:hover {
            transform: scale(1.1) rotate(-15deg);
            box-shadow: 0 6px 20px rgba(102, 126, 234, 0.5);
        }
        .theme-toggle .icon {
            transition: transform 0.3s ease;
        }
        .theme-toggle:active .icon {
            transform: rotate(360deg);
        }
'''

# 統一的淺色主題樣式
LIGHT_THEME_CSS = '''
        /* ===== 統一淺色主題樣式 ===== */
        [data-theme="light"] body {
            background: linear-gradient(135deg, #f5f7fa 0%, #e8ecf1 50%, #dfe4ea 100%) !important;
            color: #2d3748 !important;
        }
        [data-theme="light"] h1 {
            color: #2b6cb0 !important;
            border-bottom-color: #4fd1c5 !important;
        }
        [data-theme="light"] h2 {
            color: #2c5282 !important;
            border-bottom-color: #90cdf4 !important;
        }
        [data-theme="light"] h3 {
            color: #2b6cb0 !important;
        }
        [data-theme="light"] h4 {
            color: #3182ce !important;
        }
        [data-theme="light"] p,
        [data-theme="light"] li,
        [data-theme="light"] td {
            color: #4a5568 !important;
        }
        [data-theme="light"] th {
            background-color: rgba(79, 209, 197, 0.25) !important;
            color: #2d3748 !important;
        }
        [data-theme="light"] table {
            background-color: rgba(255, 255, 255, 0.9) !important;
            border-color: rgba(79, 209, 197, 0.3) !important;
        }
        [data-theme="light"] tr:nth-child(even) {
            background-color: rgba(79, 209, 197, 0.08) !important;
        }
        [data-theme="light"] tr:hover {
            background-color: rgba(79, 209, 197, 0.15) !important;
        }
        [data-theme="light"] code {
            background-color: rgba(79, 209, 197, 0.2) !important;
            color: #2b6cb0 !important;
        }
        [data-theme="light"] pre {
            background-color: #f7fafc !important;
            border: 1px solid rgba(79, 209, 197, 0.3) !important;
        }
        [data-theme="light"] pre code {
            background-color: transparent !important;
            color: #2d3748 !important;
        }
        [data-theme="light"] blockquote {
            background-color: rgba(79, 209, 197, 0.15) !important;
            border-left-color: #4fd1c5 !important;
            color: #2d3748 !important;
        }
        [data-theme="light"] a {
            color: #3182ce !important;
        }
        [data-theme="light"] a:hover {
            color: #319795 !important;
        }
        [data-theme="light"] strong {
            color: #d69e2e !important;
        }
        [data-theme="light"] hr {
            border-top-color: rgba(79, 209, 197, 0.3) !important;
        }
        /* 首頁專用淺色樣式 */
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
            background: rgba(255, 255, 255, 0.85) !important;
            border: 1px solid rgba(79, 209, 197, 0.4) !important;
        }
        [data-theme="light"] .stat-number {
            color: #319795 !important;
        }
        [data-theme="light"] .category-nav {
            background: rgba(255, 255, 255, 0.7) !important;
        }
        [data-theme="light"] .category-btn {
            background: rgba(255, 255, 255, 0.95) !important;
            color: #2d3748 !important;
            border: 1px solid rgba(79, 209, 197, 0.4) !important;
        }
        [data-theme="light"] .category-btn:hover,
        [data-theme="light"] .category-btn.active {
            background: linear-gradient(135deg, #4fd1c5, #63b3ed) !important;
            color: #ffffff !important;
        }
        [data-theme="light"] .section-title {
            color: #2b6cb0 !important;
            border-bottom-color: rgba(79, 209, 197, 0.4) !important;
        }
        [data-theme="light"] .doc-card {
            background: rgba(255, 255, 255, 0.95) !important;
            border: 1px solid rgba(79, 209, 197, 0.25) !important;
        }
        [data-theme="light"] .doc-card:hover {
            background: rgba(255, 255, 255, 1) !important;
            border-color: #4fd1c5 !important;
            box-shadow: 0 10px 30px rgba(79, 209, 197, 0.25) !important;
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
        [data-theme="light"] .tag {
            background: rgba(79, 209, 197, 0.2) !important;
            color: #319795 !important;
        }
        [data-theme="light"] .tag.beginner {
            background: rgba(72, 187, 120, 0.2) !important;
            color: #2f855a !important;
        }
        [data-theme="light"] .tag.intermediate {
            background: rgba(237, 137, 54, 0.2) !important;
            color: #c05621 !important;
        }
        [data-theme="light"] .tag.advanced {
            background: rgba(245, 101, 101, 0.2) !important;
            color: #c53030 !important;
        }
        [data-theme="light"] .tag.essential {
            background: rgba(159, 122, 234, 0.2) !important;
            color: #6b46c1 !important;
        }
        [data-theme="light"] .learning-path {
            background: rgba(255, 255, 255, 0.85) !important;
            border: 1px solid rgba(79, 209, 197, 0.3) !important;
        }
        [data-theme="light"] .learning-path h2 {
            color: #2b6cb0 !important;
        }
        [data-theme="light"] .path-item {
            background: rgba(247, 250, 252, 0.95) !important;
            border-left-color: #4fd1c5 !important;
        }
        [data-theme="light"] .path-content h4 {
            color: #2b6cb0 !important;
        }
        [data-theme="light"] .path-content p {
            color: #4a5568 !important;
        }
        [data-theme="light"] .footer {
            color: #4a5568 !important;
            border-top: 1px solid rgba(79, 209, 197, 0.3) !important;
        }
'''

# 主題切換 JavaScript
THEME_JS = '''
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
            document.documentElement.setAttribute('data-theme', savedTheme);
            var icon = document.getElementById('theme-icon');
            if (icon) {
                icon.textContent = savedTheme === 'light' ? '🌙' : '☀️';
            }
        })();
    </script>
'''

# 返回首頁按鈕 HTML（圓形，只有圖示）
HOME_BUTTON_HTML = '''
    <!-- 返回首頁按鈕 -->
    <a href="文件索引.html" class="floating-home" title="返回文件中心">🏠</a>
'''

# 主題切換按鈕 HTML
THEME_TOGGLE_HTML = '''
    <!-- 主題切換按鈕 -->
    <button class="theme-toggle" onclick="toggleTheme()" title="切換淺色/深色主題">
        <span class="icon" id="theme-icon">☀️</span>
    </button>
'''


def process_subpage(filepath):
    """處理分頁 HTML 檔案"""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()

        original_content = content

        # 1. 移除舊的 floating-home 樣式
        content = re.sub(
            r'\.floating-home\s*\{[^}]+\}',
            '',
            content
        )
        content = re.sub(
            r'\.floating-home:hover\s*\{[^}]+\}',
            '',
            content
        )
        content = re.sub(
            r'\.floating-home:active\s*\{[^}]+\}',
            '',
            content
        )

        # 2. 移除舊的 theme-toggle 相關樣式
        content = re.sub(
            r'/\*\s*主題切換按鈕\s*\*/.*?(?=/\*|</style>)',
            '',
            content,
            flags=re.DOTALL
        )

        # 3. 移除舊的淺色主題樣式
        content = re.sub(
            r'/\*\s*=+\s*淺色主題樣式\s*=+\s*\*/.*?(?=</style>)',
            '',
            content,
            flags=re.DOTALL
        )

        # 4. 移除重複的空白行
        content = re.sub(r'\n\s*\n\s*\n+', '\n\n', content)

        # 5. 在 </style> 前插入新的統一樣式
        new_styles = HOME_BUTTON_CSS + THEME_TOGGLE_CSS + LIGHT_THEME_CSS
        content = content.replace('</style>', new_styles + '\n    </style>')

        # 6. 移除舊的 floating-home HTML（各種格式）
        content = re.sub(
            r'<a[^>]*class="floating-home"[^>]*>.*?</a>\s*',
            '',
            content,
            flags=re.DOTALL
        )

        # 7. 移除舊的 theme-toggle HTML
        content = re.sub(
            r'<!--\s*主題切換按鈕\s*-->.*?</button>\s*',
            '',
            content,
            flags=re.DOTALL
        )

        # 8. 移除舊的主題切換 JavaScript
        content = re.sub(
            r'<script>\s*//\s*主題切換功能.*?</script>\s*',
            '',
            content,
            flags=re.DOTALL
        )

        # 9. 在 <body> 後加入新的按鈕
        if '<body>' in content:
            # 檢查是否是首頁（不需要返回首頁按鈕）
            is_index = '文件索引' in filepath or 'index' in filepath.lower()

            if is_index:
                buttons = THEME_TOGGLE_HTML
            else:
                buttons = THEME_TOGGLE_HTML + HOME_BUTTON_HTML

            content = content.replace('<body>', '<body>' + buttons, 1)

        # 10. 在 </body> 前加入 JavaScript
        if '</body>' in content:
            content = content.replace('</body>', THEME_JS + '</body>', 1)

        # 11. 移除 body 的 padding-top（舊版頂部按鈕用的）
        content = re.sub(r'padding-top:\s*\d+px;', '', content)

        # 寫回檔案
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)

        return True

    except Exception as e:
        print(f"  [錯誤] {os.path.basename(filepath)}: {e}")
        return False


def main():
    print("=" * 60)
    print("統一所有 HTML 文件的主題樣式")
    print("=" * 60)
    print()
    print("變更內容：")
    print("  - 主題切換按鈕：右上角（太陽/月亮）")
    print("  - 返回首頁按鈕：右下角（房子圖示）")
    print("  - 淺色主題樣式：統一配色")
    print()

    html_files = [f for f in os.listdir(STATIC_DIR) if f.endswith('.html')]

    print(f"找到 {len(html_files)} 個 HTML 檔案")
    print()

    success_count = 0

    for filename in sorted(html_files):
        filepath = os.path.join(STATIC_DIR, filename)
        print(f"  處理: {filename}...", end='')
        if process_subpage(filepath):
            print(" [OK]")
            success_count += 1
        else:
            print(" [FAIL]")

    print()
    print("=" * 60)
    print(f"處理完成！成功: {success_count}/{len(html_files)}")
    print("=" * 60)


if __name__ == '__main__':
    main()
