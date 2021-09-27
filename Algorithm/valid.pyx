from random import *


def valid(tuple array, int player, int x, int y):
    '''
    落子位置合法性检查.
    '''
    cdef int color = player
    if array[x][y] is not None:
        return False
    else:
        # 右方向合法性检查，若隔壁棋子为异色，则一直往右查找
        # 找到一个与自己颜色相同的棋子，则说明该位置为一合法落子位置
        if x < 6 and array[x+1][y] == 1 - color:
            for i in range(x+2, 8):
                if array[i][y] == color:
                    return True
                if array[i][y] is None:
                    break
        # 左方向合法性检查，规则同上
        if x > 1 and array[x-1][y] == 1 - color:
            for i in range(x - 2, -1, -1):
                if array[i][y] == color:
                    return True
                if array[i][y] is None:
                    break
        # 上方向合法性检查，规则同上
        if y < 6 and array[x][y+1] == 1 - color:
            for i in range(y+2, 8):
                if array[x][i] == color:
                    return True
                if array[x][i] is None:
                    break
        # 下方向合法性检查，规则同上
        if y > 1 and array[x][y-1] == 1 - color:
            for i in range(y - 2, -1, -1):
                if array[x][i] == color:
                    return True
                if array[x][i] is None:
                    break
        # 右上方向合法性检查，规则同上
        if x < 6 and y < 6 and array[x+1][y+1] == 1 - color:
            for i in range(2, min(8-x, 8-y)):
                if array[x+i][y+i] == color:
                    return True
                if array[x+i][y+i] is None:
                    break
        # 左上方向合法性检查，规则同上
        if x > 1 and y < 6 and array[x-1][y+1] == 1 - color:
            for i in range(2, min(x+1, 8-y)):
                if array[x-i][y+i] == color:
                    return True
                if array[x-i][y+i] is None:
                    break
        # 右下方向合法性检查，规则同上
        if x < 6 and y > 1 and array[x+1][y-1] == 1 - color:
            for i in range(2, min(8-x, y+1)):
                if array[x+i][y-i] == color:
                    return True
                if array[x+i][y-i] is None:
                    break
        # 左下方向合法性检查，规则同上
        if x > 1 and y > 1 and array[x-1][y-1] == 1 - color:
            for i in range(2, min(x+1, y+1)):
                if array[x-i][y-i] == color:
                    return True
                if array[x-i][y-i] is None:
                    break

        return False

def get_valid_moves(tuple array, int player=1):
    """
    获取所有合法性位置.
    :param array: 8x8 当前棋盘局势
    :param player: 默认为计算机
    :return:
    """
    cdef list valid_moves = []
    for x in range(8):
        for y in range(8):
            if valid(array, player, x, y):
                valid_moves.append((x, y))
    return valid_moves

def get_priority_valid_moves(tuple array, list priority_table, int player=1):
    """
    get the valid moves of the same priority.
    :param array: 8x8 matrix
    :param priority_table: 优先表
    :param player: 默认为计算机
    :return: valid list
    """
    cdef list valid_moves = []
    for priority in priority_table:
        for (x, y) in priority:
            if valid(array, player, x, y):
                valid_moves.append((x, y))
        if len(valid_moves) > 0:
            break
    return valid_moves

def move(tuple array, int player, int x, int y):
    """
    FUNCTION: 棋子翻转
    Assumes the exec_move is valid
    :param array: 8x8 棋盘
    :param player: 当前落子方
    :param x: 落子位置的横坐标
    :param y: 落子位置的纵坐标
    :return: 8x8 翻转后的棋盘
    """

    cdef int color = player
    array[x][y] = color

    # 沿着右边的方向进行翻转棋子，若隔壁棋子与当前棋子颜色不变，继续向前寻找
    if x < 6 and array[x + 1][y] == 1 - color:
        for i in range(x + 2, 8):
            # 在右方向找到第二个与当前棋子颜色相同的棋子，就翻转两个棋子之前的异色棋子
            # 若查找到的棋子颜色与当前棋子颜色不同（非空），则继续往右查找
            if array[i][y] == color:
                for j in range(x+1, i):
                    array[j][y] = color
                break
            # 遇到空棋子，直接返回，不找了
            if array[i][y] is None:
                break
    # 左方向查找，查找规则同上
    if x > 1 and array[x - 1][y] == 1 - color:
        for i in range(x - 1, -1, -1):
            if array[i][y] == color:
                for j in range(x - 1, i, -1):
                    array[j][y] = color
                break
            if array[i][y] is None:
                break
    # 上方向查找，查找规则同上
    if y < 6 and array[x][y + 1] == 1 - color:
        for i in range(y + 1, 8):
            if array[x][i] == color:
                for j in range(y+1, i):
                    array[x][j] = color
                break
            if array[x][i] is None:
                break
    # 下方向查找，查找规则同上
    if y > 1 and array[x][y - 1] == 1 - color:
        for i in range(y - 2, -1, -1):
            if array[x][i] == color:
                for j in range(y-1, i, -1):
                    array[x][j] = color
                break
            if array[x][i] is None:
                break
    # 右上方向查找，查找规则同上
    if x < 6 and y < 6 and array[x + 1][y + 1] == 1 - color:
        for i in range(2, min(8 - x, 8 - y)):
            if array[x + i][y + i] == color:
                for j in range(1, i):
                    array[x+j][y+j] = color
                break
            if array[x + i][y + i] is None:
                break
    # 左上方向查找，查找规则同上
    if x > 1 and y < 6 and array[x - 1][y + 1] == 1 - color:
        for i in range(2, min(x + 1, 8 - y)):
            if array[x - i][y + i] == color:
                for j in range(1, i):
                    array[x - j][y + j] = color
                break
            if array[x - i][y + i] is None:
                break
    # 右下方向查找，查找规则同上
    if x < 6 and y > 1 and array[x + 1][y - 1] == 1 - color:
        for i in range(2, min(8 - x, y + 1)):
            if array[x + i][y - i] == color:
                for j in range(1, i):
                    array[x+j][y-j] = color
                break
            if array[x + i][y - i] is None:
                break
    # 左下方向查找，查找规则同上
    if x > 1 and y > 1 and array[x - 1][y - 1] == 1 - color:
        for i in range(2, min(x + 1, y + 1)):
            if array[x - i][y - i] == color:
                for j in range(1, i):
                    array[x-j][y-j] = color
                break
            if array[x - i][y - i] is None:
                break
    return array


def parity(tuple array, int player, int x, int y):
    """
    whether (x, y) is even or odd
    :param array: 8x8 matrix
    :param player: 1 for ai
    :param x:
    :param y:
    :return: False even, True odd
    """
    cdef int count = 0
    cdef int has_opposite = False
    for i in range(8):
        if array[x][i] is None:
            count += 1
        elif array[x][i] == 1 - player:
            has_opposite = True
    if count > 0 and count % 2 == 0 and has_opposite:
        return False
    count = 0
    has_opposite = False
    for i in range(8):
        if array[i][y] is None:
            count += 1
        elif array[i][y] == 1 - player:
            has_opposite = True
    return count > 0 and count % 2 == 0 and has_opposite
