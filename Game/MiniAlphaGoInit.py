"""
MiniAlphaGo初始化模块
"""
# !/usr/bin/python3
from tkinter import *

# 引入Tk,画布的加入，底色为黑
root = Tk()
MainScreen = Canvas(root, width=500, height=600, background="#FF1493", highlightthickness=0)
MainScreen.pack()

# 游戏启动状态
first_player = 0
