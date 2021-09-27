"""
MiniAlphaGo环境说明
"""
运行环境：windows10、python3、PyCharm Community Editor 2021.2.2
依赖包环境：tkinter
目录介绍：
Algorithm/valid.pyx：关于落子位置的合法性检查以及棋子翻转等操作，需要进行联合
编译，联合编译方法见Algorithm/README.md
Algorithm/SearchMgr：关于ai一方进行最佳落子位置的查找算法
Config/UiConfig.py：用户界面配置
Game/MiniAlphaGoBoard.py：MiniAlphaGo棋盘维护模块
Game/MiniAlphaGoInit.py：软件初始化
Game/MiniAlphaGoMgr.py；MiniAlphaGo的界面管理
./main.py：主程序入口