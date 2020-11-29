import requests
import sqlite3
from datetime import datetime, timedelta 

conn = sqlite3.connect('DB/test.db')
c = conn.cursor()

#192, 168, 1, 125

#requests.get('http://192.168.4.1/?ssid=JAZZTEL_Ut6&?pwd=fn9gn5v5b273&?api=r4GO1L6jR0srm2OXxZ5WnxI6lLC5fDM3cDHMDdgppGQ!')
#requests.get('http://192.168.4.1/?ssid=AndroidAP_9061&?pwd=4c47806fb28d&?api=r4GO1L6jR0srm2OXxZ5WnxI6lLC5fDM3cDHMDdgppGQ!')


"""
register = requests.post('http://127.0.0.1:5000/register',
                         json = {
                             'email':'sergicarol35@gmail.com',
                             'password': 'testtestpassword'})

new_api_key = register.json()['api_key']
print(new_api_key)
exit()
"""
new_api_key = "eSVorisNYFLZA2WzXAoTt3M5ViAoAK28IKwLknK5s1Y"
login = requests.post('http://127.0.0.1:5000/login',
                       json = {
                           'email':'sergicarol35@gmail.com',
                           'password': 'testtestpassword'})
print("Does user exist", login.text)

login = requests.post('http://127.0.0.1:5000/login',
                       json = {
                           'api_key': new_api_key})
print("Does key exist", login.json()['api_key'])
"""
register_arduino = requests.post('http://127.0.0.1:5000/register_arduino',
                                 json={
                                    'api_key': new_api_key,
                                    'arduino_name': "Test Arduino Name"
                                 })
print(register_arduino.json()['arduino_key'])
res = c.execute("SELECT * FROM arduino WHERE api_key = ?", (new_api_key, ))
print("Is arduino registered correctly", res.fetchone())
exit()
"""
arduino_key = "xsaRhdxt6otlpsFkWNK99plKUZu-dOccMTCbgQSMQmk"
register_arduino_fail = requests.post('http://127.0.0.1:5000/register_arduino',
                                 json={
                                    'api_key': "sadijfosfd",
                                    'arduino_name': "Test Arduino Name Fail"
                                 })
res = c.execute("SELECT * FROM arduino WHERE api_key = ?", ("sadijfosfd", ))
print("Is arduino registered correctly", res.fetchone())

ser_service = requests.post('http://localhost:5000/set_service',
                                 json={
                                    'api_key': new_api_key,
                                    'arduino_key': arduino_key,
                                    'service_name': "water_pump_2",
                                    'start_time': (datetime.now() -  timedelta(hours = 4)).isoformat(),
                                    'end_time': (datetime.now() -  timedelta(hours = 2)).isoformat()
                                 })
print("Server response", ser_service.text)
res = c.execute("SELECT * FROM services")
print("Is service registered correctly", res.fetchone())

ser_service = requests.post('http://localhost:5000/set_service',
                                 json={
                                    'api_key': new_api_key,
                                    'arduino_key': arduino_key,
                                    'service_name': "fan_2",
                                    'active': True
                                 })
print("Server response", ser_service.text)
res = c.execute("SELECT * FROM services")
print("Is service registered correctly", res.fetchone())