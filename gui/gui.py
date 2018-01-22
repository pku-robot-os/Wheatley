import webbrowser
from flask import Flask
from flask import render_template
import socket
import threading
import sys
import time
lock = threading.Lock()
dialog = '' # 'test'
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
#    lock.acquire()
	tmp = dialog
	print(dialog, file=sys.stdout)
#    lock.release()
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
#        lock.acquire()
		buf = b''
		print('Accept new connection from %s:%s...' % addr)
		while True:
			time.sleep(0.100)
			tmp = sock.recv(1024)
			if not len(tmp):
				break
			buf+=tmp
		try:
			dialog = buf.decode("utf-8")
		except UnicodeDecodeError:
			print("Decode failed.")
#        lock.release()

		sock.close()
		print('Connection from %s:%s closed.' % addr)
