"""
客户端管理器
"""
# !/usr/bin/python3
import socket
from Config.UiConfig import *


class ClientMgr:
    # 构造函数
    def __init__(self):
        pass

    # 初始化
    def Init(self):
        sk_obj = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sk_obj.connect((HOST, PORT))
        while True:
            d = (-1, -1)
            sk_obj.send(d)
            data = sk_obj.recv(1024)
            print(data)
