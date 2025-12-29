# -*- coding: utf-8 -*-
"""
應用程式設定檔
"""

import os


class Config:
    """基礎設定"""
    SECRET_KEY = os.environ.get('SECRET_KEY', 'smart-agri-docs-secret-key')
    DEBUG = False
    TESTING = False


class DevelopmentConfig(Config):
    """開發環境設定"""
    DEBUG = True


class ProductionConfig(Config):
    """生產環境設定"""
    DEBUG = False
    # 建議在生產環境中設定環境變數 SECRET_KEY


class TestingConfig(Config):
    """測試環境設定"""
    TESTING = True


# 設定對照
config = {
    'development': DevelopmentConfig,
    'production': ProductionConfig,
    'testing': TestingConfig,
    'default': DevelopmentConfig
}
