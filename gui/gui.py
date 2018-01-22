import webbrowser
from flask import Flask
from flask import render_template
import socket
import threading
import sys
import time
lock = threading.Lock()
question = ''
answer = '你好我是Wheatly\n'
import logging
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')


@app.route('/question')
def query():
    global question
    tmp = question
    return tmp

@app.route('/answer')
def ans():
    global answer
    tmp = answer
    return tmp

def flaskThread():
    app.run()


if __name__ == '__main__':
    t = threading.Thread(target=flaskThread, args=())
    t.start()

    webbrowser.get('firefox').open('http://127.0.0.1:5000/')
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(('127.0.0.1', 9002))
    s.listen(5)
    while True:
        sock, addr = s.accept()
        buf = b''
        print('Accept new connection from %s:%s...' % addr)
        chunks = []
        bytes_recd = 0
        while True:
            chunk = sock.recv(2048)
            if chunk == b'':
                break
            chunks.append(chunk)
            bytes_recd = bytes_recd + len(chunk)
        buf = b''.join(chunks)
        sock.close()
        try:
            dialog = buf.decode("utf-8")
            dialog = dialog.split('\n')
            dialog = [x for x in dialog if len(x.strip())]
            if len(dialog) >= 1 and question != "__no_input__" :
                question = dialog[0]
            if len(dialog) >= 2:
                answer = dialog[1]
        except UnicodeDecodeError:
            print("Decode failed.")


    print('Connection from %s:%s closed.' % addr)
