import requests

register = requests.post('http://127.0.0.1:5000/register',
                         data = {
                             'email':'sergicarol35@gmail.com',
                             'password': 'testtestpassword'})

new_api_key = register.text
print(new_api_key)

login = requests.post('http://127.0.0.1:5000/login',
                       data = {
                           'email':'sergicarol35@gmail.com',
                            'password': 'testtestpassword'})
print(login.text)