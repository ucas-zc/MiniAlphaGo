"""
用户配置
"""
# !/usr/bin/python3

# 棋盘宽度
BOARD_WIDTH = 8
# 棋盘长度
BOARD_HEIGHT = 8

# 操作手代码
AI_OPERATOR = 0
PLAYER_OPERATOR = 1

# 字体编号
FONT_FIFTY = 50
FONT_THIRTY = 30
FONT_FOURTEEN = 14

# 颜色编号
GREY_COLOR = "#AAAAAA"
WHITE_COLOR = "#FFFFFF"
BLACK_COLOR = "#000000"
GOLD_COLOR = "#B29600"
GREEN_COLOR = "#008800"
YELLOW_COLOR = "#FFFF00"
ALARM_COLOR = "#DC143C"
BROWN_COLOR = "#BDB76B"

# 游戏名称
GAME_NAME = "Mini Alpha Go"

# 坐标配置
# 游戏logo坐标
LOGO_COORDINATE = (250, 210)
LOGO_SHADOW_COORDINATE = (250, 213)
# pc博弈按钮坐标
PC_BATTLE_COORDINATE = (155, 310, 335, 355)
PC_BATTLE_SHADOW_COORDINATE = (155, 300, 335, 350)
PC_BATTLE_TEXT_COORDINATE = (255, 326)
# ai博弈的配置
AI_BATTLE_COORDINATE = (155, 390, 335, 435)
AI_BATTLE_SHADOW_COORDINATE = (155, 380, 335, 430)
AI_BATTLE_TEXT_COORDINATE = (255, 406)
# 重开按钮坐标
REMAKE_COORDINATE = (0, 0, 40, 40)
REMAKE_OVAL_COORDINATE = (5, 5, 35, 35)
REMAKE_OVAL_WIDTH = 4
REMAKE_OVAL_EXTENT = 300
REMAKE_POLYGON_COORDINATE = (23, 28, 36, 29, 30, 39)
# 退出按钮坐标
QUIT_COORDINATE = (460, 0, 500, 40)
QUIT_BACK_SLASH_COORDINATE = (465, 5, 495, 35)
QUIT_SLASH_COORDINATE = (495, 5, 465, 35)
QUIT_SLASH_WIDTH = 4
# 棋盘坐标
BOARD_COORDINATE = (50, 50, 450, 450)
BOARD_LEFT_COORDINATE = 50
BOARD_RIGHT_COORDINATE = 450
BOARD_SINGLE_WIDTH = 50
BOARD_LVERTEX_COORDINATE = 54
BOARD_RVERTEX_COORDINATE = 96
BOARD_NOTE_LVERTEX_COORDINATE = 68
BOARD_NOTE_RVERTEX_COORDINATE = 32
BOARD_ERROR_MSG_COORDINATE = (250, 20)
# 最终结果信息坐标
RESULT_COORDINATE = (250, 500)
RESULT_WHITE_TAG_COORDINATE = (50, 540, 70, 560)
RESULT_WHITE_COUNT_COORDINATE = (80, 550)
RESULT_BLACK_TAG_COORDINATE = (430, 540, 450, 560)
RESULT_BLACK_COUNT_COORDINATE = (410, 550)
RESULT_AI_SINGLE_TIME_COORDINATE = (120, 550)
RESULT_AI_TOTAL_TIME_COORDINATE = (180, 550)
RESULT_PLAYER_SINGLE_TIME_COORDINATE = (300, 550)
RESULT_PLAYER_TOTAL_TIME_COORDINATE = (350, 550)

ROXANNE_TBL = [[(0, 0), (0, 7), (7, 0), (7, 7)],
               [(2, 2), (2, 3), (2, 4), (2, 5), (3, 2), (3, 3), (3, 4), (3, 5),
                (4, 2), (4, 3), (4, 4), (4, 5), (5, 2), (5, 3), (5, 4), (5, 5)],
               [(2, 0), (3, 0), (4, 0), (5, 0), (2, 7), (3, 7), (4, 7), (5, 7),
                (0, 2), (0, 3), (0, 4), (0, 5), (7, 2), (7, 3), (7, 4), (7, 5)],
               [(2, 1), (3, 1), (4, 1), (5, 1), (2, 6), (3, 6), (4, 6), (5, 6),
                (1, 2), (1, 3), (1, 4), (1, 5), (6, 2), (6, 3), (6, 4), (6, 5)],
               [(0, 1), (1, 0), (1, 1), (1, 6), (0, 6), (1, 7),
                (6, 1), (6, 0), (7, 1), (6, 6), (6, 7), (7, 6)]]

priority_table = [[(0, 0), (0, 7), (7, 0), (7, 7)],
                  [(0, 2), (0, 5), (2, 0), (5, 0), (2, 7), (5, 7), (7, 2), (7, 5)],
                  [(2, 2), (2, 5), (5, 2), (5, 5)],
                  [(3, 0), (4, 0), (0, 3), (0, 4), (7, 3), (7, 4), (3, 7), (4, 7)],
                  [(3, 2), (4, 2), (2, 3), (2, 4), (3, 5), (4, 5), (5, 3), (5, 4)],
                  [(3, 3), (4, 4), (3, 4), (4, 3)], # 0
                  [(1, 3), (1, 4), (3, 1), (4, 1), (6, 3), (6, 4), (3, 6), (4, 6)],
                  [(1, 2), (1, 5), (2, 1), (5, 1), (6, 2), (6, 5), (2, 6), (5, 6)],
                  [(0, 1), (0, 6), (7, 1), (7, 6), (1, 0), (6, 0), (1, 7), (6, 7)],
                  [(1, 1), (6, 6), (1, 6), (6, 1)]]

# 通信模块
HOST = '10.20.210.191'
PORT = 90
RECV_SIZE = 128