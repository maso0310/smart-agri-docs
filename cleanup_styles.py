# -*- coding: utf-8 -*-
"""
清理 HTML 檔案中重複的 CSS 樣式
"""

import os
import re

STATIC_DIR = os.path.join(os.path.dirname(__file__), 'static')

def cleanup_html(filepath):
    """清理 HTML 檔案中的重複樣式"""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()

        # 找到 <style> 和 </style> 之間的內容
        style_match = re.search(r'(<style[^>]*>)(.*?)(</style>)', content, re.DOTALL)
        if not style_match:
            return True

        style_start = style_match.group(1)
        style_content = style_match.group(2)
        style_end = style_match.group(3)

        # 分割 CSS 規則
        # 移除所有重複的 .theme-toggle 和 .floating-home 樣式
        # 只保留第一組完整的樣式

        # 用於追蹤已處理的選擇器
        seen_selectors = set()
        cleaned_rules = []

        # 使用正則表達式分割 CSS 規則
        # 匹配選擇器 { 內容 } 的模式
        css_rules = re.findall(r'([^{}]+)\{([^{}]+)\}', style_content)

        for selector, properties in css_rules:
            selector = selector.strip()
            # 跳過已經看過的選擇器
            if selector in seen_selectors:
                continue
            seen_selectors.add(selector)
            cleaned_rules.append(f"        {selector} {{\n            {properties.strip()}\n        }}")

        # 重建 style 內容
        new_style_content = '\n'.join(cleaned_rules)

        # 替換原有的 style 內容
        new_content = content[:style_match.start()] + style_start + '\n' + new_style_content + '\n    ' + style_end + content[style_match.end():]

        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(new_content)

        return True

    except Exception as e:
        print(f"  [錯誤] {os.path.basename(filepath)}: {e}")
        return False


def main():
    print("清理重複的 CSS 樣式...")

    html_files = [f for f in os.listdir(STATIC_DIR) if f.endswith('.html')]

    for filename in sorted(html_files):
        filepath = os.path.join(STATIC_DIR, filename)
        cleanup_html(filepath)
        print(f"  [OK] {filename}")

    print("清理完成！")


if __name__ == '__main__':
    main()
