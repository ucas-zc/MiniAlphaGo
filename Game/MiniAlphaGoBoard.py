"""
游戏棋盘维护
"""
# !/usr/bin/python3
from copy import deepcopy
from datetime import timedelta, datetime
from Config.UiConfig import *
from Game.MiniAlphaGoInit import *
from Algorithm import valid
from Algorithm.SearchMgr import *


class MiniAlphaGoBoard:
    # 构造函数
    def __init__(self):
        # 设置当前玩家
        self.cur_player = first_player
        # 通过标志位
        self.passed = False
        # 结束标志位
        self.over = False
        # 初始化一个空的棋盘(8*8)
        self.board = []
        for index_x in range(BOARD_HEIGHT):
            self.board.append([])
            for index_y in range(BOARD_WIDTH):
                self.board[index_x].append(None)
        self.board[3][3] = AI_OPERATOR
        self.board[3][4] = PLAYER_OPERATOR
        self.board[4][3] = PLAYER_OPERATOR
        self.board[4][4] = AI_OPERATOR
        self.board = tuple(self.board)
        # 保存棋盘信息
        self.OldBoard = self.board
        # 保持上一步的棋盘信息，用于回退操作
        self.LastBoard = self.board
        # 记录玩家与AI的单步时间与总时间
        self.player_single_time = timedelta(seconds=0)
        self.player_total_time = timedelta(seconds=0)
        self.ai_single_time = timedelta(seconds=0)
        self.ai_total_time = timedelta(seconds=0)
        # 上一步终止时间
        self.single_end_time = datetime.utcnow()
        # 蒙特卡洛树更新
        SearchMgr.GetInstance().UpdateMct(self)

    # 返回单例
    @classmethod
    def GetInstance(cls):
        if not hasattr(MiniAlphaGoBoard, '_instance'):
            MiniAlphaGoBoard._instance = MiniAlphaGoBoard()
        return MiniAlphaGoBoard._instance

    # 重开接口
    def ReMake(self):
        # 设置当前玩家
        self.cur_player = first_player
        # 通过标志位
        self.passed = False
        # 结束标志位
        self.over = False
        for i in range(BOARD_HEIGHT):
            for j in range(BOARD_WIDTH):
                self.board[i][j] = None
        self.board[3][3] = AI_OPERATOR
        self.board[3][4] = PLAYER_OPERATOR
        self.board[4][3] = PLAYER_OPERATOR
        self.board[4][4] = AI_OPERATOR
        self.board = tuple(self.board)
        # 保存棋盘信息
        self.OldBoard = self.board
        # 保持上一步的棋盘信息，用于回退操作
        self.LastBoard = self.board
        # 记录玩家与AI的单步时间与总时间
        self.player_single_time = timedelta(seconds=0)
        self.player_total_time = timedelta(seconds=0)
        self.ai_single_time = timedelta(seconds=0)
        self.ai_total_time = timedelta(seconds=0)
        # 上一步终止时间
        self.single_end_time = datetime.utcnow()
        # 蒙特卡洛树更新
        SearchMgr.GetInstance().UpdateMct(self)

    # 绘制当前步骤前的棋盘情况
    def DrawScoreBoard(self):
        # 删除之前的得分标签内容
        MainScreen.delete("score")
        # 声明玩家与电脑计数的临时变量
        player_score = 0
        compute_score = 0
        for i in range(BOARD_HEIGHT):
            for j in range(BOARD_WIDTH):
                if self.board[i][j] == PLAYER_OPERATOR:
                    player_score += 1
                elif self.board[i][j] == AI_OPERATOR:
                    compute_score += 1
        # 绘制黑白棋子的标记
        MainScreen.create_oval(RESULT_WHITE_TAG_COORDINATE,
                               fill=WHITE_COLOR, outline=WHITE_COLOR)
        MainScreen.create_oval(RESULT_BLACK_TAG_COORDINATE,
                               fill=BLACK_COLOR, outline=BLACK_COLOR)
        # 显示白棋数量
        MainScreen.create_text(RESULT_WHITE_COUNT_COORDINATE,
                               anchor="w", tags="score",
                               font=("Consolas", FONT_FOURTEEN),
                               fill=WHITE_COLOR, text=compute_score)
        # 显示时间
        player_single_time = self.player_single_time
        player_total_time = self.player_total_time
        ai_single_time = self.ai_single_time
        ai_total_time = self.ai_total_time
        text_tmp = str(ai_single_time.seconds // 60) \
                   + ':' + str(ai_single_time.seconds % 60) + ' /'
        MainScreen.create_text(RESULT_AI_SINGLE_TIME_COORDINATE,
                               anchor="w", tags="score",
                               font=("Consolas", FONT_FOURTEEN),
                               fill=WHITE_COLOR, text=text_tmp)
        text_tmp = str(ai_total_time.seconds // 60) \
                   + ':' + str(ai_total_time.seconds % 60)
        MainScreen.create_text(RESULT_AI_TOTAL_TIME_COORDINATE,
                               anchor="w", tags="score",
                               font=("Consolas", FONT_FOURTEEN),
                               fill=WHITE_COLOR, text=text_tmp)
        text_tmp = str(player_single_time.seconds // 60) \
                   + ':' + str(player_single_time.seconds % 60) + '/ '
        MainScreen.create_text(RESULT_PLAYER_SINGLE_TIME_COORDINATE,
                               anchor="w", tags="score",
                               font=("Consolas", FONT_FOURTEEN),
                               fill=BLACK_COLOR, text=text_tmp)
        text_tmp = str(player_total_time.seconds // 60) \
                   + ':' + str(player_total_time.seconds % 60)
        MainScreen.create_text(RESULT_PLAYER_TOTAL_TIME_COORDINATE,
                               anchor="w", tags="score",
                               font=("Consolas", FONT_FOURTEEN),
                               fill=BLACK_COLOR, text=text_tmp)
        # 显示黑棋数量
        MainScreen.create_text(RESULT_BLACK_COUNT_COORDINATE,
                               anchor="w", tags="score",
                               font=("Consolas", FONT_FOURTEEN),
                               fill=BLACK_COLOR, text=player_score)

    # 询问当前哪一方取得落子权
    def Who2Play(self):
        return self.cur_player

    # 执行棋盘移动
    def ExecMove(self, board, player, coordinate_x, coordinate_y):
        """
        :param board: 当前棋盘布局情况
        :param player: 当前落子方
        :param coordinate_x: 棋盘横坐标
        :param coordinate_y: 棋盘纵坐标
        :return:
        """
        _tmp = deepcopy(board)
        return valid.move(_tmp, player, coordinate_x, coordinate_y)

    # 移动棋盘
    def MoveBoard(self, player, coordinate_x, coordinate_y):
        """
        :param player: 当前发出移动命令的玩家
        :param coordinate_x: 将移动到的横坐标
        :param coordinate_y: 将移动到的纵坐标
        :return:
        """
        # 无落子权不移动棋盘
        if player != self.Who2Play():
            return False
        # 开始移动棋盘
        self.board = self.ExecMove(self.board, self.cur_player,
                                   coordinate_x, coordinate_y)
        self.OldBoard = deepcopy(self.board)
        # 切换玩家
        self.cur_player = 1 - player
        SearchMgr.GetInstance().UpdateMct(self)
        self.UpdateBoard(self.cur_player)

    # 更新棋盘
    def UpdateBoard(self, player):
        """
        白棋代表AI，黑棋代表玩家
        :param player: 玩家编号，0代表ai，1代表玩家
        :return:
        """
        # 删除之前的标签
        MainScreen.delete("highlight")
        MainScreen.delete("tile")
        # 布置棋盘
        for x in range(BOARD_HEIGHT):
            for y in range(BOARD_WIDTH):
                if self.OldBoard[x][y] == AI_OPERATOR:
                    MainScreen.create_oval(
                        BOARD_LVERTEX_COORDINATE + BOARD_SINGLE_WIDTH * x,
                        BOARD_LVERTEX_COORDINATE + BOARD_SINGLE_WIDTH * y,
                        BOARD_RVERTEX_COORDINATE + BOARD_SINGLE_WIDTH * x,
                        BOARD_RVERTEX_COORDINATE + BOARD_SINGLE_WIDTH * y,
                        tags="tile {0}-{1}".format(x, y), fill=WHITE_COLOR,
                        outline=WHITE_COLOR)
                elif self.OldBoard[x][y] == PLAYER_OPERATOR:
                    MainScreen.create_oval(
                        BOARD_LVERTEX_COORDINATE + BOARD_SINGLE_WIDTH * x,
                        BOARD_LVERTEX_COORDINATE + BOARD_SINGLE_WIDTH * y,
                        BOARD_RVERTEX_COORDINATE + BOARD_SINGLE_WIDTH * x,
                        BOARD_RVERTEX_COORDINATE + BOARD_SINGLE_WIDTH * y,
                        tags="tile {0}-{1}".format(x, y), fill=BLACK_COLOR,
                        outline=BLACK_COLOR)
        MainScreen.update()
        # 给出提示信息（玩家哪些位置可以落子）
        for x in range(BOARD_HEIGHT):
            for y in range(BOARD_WIDTH):
                if valid.valid(self.board, player, x, y):
                    MainScreen.create_oval(
                        BOARD_NOTE_LVERTEX_COORDINATE + BOARD_SINGLE_WIDTH * x,
                        BOARD_NOTE_LVERTEX_COORDINATE + BOARD_SINGLE_WIDTH * y,
                        BOARD_NOTE_RVERTEX_COORDINATE + BOARD_SINGLE_WIDTH * (x + 1),
                        BOARD_NOTE_RVERTEX_COORDINATE + BOARD_SINGLE_WIDTH * (y + 1),
                        tags="highlight", fill=GREEN_COLOR, outline=GREEN_COLOR)
        # 如果非PC玩家均无处落子，则继续游戏
        if not (len(valid.get_valid_moves(self.board, player)) == 0 and
                len(valid.get_valid_moves(self.board, 1 - player)) == 0):
            MainScreen.update()
            # 给出当前棋盘情况
            self.DrawScoreBoard()
            # 轮到AI下棋
            if self.cur_player == AI_OPERATOR:
                print("ai")
                # 由用户切换到ai，清算用户所花时间并完成界面更新
                self.player_single_time = datetime.utcnow() \
                                          - self.single_end_time
                self.player_total_time += self.player_single_time
                self.DrawScoreBoard()
                # 开始ai下棋
                start_time = datetime.utcnow()
                # 蒙特卡洛树搜索
                next_move = SearchMgr.GetInstance().MCTS()
                print(next_move)
                if next_move is not None:
                    self.MoveBoard(AI_OPERATOR, next_move[0], next_move[1])
                else:
                    self.cur_player = PLAYER_OPERATOR
                    self.UpdateBoard(PLAYER_OPERATOR)
                # 计算ai单步时间，并更新界面
                self.ai_single_time = datetime.utcnow() - start_time
                self.ai_total_time += self.ai_single_time
                self.single_end_time = datetime.utcnow()
                self.DrawScoreBoard()
        # 如果PC玩家均无处落子，游戏结束
        else:
            black_cnt = 0
            white_cnt = 0
            for _board in self.board:
                for tmp in _board:
                    if tmp == PLAYER_OPERATOR:
                        black_cnt += 1
                    if tmp == AI_OPERATOR:
                        white_cnt += 1
            # 玩家输
            if white_cnt > black_cnt:
                MainScreen.create_text(RESULT_COORDINATE, anchor="center",
                                       font=("Consolas", FONT_THIRTY),
                                       fill=YELLOW_COLOR, text="You Lose!")
            # 平局
            elif white_cnt == black_cnt:
                MainScreen.create_text(RESULT_COORDINATE, anchor="center",
                                       font=("Consolas", FONT_THIRTY),
                                       fill=YELLOW_COLOR, text="Tie!")
            # 玩家赢
            else:
                self.DrawScoreBoard()
                MainScreen.create_text(RESULT_COORDINATE, anchor="center",
                                       font=("Consolas", FONT_THIRTY),
                                       fill=YELLOW_COLOR, text="You Win!")
            # 棋盘满了
            if black_cnt + white_cnt == 64 \
                    or (len(valid.get_valid_moves(self.board, 0)) == 0
                        and len(valid.get_valid_moves(self.board, 1))):
                self.over = True
