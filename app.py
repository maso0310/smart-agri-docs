# -*- coding: utf-8 -*-
"""
智慧農業課程技術文件中心 - Flask 應用
=====================================
提供課程技術文件的 Web 服務

使用方式：
    開發模式: python app.py
    生產模式: gunicorn -w 4 -b 0.0.0.0:5000 wsgi:app

路由前綴：/smart-agri-docs
"""

import os
from flask import Flask, send_from_directory, redirect, url_for, abort
from werkzeug.middleware.dispatcher import DispatcherMiddleware

# 初始化 Flask 應用（實際處理請求的應用）
real_app = Flask(__name__, static_folder='static', static_url_path='')

# 設定
real_app.config['SECRET_KEY'] = os.environ.get('SECRET_KEY', 'smart-agri-docs-2024')
real_app.config['DEBUG'] = os.environ.get('FLASK_DEBUG', 'False').lower() == 'true'

# 路由前綴
URL_PREFIX = "/smart-agri-docs"


@real_app.route('/')
def index():
    """首頁 - 重導向到文件索引"""
    return send_from_directory('static', '文件索引.html')


@real_app.route('/<path:filename>')
def serve_file(filename):
    """提供靜態 HTML 檔案"""
    # 確保檔案存在
    filepath = os.path.join(real_app.static_folder, filename)
    if os.path.exists(filepath):
        return send_from_directory('static', filename)

    # 嘗試加上 .html 副檔名
    if not filename.endswith('.html'):
        html_path = os.path.join(real_app.static_folder, filename + '.html')
        if os.path.exists(html_path):
            return send_from_directory('static', filename + '.html')

    abort(404)


@real_app.errorhandler(404)
def page_not_found(e):
    """404 錯誤頁面"""
    return '''
    <!DOCTYPE html>
    <html lang="zh-TW">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>404 - 找不到頁面</title>
        <style>
            body {
                font-family: "Microsoft JhengHei", sans-serif;
                background: linear-gradient(135deg, #1a1a2e 0%, #16213e 100%);
                color: #e8e8e8;
                min-height: 100vh;
                display: flex;
                justify-content: center;
                align-items: center;
                margin: 0;
            }
            .container {
                text-align: center;
                padding: 40px;
            }
            h1 {
                font-size: 6em;
                color: #4fd1c5;
                margin: 0;
            }
            p {
                font-size: 1.5em;
                color: #a0aec0;
            }
            a {
                display: inline-block;
                margin-top: 20px;
                padding: 15px 30px;
                background: linear-gradient(135deg, #4fd1c5, #63b3ed);
                color: #1a1a2e;
                text-decoration: none;
                border-radius: 25px;
                font-weight: bold;
                transition: transform 0.3s;
            }
            a:hover {
                transform: translateY(-3px);
            }
        </style>
    </head>
    <body>
        <div class="container">
            <h1>404</h1>
            <p>找不到您要的頁面</p>
            <a href="/smart-agri-docs/">返回文件中心</a>
        </div>
    </body>
    </html>
    ''', 404


@real_app.errorhandler(500)
def internal_error(e):
    """500 錯誤頁面"""
    return '''
    <!DOCTYPE html>
    <html lang="zh-TW">
    <head>
        <meta charset="UTF-8">
        <title>500 - 伺服器錯誤</title>
        <style>
            body {
                font-family: "Microsoft JhengHei", sans-serif;
                background: linear-gradient(135deg, #1a1a2e 0%, #16213e 100%);
                color: #e8e8e8;
                min-height: 100vh;
                display: flex;
                justify-content: center;
                align-items: center;
                margin: 0;
            }
            .container { text-align: center; }
            h1 { font-size: 4em; color: #fc8181; }
            a {
                display: inline-block;
                margin-top: 20px;
                padding: 15px 30px;
                background: linear-gradient(135deg, #4fd1c5, #63b3ed);
                color: #1a1a2e;
                text-decoration: none;
                border-radius: 25px;
            }
        </style>
    </head>
    <body>
        <div class="container">
            <h1>500</h1>
            <p>伺服器發生錯誤，請稍後再試</p>
            <a href="/smart-agri-docs/">返回首頁</a>
        </div>
    </body>
    </html>
    ''', 500


# 使用 DispatcherMiddleware 將應用掛載到 /smart-agri-docs 路徑
# 根路徑返回 404（或可導向到 /smart-agri-docs）
def not_found_app(environ, start_response):
    """根路徑的處理：返回提示訊息"""
    response_body = b'Smart Agri Docs is available at /smart-agri-docs/'
    status = '302 Found'
    response_headers = [
        ('Content-Type', 'text/plain; charset=utf-8'),
        ('Location', '/smart-agri-docs/')
    ]
    start_response(status, response_headers)
    return [response_body]


# 建立 DispatcherMiddleware
app = DispatcherMiddleware(not_found_app, {
    URL_PREFIX: real_app
})


if __name__ == '__main__':
    # 開發模式 - 直接執行 real_app
    port = int(os.environ.get('PORT', 5000))
    print(f"=" * 50)
    print(f"智慧農業課程技術文件中心")
    print(f"開發模式啟動中...")
    print(f"訪問位址: http://localhost:{port}")
    print(f"=" * 50)
    real_app.run(host='0.0.0.0', port=port, debug=True)
