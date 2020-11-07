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
        return api_key, 200
    except Exception as e:
	    return(str(e)), 404

@app.route("/login", methods=['POST'])
def login_user():
    email = request.args.get('email')
    password = request.args.get('password')
    try:
        user = User.query.filter_by(email=email, password=password)
        if len(user) == 1:
            return user.api_key, 200
        return "False", 404
    except Exception as e:
	    return(str(e)), 404

@app.route('/register_arduino', methods=['POST'])
def register_arduino():
    api_key = secrets.token_urlsafe(32)
    request.args.get('password')
    user = _get_user(api_key)
    if user == False :
        return "False", 404
    arduino = Arduino(
        api_key=api_key,
        arduino_name = request.args.get('arduino_name'),
        user = user
    )
    db.session.add(arduino)
    db.session.commit()
    return api_key, 200

@app.route('/upload', methods=['GET'])
def upload():
    print("Receiving data", request.args)
    key = request.args.get('api_key')
    if not _get_arduino(key):
        return "not_logged", 404
    try:
        client.deleteOne({"api_key": key})
    except:
        pass
    record = Data(**request.args)
    record.save()
    services = Schedule.query.filter_by(arduino=_get_user(key))
    
    service_json = dict()
    now = datetime.datetime.now()

    for service in services:
        service_json[service.service] = service.start_time <= now <= service.end_time

    return jsonify(service_json)

@app.route('/get', methods=['GET', 'POST'])
def get_data():
    user_key = request.args.get('api_key')
    if not _get_user(user_key):
        return "False", 404
    
    key = request.args.get('arduino_key')

    record = Data(api_key=key)
    print("Getting record ", record)
    return record, 200

@app.route('/set_service', methods=['POST'])
def set_service():
    key = request.args.get('api_key')
    if not _get_user(key):
        return "False", 404
    
    key = request.args.get('arduino_key')

    service_name = request.args.get('service_name')
    if service_name not in SERVICE_OPTIONS:
        return "Service not found", 
    start_time = request.args.get('start_time')
    end_time = request.args.get('end_time')

    start_time = datetime.datetime.fromisoformat(start_time)
    end_time = datetime.datetime.fromisoformat(end_time)

    try:
        schedule = Schedule(
            service=service_name,
            start_time=start_time,
            end_time=end_time,
            arduino=_get_arduino(key)
        )
        db.session.add(user)
        db.session.commit()
        return "ok", 202
    except Exception as e:
	    return(str(e)), 404

def _get_user(api_key):
    user = User.query.filter_by(api_key=api_key).first()
    if user is None:
        return False
    return user[0]

def _get_arduino(api_key):
    arduino = Arduino.query.filter_by(api_key=api_key).first()
    if arduino is None:
        return False
    return arduino[0]