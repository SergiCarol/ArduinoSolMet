import requests
import sqlite3
from datetime import datetime, timedelta 

#conn = sqlite3.connect('DB/test.db')
#c = conn.cursor()

#192, 168, 1, 125

#requests.get('http://192.168.4.1/?ssid=JAZZTEL_Ut6&?pwd=fn9gn5v5b273&?api=r4GO1L6jR0srm2OXxZ5WnxI6lLC5fDM3cDHMDdgppGQ!')
requests.get('http://192.168.4.1/?ssid=AndroidAP_9061&?pwd=4c47806fb28d&?api=r4GO1L6jR0srm2OXxZ5WnxI6lLC5fDM3cDHMDdgppGQ!')


"""
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

ser_service = requests.post('http://localhost:5000/set_service',
                                 data={
                                    'api_key': "r4GO1L6jR0srm2OXxZ5WnxI6lLC5fDM3cDHMDdgppGQ",
                                    'arduino_key': "r4GO1L6jR0srm2OXxZ5WnxI6lLC5fDM3cDHMDdgppGQ",
                                    'service_name': "water_pump_2",
                                    'start_time': (datetime.now() -  timedelta(hours = 4)).isoformat(),
                                    'end_time': (datetime.now() -  timedelta(hours = 2)).isoformat()
                                 })
print("Server response", ser_service)
res = c.execute("SELECT * FROM services")
print("Is service registered correctly", res.fetchone())
"""