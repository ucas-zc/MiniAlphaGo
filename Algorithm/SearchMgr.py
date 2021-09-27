"""
搜索管理器
"""
# !/usr/bin/python3
from datetime import *
from copy import deepcopy
from math import fabs, sqrt, log
from multiprocessing.pool import ThreadPool
from random import choice
from Algorithm import valid
from Config.UiConfig import *


# 蒙特卡洛树结点定义
class MctNode:
    def __init__(self, depth, player, state, parent=None, pre=None):
        # 结点深度
        self.depth = depth
        # 当前结点代表的玩家
        self.player = player
        # 当前玩家的棋局
        self.state = state
        # 父结点
        self.parent = parent
        # 孩子结点
        self.child = []
        # 下一步结点
        self.pre = pre
        # 扩展次数
        self.N = 0
        # 收益值
        self.Q = 0
        # 可选择落子位置
        self.remain_valid_pos = valid.get_valid_moves(state, player)

    # 添加子结点
    def AddChild(self, chose_move):
        # 剔除一个可落子位置
        self.remain_valid_pos.remove(chose_move)
        # 构建子结点的状态
        _tmp = deepcopy(self.state)
        _state = valid.move(_tmp, self.player, chose_move[0], chose_move[1])
        # 构建子结点
        child = MctNode(self.depth + 1, 1 - self.player, _state, self, chose_move)
        # 将子结点添加到子结点元组中
        self.child.append(child)

    # 判断当前结点是否完全扩展
    def IsFullExpand(self):
        return len(self.remain_valid_pos) == 0

    # 是否为终端结点
    def IsTerminalNode(self):
        return len(self.remain_valid_pos) == 0 and len(self.child) == 0


# 搜索算法管理器
class SearchMgr:
    # 构造函数
    def __init__(self):
        # 获取棋盘操作的指针
        self.board = None
        # 蒙特卡洛树根结点
        self.MctRoot = None
        # 单步限制时间10s
        self.time_limit = 10
        # 最大步数64
        self.step_limit = 64
        # 单步起始时间
        self.start_time = None
        # 最大深度
        self.max_depth = 0
        # 参数
        self.C = 1.414
        # 已走的步数
        self.moves = 0
        self.pri_tbl = deepcopy(ROXANNE_TBL)

    # 返回单例
    @classmethod
    def GetInstance(cls):
        if not hasattr(SearchMgr, '_instance'):
            SearchMgr._instance = SearchMgr()
        return SearchMgr._instance

    # 蒙特卡洛树更新
    def UpdateMct(self, board):
        # 获取棋盘操作的指针
        self.board = board
        # 更新根结点
        self.MctRoot = MctNode(0, board.cur_player, board.board)
        # 最大深度为0
        self.max_depth = 0
        # 更新已走的步数
        self.moves = 0
        for _row in board.board:
            for _col in _row:
                if _col is not None:
                    self.moves += 1

    # 选择最好的孩子结点
    def GetBestChild(self, node, c):
        """
        :param node: 当前结点
        :param c: 函数参数C
        :return:
        """
        child_v = self.UCB(node, c)
        return node.child[child_v.index(max(child_v))]

    # 计算UCB
    def UCB(self, node, c):
        return [1 - child.Q / child.N + c *
                sqrt(log(node.N) / child.N) for child in node.child]

    # 蒙特卡洛树过程之选择
    def SelectV(self, node):
        while not node.IsTerminalNode():
            if node.IsFullExpand():
                node = self.GetBestChild(node, self.C)
            else:
                return self.Expand(node)
        return node

    # 扩展
    def Expand(self, node):
        chose_move = choice(node.remain_valid_pos)
        node.AddChild(chose_move)
        if node.child[-1].depth >= self.max_depth:
            self.max_depth = node.child[-1].depth
        return node.child[-1]

    # 模拟
    def Simulate(self, node):
        Count = 0
        while datetime.utcnow() - self.start_time < self.time_limit:
            v = self.SelectV(node)
            reward = self.SimulatePolicy(v)
            self.BackPropagate(v, reward)
            Count += 1
        return Count

    # 模拟策略
    def SimulatePolicy(self, node):
        # 模拟的步数
        _moves = 0
        _state = deepcopy(node.state)
        _player = node.player
        while _moves + self.moves <= self.step_limit:
            valid_set = valid.get_priority_valid_moves(_state, self.pri_tbl, _player) \
                if _moves + self.moves < 56 else valid.get_valid_moves(_state, _player)
            _player = 1 - _player
            _moves += 1
            if len(valid_set) == 0:
                continue
            (Cx, Cy) = choice(valid_set)
            _state = valid.move(_state, _player, Cx, Cy)
        return self.DumbScore(_state, self.board.cur_player) > 0

    # 计算得分情况
    def DumbScore(self, state, player):
        score = 0
        for _col in state:
            for _row in _col:
                if _row == player:
                    score += 1
                else:
                    score -= 1
        return score

    # 反向传播
    def BackPropagate(self, v, r):
        while v is not None:
            v.N += 1
            v.Q += r if v.player == self.board.cur_player else 1 - r
            v = v.parent

    # 蒙特卡洛树建立过程
    def MctProcess(self, root):
        Count = 0
        # 开始蒙特拉洛树建立
        self.time_limit = timedelta(seconds=62 - fabs(34 - self.moves) * 2)
        while datetime.utcnow() - self.start_time <= self.time_limit:
            # 选择
            vNode = self.SelectV(root)
            # 模拟
            reward = self.SimulatePolicy(vNode)
            # 反向传播
            self.BackPropagate(vNode, reward)
            Count += 1
            # 结点全部扩展，循环结束
            if root.IsFullExpand():
                break
        if len(root.child) == 0:
            return Count
        pool = ThreadPool(len(root.child))
        counts = pool.map(self.Simulate, root.child)
        pool.close()
        pool.join()
        Count += sum(counts)
        return Count

    # 蒙特卡洛树搜索
    def MCTS(self):
        """
        :return: 下一步的棋盘坐标
        """
        self.max_depth = 0
        self.start_time = datetime.utcnow()
        # 蒙特卡洛树建立过程
        self.MctProcess(self.MctRoot)

        return None if len(self.MctRoot.child) == 0 else \
            self.GetBestChild(self.MctRoot, 0).pre
