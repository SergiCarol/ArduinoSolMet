from flask import Flask, request, jsonify
from mongoengine import connect
import os
import secrets
import datetime
import pytz

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///C:\\Users\\Sergi\\Documents\\Projects\\arduinosolo-webpage\\ArduinoServer\\Server\\DB\\test.db'
from models import db, User, Arduino, Schedule, Data

db.init_app(app)

client = connect('arduino_data')

SERVICE_OPTIONS = ['water_pump_1', 'water_pump_2', 'fan',
                   'fan_2', 'air_pump']

@app.route('/register', methods=['POST'])
def registe_user():
    email = request.args.get('email')
    password = request.args.get('password')
    api_key = secrets.token_urlsafe(32)
    try:
        user = User(
            email=email,
            password=password,
            api_key=api_key
        )
        db.session.add(user)
        db.session.commit()
        return api_key
    except Exception as e:
	    return(str(e)), 404

@app.route("/login", methods=['POST'])
def login_user():
    email = request.args.get('email')
    password = request.args.get('password')
    api_key = request.form.get('api_key')
    if api_key is not None:
        user = User.query.filter_by(api_key=api_key).first()
        return api_key
    try:
        user = User.query.filter_by(email=email, password=password).first()
        print(user)
        if user is not None:
            return user.api_key
        return "False"
    except Exception as e:
	    return(str(e))

@app.route('/register_arduino', methods=['POST'])
def register_arduino():
    api_key = request.form.get('api_key')
    user = _get_user(api_key)
    if user == False :
        return "False"
    try:
        arduino_key = secrets.token_urlsafe(32)
        arduino = Arduino(
            api_key=arduino_key,
            arduino_name = request.form.get('arduino_name'),
            user = user
        )
        print("New arduino added", arduino)
        db.session.add(arduino)
        db.session.commit()
    except Exception as e:
        return "False"
    return arduino_key

@app.route('/upload', methods=['POST'])
def upload():
    print("Receiving data", request.json)
    key = request.json.get('api_key')
    if not _get_arduino(key):
        return "not_logged"
    try:
        client.deleteOne({"api_key": key})
    except:
        pass
    record = Data(**request.json)
    #record.save()
    services = Schedule.query.filter_by(arduino=_get_user(key))
    
    service_json = dict()
    now = datetime.datetime.now().hour

    for service in services:
        service_json[service.service] = service.start_time.hour <= now <= service.end_time.hour
    print("Returning data", service_json)
    return jsonify(service_json)

@app.route('/get', methods=['GET', 'POST'])
def get_data():
    user_key = request.args.get('api_key')
    if not _get_user(user_key):
        return "False"
    
    key = request.args.get('arduino_key')

    record = Data(api_key=key)
    print("Getting record ", record)
    return record

@app.route('/set_service', methods=['POST'])
def set_service():
    print(request.form)
    key = request.form.get('api_key')
    if not _get_user(key):
        return "False"
    
    key = request.form.get('arduino_key')

    service_name = request.form.get('service_name')
    if service_name not in SERVICE_OPTIONS:
        print("No service")
        return "Service not found", 
    start_time = request.form.get('start_time')
    end_time = request.form.get('end_time')

    start_time = datetime.datetime.fromisoformat(start_time)
    end_time = datetime.datetime.fromisoformat(end_time)
    print(type(start_time), type(end_time))
    try:
        print("Inserting new service")
        schedule = Schedule(
            service=service_name,
            start_time=start_time,
            end_time=end_time,
            arduino=_get_arduino(key)
        )
        db.session.add(schedule)
        db.session.commit()
        return "ok"
    except Exception as e:
        print(e)
        return "Error"

def _get_user(api_key):
    user = User.query.filter_by(api_key=api_key).first()
    if user is None:
        return False
    return user

def _get_arduino(api_key):
    arduino = Arduino.query.filter_by(api_key=api_key).first()
    if arduino is None:
        return False
    return arduino