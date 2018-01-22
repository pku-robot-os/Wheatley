import webbrowser
from flask import Flask
from flask import render_template
import socket
import threading
import sys
lock = threading.Lock()
dialog = 'test'
import logging
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')


@app.route('/query')
def query():
    global dialog
    lock.acquire()
    tmp = dialog
    print(dialog, file=sys.stdout)
    lock.release()
    return tmp

def flaskThread():
    app.run()

if __name__ == '__main__':
    t = threading.Thread(target=flaskThread, args=())
    t.start()

    webbrowser.open('http://127.0.0.1:5000/')
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(('127.0.0.1', 9002))
    s.listen(5)
    while True:
        sock, addr = s.accept()
        lock.acquire()
        dialog = ""
        print('Accept new connection from %s:%s...' % addr)
        while True:
            buf = sock.recv(1024)
            if not len(buf):
                break
            dialog += buf.decode("utf-8")
        lock.release()

        sock.close()
        print('Connection from %s:%s closed.' % addr)
