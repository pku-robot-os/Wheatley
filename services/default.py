# sudo pip install pipenv
# sudo pipenv install requests
import requests
import json
import sys

if __name__=="__main__":
	s = input()
	p = {'key':'43f0f552df74488580a4a7b06af4a134', 'info':s, 'userid':'204842'}
	r = requests.post('http://www.tuling123.com/openapi/api', params=p)
	j = json.loads(r.text)
	print(j['text'])
