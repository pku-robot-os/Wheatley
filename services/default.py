# sudo pip install pipenv
# sudo pipenv install requests
import requests
import json
import sys
p = {'key':'486dba631b5c438ca1e8f419e61eab3e', 'info':sys.argv[1], 'userid':'204842'}
r = requests.post('http://www.tuling123.com/openapi/api', params=p)
j = json.loads(r.text)
print(j['text'])