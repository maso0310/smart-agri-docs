# -*- coding: utf-8 -*-
"""
WSGI 入口點
用於 Gunicorn、uWSGI 等 WSGI 伺服器

使用方式：
    gunicorn -w 4 -b 0.0.0.0:5000 wsgi:app
    uwsgi --http :5000 --wsgi-file wsgi.py --callable app

注意：
    - app 是 DispatcherMiddleware，掛載在 /smart-agri-docs 路徑
    - 直接執行 python wsgi.py 會使用開發模式（不含路由前綴）
"""

from app import app, real_app

if __name__ == '__main__':
    # 開發模式直接執行 real_app
    real_app.run(host='0.0.0.0', port=5000, debug=True)
