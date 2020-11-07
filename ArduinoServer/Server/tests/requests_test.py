import requests
import sqlite3


conn = sqlite3.connect('DB/test.db')
c = conn.cursor()

register = requests.post('http://127.0.0.1:5000/register',
                         data = {
                             'email':'sergicarol35@gmail.com',
                             'password': 'testtestpassword'})

new_api_key = "r4GO1L6jR0srm2OXxZ5WnxI6lLC5fDM3cDHMDdgppGQ"

login = requests.post('http://127.0.0.1:5000/login',
                       data = {
                           'email':'sergicarol35@gmail.com',
                           'password': 'testtestpassword'})
print("Does user exist", login.text)

login = requests.post('http://127.0.0.1:5000/login',
                       data = {
                           'api_key': new_api_key})
print("Does key exist", login.text)

register_arduino = requests.post('http://127.0.0.1:5000/register_arduino',
                                 data={
                                    'api_key': new_api_key,
                                    'arduino_name': "Test Arduino Name"
                                 })
res = c.execute("SELECT * FROM arduino WHERE api_key = ?", (new_api_key, ))
print("Is arduino registered correctly", res.fetchone())
register_arduino_fail = requests.post('http://127.0.0.1:5000/register_arduino',
                                 data={
                                    'api_key': "sadijfosfd",
                                    'arduino_name': "Test Arduino Name Fail"
                                 })
res = c.execute("SELECT * FROM arduino WHERE api_key = ?", ("sadijfosfd", ))
print("Is arduino registered correctly", res.fetchone())
