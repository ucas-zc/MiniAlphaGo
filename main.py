"""
MiniAlphaGo主程序入口
"""
# !/usr/bin/python3
from Game.MiniAlphaGoMgr import *


""" 
主程序入口 
"""
if __name__ == '__main__':
    # 游戏初始化
    MiniAlphaGoMgr.GetInstance().InitGame()

    MainScreen.bind("<Button-1>", MiniAlphaGoMgr.GetInstance().HandleClickEvent)
    MainScreen.focus_set()

    # 设置title
    root.wm_title('Mini Alpha Go')
    root.mainloop()
