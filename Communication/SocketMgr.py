"""
通信处理模块
"""
import socket
from time import sleep
from Algorithm import valid
from Game.MiniAlphaGoBoard import *


class SocketMgr:
    # 初始化函数
    def __init__(self):
        self.cfd = None
        self.caddr = None

    # 返回单例对象
    @classmethod
    def GetInstance(cls):
        if not hasattr(SocketMgr, '_instance'):
            SocketMgr._instance = SocketMgr()
        return SocketMgr._instance

    # 等待连接
    def Init(self):
        print("Game Init...")
        # 套接字的建立
        sk_obj = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sk_obj.bind((HOST, PORT))
        sk_obj.listen(1)
        # 给出提示信息
        print("Wait For Connection...")
        # 给出提示信息
        self.cfd, self.caddr = sk_obj.accept()
        print("Action...")
        # 棋盘初始化
        MiniAlphaGoBoard.GetInstance().cur_player = PLAYER_OPERATOR

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
        MainScreen.create_rectangle(BOARD_COORDINATE, fill=YELLOW_COLOR,
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

    def Process(self):
        MainScreen.delete(ALL)
        # 构建重开按钮与退出按钮
        self.CreateButton()
        # 绘制棋盘
        self.DrawBoard()
        # 棋盘绘制
        MiniAlphaGoBoard.GetInstance().UpdateBoard(PLAYER_OPERATOR)
        while True:
            sleep(0.5)
            # 接受对手的消息
            data = self.cfd.recv(RECV_SIZE)
            print(data)
            sleep(1)
            if data is not None:
                # 合法性分析
                if valid.valid(MiniAlphaGoBoard.GetInstance().board,
                               PLAYER_OPERATOR, data):
                    MiniAlphaGoBoard.GetInstance().MoveBoard(PLAYER_OPERATOR, data)
            # mct搜索
            sett = SearchMgr.GetInstance().MCTS()
            if sett is not None:
                # 发送
                MiniAlphaGoBoard.GetInstance().MoveBoard(AI_OPERATOR, sett)
            else:
                # 发送(-1, -1)
                sett = (-1, -1)
                MiniAlphaGoBoard.GetInstance().cur_player = PLAYER_OPERATOR
                MiniAlphaGoBoard.GetInstance().UpdateBoard(PLAYER_OPERATOR)
            self.cfd.sendall(sett)

    # 关闭套接字
    def Close(self):
        self.cfd.close()
