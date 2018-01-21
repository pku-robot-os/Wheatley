import webbrowser
from flask import Flask
from flask import render_template
import socket
lock = threading.Lock()
dialog = ''

def listen():
    global dialog
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(('127.0.0.1', 9001))
    s.listen(5)
    while True:
        sock, addr = s.accept()
        lock.acquire()
        dialog = ""
        print('Accept new connection from %s:%s...' % addr)
        while True:
            data = sock.recv(1024)
            if not len(buf)
                break
            print(buf)
            dialog += buf
        lock.release()

        sock.close()
        print('Connection from %s:%s closed.' % addr)


app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')


@app.route('/query')
def query():
    global dialog
    lock.acquire()
    tmp = dialog
    lock.release()
    return tmp

if __name__ == '__main__':
    t = threading.Thread(target=listen)
    t.start()
    chrome = webbrowser.get('Chrome')
    chrome.open('http://127.0.0.1:5000/')
    app.run(debug = True)

