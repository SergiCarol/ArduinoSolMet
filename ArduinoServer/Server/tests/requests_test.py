import requests
import sqlite3
import time
from datetime import datetime, timedelta 

#conn = sqlite3.connect('DB/test.db')
#c = conn.cursor()

#192, 168, 1, 125

#requests.get('http://192.168.4.1/?ssid=JAZZTEL_Ut6&?pwd=fn9gn5v5b273&?api=r4GO1L6jR0srm2OXxZ5WnxI6lLC5fDM3cDHMDdgppGQ!')
#requests.get('http://192.168.4.1/?ssid=AndroidAP_9061&?pwd=4c47806fb28d&?api=r4GO1L6jR0srm2OXxZ5WnxI6lLC5fDM3cDHMDdgppGQ!')
requests.get('http://192.168.1.4/?ssid=SOLMET-OFICINA&?pwd=8S4R375SIN&?api=so11zAoEGpvwDaNxO6gASqysSPXGA2Vi7p3JTx8ct5k!')

"""
register = requests.post('http://127.0.0.1:5000/register',
                         json = {
                             'email':'sergicarol35@gmail.com',
                             'password': 'testtestpassword'})

new_api_key = register.json()['api_key']
print(new_api_key)


login = requests.post('http://127.0.0.1:5000/login',
                       json = {
                           'email':'sergicarol35@gmail.com',
                           'password': 'testtestpassword'})
print("Does user exist", login.text)

login = requests.post('http://127.0.0.1:5000/login',
                       json = {
                           'api_key': new_api_key})
print("Does key exist", login.json()['api_key'])

register_arduino = requests.post('http://127.0.0.1:5000/register_arduino',
                                 json={
                                    'api_key': new_api_key,
                                    'arduino_name': "Test Arduino Name 4"
                                 })
                                 
print(register_arduino.json())

arduino_key = register_arduino.json()['api_key']

ser_service = requests.post('http://localhost:5000/set_service',
                                 json={
                                    'api_key': new_api_key,
                                    'arduino_key': arduino_key,
                                    'service_name': "water_pump_2",
                                    'start_time': (datetime.now() -  timedelta(hours = 4)).isoformat(),
                                    'end_time': (datetime.now() -  timedelta(hours = 2)).isoformat(),
                                    'active': 'false'
                                 })
print("Server response", ser_service.text)

ser_service = requests.post('http://localhost:5000/set_service',
                                 json={
                                    'api_key': new_api_key,
                                    'arduino_key': arduino_key,
                                    'service_name': "fan_2",
                                    'start_time': (datetime.now() -  timedelta(hours = 4)).isoformat(),
                                    'end_time': (datetime.now() -  timedelta(hours = 2)).isoformat(),
                                    'active': 'true'
                                 })
print("Server response", ser_service.text)

#r
ser_service = requests.post('http://localhost:5000/set_service',
                                 json={
                                    'api_key': new_api_key,
                                    'arduino_key': arduino_key,
                                    'service_name': "air_pump",
                                    'start_time': (datetime.now() -  timedelta(hours = 4)).isoformat(),
                                    'end_time': (datetime.now() -  timedelta(hours = 2)).isoformat(),
                                    'active': 'true'
                                 })
print(ser_service.text)
services = requests.get('http://localhost:5000/get_services',
                                 {
                                    'api_key': new_api_key,
                                    'arduino_key': arduino_key
                                 })
print(services.text)
time.sleep(2)
services = requests.get('http://localhost:5000/get_arduinos',
                                 {
                                    'api_key': new_api_key,
                                 })
print(services.text)
"""