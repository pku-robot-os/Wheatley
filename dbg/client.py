import socket

# 建立连接:
while True :
    x = input()
# 创建一个socket:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('127.0.0.1', 9002))
    # vim: ts=4 sw=4 sts=4 expandtab
    s.send(x.encode('utf-8'))
    s.close()

