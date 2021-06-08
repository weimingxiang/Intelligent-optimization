# coding=utf-8
__author__ = "x1nglZ"

# 导入包时调用，在此进行包路径管理。
import os
import sys

# sys.path.append('/path/to/test') # 这里可以用os.path类方法自动获得绝对路径
sys.path.append(os.path.dirname(__file__))
print(os.path.dirname(os.path.dirname(__file__)))
