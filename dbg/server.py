import threading
import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('127.0.0.1', 9002))
s.listen(5)
print('Waiting for connection...')
def tcplink(sock, addr):
    print('Accept new connection from %s:%s...' % addr)
    while True:
        data = sock.recv(1024)
        if not data or data.decode('utf-8') == 'exit':
            break
        print(data.decode('utf-8'))
    sock.close()
    print('Connection from %s:%s closed.' % addr)
while True:
    # 接受一个新连接:
    sock, addr = s.accept()
    # 创建新线程来处理TCP连接:
    t = threading.Thread(target=tcplink, args=(sock, addr))
    t.start()
