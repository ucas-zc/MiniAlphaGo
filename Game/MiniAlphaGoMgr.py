"""
MiniAlphaGo管理器模块
"""
# !/usr/bin/python3
from Algorithm import valid
from Communication.SocketMgr import *


class MiniAlphaGoMgr:
    # 构造函数
    def __init__(self):
        pass

    # 返回单例
    @classmethod
    def GetInstance(cls):
        if not hasattr(MiniAlphaGoMgr, '_instance'):
            MiniAlphaGoMgr._instance = MiniAlphaGoMgr()
        return MiniAlphaGoMgr._instance

    # 游戏初始化
    def InitGame(self):
        # 全局变量运行状态
        global running
        running = False
        # 删除所有已经绘制的图像
        MainScreen.delete(ALL)
        # 贴游戏名称水印
        MainScreen.create_text(LOGO_COORDINATE,
                               anchor="center", text=GAME_NAME,
                               font=("Consolas", FONT_FIFTY),
                               fill=GREY_COLOR)
        MainScreen.create_text(LOGO_SHADOW_COORDINATE,
                               anchor="center", text=GAME_NAME,
                               font=("Consolas", FONT_FIFTY),
                               fill=WHITE_COLOR)
        # 构造pc作战的按钮
        MainScreen.create_rectangle(PC_BATTLE_COORDINATE,
                                    fill=GREY_COLOR, outline=GREY_COLOR)
        MainScreen.create_rectangle(PC_BATTLE_SHADOW_COORDINATE,
                                    fill=WHITE_COLOR, outline=WHITE_COLOR)
        MainScreen.create_text(PC_BATTLE_TEXT_COORDINATE,
                               anchor="center", text="PC battle",
                               font=("Consolas", FONT_FOURTEEN),
                               fill=GOLD_COLOR)

        # 构造ai博弈的按钮
        MainScreen.create_rectangle(AI_BATTLE_COORDINATE,
                                    fill=GREY_COLOR, outline=GREY_COLOR)
        MainScreen.create_rectangle(AI_BATTLE_SHADOW_COORDINATE,
                                    fill=WHITE_COLOR, outline=WHITE_COLOR)
        MainScreen.create_text(AI_BATTLE_TEXT_COORDINATE,
                               anchor="center", text="AI battle",
                               font=("Consolas", FONT_FOURTEEN),
                               fill=GOLD_COLOR)
        MainScreen.update()

    # 处理点击事件
    def HandleClickEvent(self, event):
        # 获取横纵坐标
        CoordinateX = event.x
        CoordinateY = event.y
        # 运行状态下的处理
        if running:
            # 点击退出按钮，退出游戏
            if CoordinateX >= QUIT_COORDINATE[0] \
                    and CoordinateY <= QUIT_COORDINATE[3]:
                root.destroy()
            # 点击重开按钮
            elif CoordinateX <= REMAKE_COORDINATE[2] \
                    and CoordinateY <= REMAKE_COORDINATE[3]:
                MiniAlphaGoBoard.GetInstance().ReMake()
                self.InitGame()
            # 玩家开始下棋
            else:
                MainScreen.delete("error")
                MainScreen.delete("red_alarm")
                # 获取玩家点击的棋盘坐标
                coordinate_x = int((event.x - BOARD_SINGLE_WIDTH)
                                   / BOARD_SINGLE_WIDTH)
                coordinate_y = int((event.y - BOARD_SINGLE_WIDTH)
                                   / BOARD_SINGLE_WIDTH)
                # 坐标异常判断
                if 0 <= coordinate_x < BOARD_HEIGHT \
                        and 0 <= coordinate_y < BOARD_WIDTH:
                    # 是否拥有落子权
                    if PLAYER_OPERATOR == MiniAlphaGoBoard.GetInstance().Who2Play():
                        # 位置合法性检查
                        if valid.valid(MiniAlphaGoBoard.GetInstance().board,
                                       PLAYER_OPERATOR, coordinate_x, coordinate_y):
                            MiniAlphaGoBoard.GetInstance(). \
                                MoveBoard(PLAYER_OPERATOR, coordinate_x, coordinate_y)
                        else:
                            # 标记不合法落子位置
                            MainScreen.create_oval(BOARD_NOTE_LVERTEX_COORDINATE
                                                   + BOARD_SINGLE_WIDTH * coordinate_x,
                                                   BOARD_NOTE_LVERTEX_COORDINATE
                                                   + BOARD_SINGLE_WIDTH * coordinate_y,
                                                   BOARD_NOTE_RVERTEX_COORDINATE
                                                   + BOARD_SINGLE_WIDTH * (coordinate_x + 1),
                                                   BOARD_NOTE_RVERTEX_COORDINATE
                                                   + BOARD_SINGLE_WIDTH * (coordinate_y + 1),
                                                   tag="red_alarm", fill=ALARM_COLOR,
                                                   outline=ALARM_COLOR)
                            # 给出提示信息
                            MainScreen.create_text(BOARD_ERROR_MSG_COORDINATE, anchor="c",
                                                   text="Please drop in the allowed position!",
                                                   font=("Consolas", FONT_FOURTEEN),
                                                   tag="error", fill=BLACK_COLOR)
                    # 尚未获得落子权
                    else:
                        # 给出提示信息
                        MainScreen.create_text(BOARD_ERROR_MSG_COORDINATE, anchor="c",
                                               text="Please drop in the allowed time!",
                                               font=("Consolas", FONT_FOURTEEN),
                                               tag="error", fill=BLACK_COLOR)
            # 挂
            if CoordinateY > 580:
                dog = SearchMgr.GetInstance().MCTS()
                print(dog)
        # 非运行状态下的处理
        else:
            # 根据坐标进行判断选择什么模式
            if PC_BATTLE_SHADOW_COORDINATE[1] <= CoordinateY \
                    <= PC_BATTLE_SHADOW_COORDINATE[3]:
                # pc作战首局玩家获取优先权，随后轮流切换优先权
                if PC_BATTLE_SHADOW_COORDINATE[0] <= CoordinateX \
                        <= PC_BATTLE_SHADOW_COORDINATE[2]:
                    self.PlayGame(first_player)
            if AI_BATTLE_SHADOW_COORDINATE[1] <= CoordinateY \
                    <= AI_BATTLE_SHADOW_COORDINATE[3]:
                # pc作战首局玩家获取优先权，随后轮流切换优先权
                if AI_BATTLE_SHADOW_COORDINATE[0] <= CoordinateX \
                        <= AI_BATTLE_SHADOW_COORDINATE[2]:
                    # 初始化
                    SocketMgr.GetInstance().Init()
                    # 博弈处理
                    SocketMgr.GetInstance().Process()
                    # 关闭套接字
                    SocketMgr.GetInstance().Close()

    # 构建按钮
    def CreateButton(self):
        # 构建重开按钮
        MainScreen.create_rectangle(REMAKE_COORDINATE, fill=GREEN_COLOR,
                                    outline=GREEN_COLOR)
        MainScreen.create_arc(REMAKE_OVAL_COORDINATE, fill=WHITE_COLOR,
                              width=REMAKE_OVAL_WIDTH, style="arc",
                              outline=WHITE_COLOR, extent=REMAKE_OVAL_EXTENT)
        MainScreen.create_polygon(REMAKE_POLYGON_COORDINATE, fill=WHITE_COLOR,
                                  outline=WHITE_COLOR)
        # 退出按钮
        MainScreen.create_rectangle(QUIT_COORDINATE, fill=GREEN_COLOR,
                                    outline=GREEN_COLOR)
        MainScreen.create_line(QUIT_BACK_SLASH_COORDINATE,
                               fill=WHITE_COLOR, width=QUIT_SLASH_WIDTH)
        MainScreen.create_line(QUIT_SLASH_COORDINATE, fill=WHITE_COLOR,
                               width=QUIT_SLASH_WIDTH)

    # 绘制棋盘
    def DrawBoard(self):
        MainScreen.create_rectangle(BOARD_COORDINATE, fill=BROWN_COLOR,
                                    outline=BLACK_COLOR)
        # 绘制8*8方格
        for index in range(BOARD_HEIGHT - 1):
            line_shift = BOARD_LEFT_COORDINATE \
                         + BOARD_LEFT_COORDINATE * (index + 1)
            MainScreen.create_line(BOARD_LEFT_COORDINATE, line_shift,
                                   BOARD_RIGHT_COORDINATE, line_shift,
                                   fill=BLACK_COLOR)
            MainScreen.create_line(line_shift, BOARD_LEFT_COORDINATE,
                                   line_shift, BOARD_RIGHT_COORDINATE,
                                   fill=BLACK_COLOR)
        MainScreen.update()

    # 操作游戏
    def PlayGame(self, player):
        # 更新运行状态
        global running
        running = True
        # 清屏
        MainScreen.delete(ALL)
        # 构建重开按钮与退出按钮
        self.CreateButton()
        # 绘制棋盘
        self.DrawBoard()
        # 棋盘维护
        MiniAlphaGoBoard.GetInstance().UpdateBoard(player)
