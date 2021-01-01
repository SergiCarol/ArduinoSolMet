from flask import Flask, request, jsonify
from mongoengine import connect
from flask_cors import CORS
from bson import json_util
#from flask_migrate import Migrate
import os
import secrets
import datetime
import pytz
import json

app = Flask(__name__)
#pp.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///C:\\Users\\Sergi\\Documents\\Projects\\arduinosolo-webpage\\ArduinoServer\\Server\\DB\\test.db'
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///C:\\Users\\Sergi\\Documents\\Arduino_Repo\\ArduinoSolMet\\ArduinoServer\\Server\\DB\\test.db'
from models import db, User, Arduino, Schedule, Data
CORS(app)
db.init_app(app)
#migrate = Migrate(app, db)
client = connect('arduino_data')

SERVICE_OPTIONS = ['water_pump_1', 'water_pump_2', 'fan',
                   'fan_2', 'air_pump']

@app.route('/register', methods=['POST'])
def registe_user():
    print(request)
    email = request.json.get('email')
    password = request.json.get('password')
    api_key = secrets.token_urlsafe(32)
    try:
        user = User(
            email=email,
            password=password,
            api_key=api_key
        )
        print(user)
        db.session.add(user)
        db.session.commit()
        print(api_key)
        return jsonify({'api_key': api_key})
    except Exception as e:
        print(str(e))
        return "False"

@app.route("/login", methods=['POST'])
def login_user():
    print(request.json)
    email = request.json.get('email')
    password = request.json.get('password')
    api_key = request.json.get('api_key')
    if api_key is not None:
        user = User.query.filter_by(api_key=api_key).first()
        return jsonify({'api_key': api_key})
    try:
        user = User.query.filter_by(email=email, password=password).first()
        if user is not None:
            return jsonify({'api_key': user.api_key})
        return "False"
    except Exception as e:
        print(str(e))
        return "False"

@app.route('/register_arduino', methods=['POST'])
def register_arduino():
    api_key = request.json.get('api_key')
    user = _get_user(api_key)
    if user == False :
        return "False"
    try:
        arduino_key = secrets.token_urlsafe(32)
        arduino = Arduino(
            api_key=arduino_key,
            arduino_name = request.json.get('arduino_name'),
            user = user
        )
        db.session.add(arduino)
        db.session.commit()
    except Exception as e:
        print(e)
        return "False"
    return jsonify({'api_key': arduino_key})

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
    record.save()
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
    try:
        record = Data.objects(api_key=key)[0]
        if record.temperature is not None:
            data = {
                'temperature': record.temperature,
                'humidity': record.humidity,
                'water_temp': record.water_temperature,
                'water_ph': record.water_ph,
                'water_electrodes': record.water_electrodes
            }
        else: 
            data = {}
    except:
        data = {'status': 'Error: no data'}
    return jsonify(data)

@app.route('/set_service', methods=['POST'])
def set_service():
    key = request.json.get('api_key')
    print(key)
    if not _get_user(key):
        print("no_user")
        return "False"

    arduino_key = request.json.get('arduino_key')
    service_id = request.json.get('service_id')

    service_name = request.json.get('service_name')
    if service_name not in SERVICE_OPTIONS:
        print(service_name, SERVICE_OPTIONS)
        return "Service not found"

    start_time = request.json.get('start_time')
    end_time = request.json.get('end_time')
    active = request.json.get('active')

    print("Start time", start_time)
    print("End time", end_time)
    print("Start time", type(start_time))
    print("End time", type(end_time))
    print("Active", active)
    if active:
        active = 1
    else:
        active = 0
    service = Schedule.query.filter_by(id=service_id).first()

    if service is not None:
        try:
            print("Updating  service")
            service.start_time=str(start_time)
            service.end_time=str(end_time)
            service.active=active
            db.session.commit()
            return jsonify({'status': 'ok'})
        except Exception as e:
            print(e)
            return jsonify({'status': 'Error' + e})
    else:
        
        print("Inserting new service")
        schedule = Schedule(
            service=service_name,
            start_time=start_time,
            end_time=end_time,
            arduino_id=_get_arduino(arduino_key).api_key,
            active=active,
        )
        db.session.add(schedule)
        db.session.commit()
        return jsonify({'status': 'ok'})


@app.route('/get_arduinos', methods=['GET'])
def get_arduinos():
    key = request.args.get('api_key')
    user = _get_user(key)
    print(user)
    if not user:
        return jsonify({'status': 'Error: user not logged'})
    arduinos = Arduino.query.filter_by(user_id=user.id)
    payload = []
    for arduino in arduinos:
        data = {
            'name': arduino.arduino_name,
            'id': arduino.id,
            'api_key': arduino.api_key
        }
        payload.append(data)
    return jsonify({'arduino': payload})

@app.route('/get_services', methods=['GET'])
def get_services():
    key = request.args.get('api_key')
    if not _get_user(key):
        return jsonify({'status': 'Error: not logged'})

    key = request.args.get('arduino_key')
    arduino = _get_arduino(key)
    if not arduino:
        return jsonify({'status': 'Error: Arduino does not exist'})
    
    services = Schedule.query.filter_by(arduino_id=arduino.api_key)
    #services = Schedule.query.all()
    #print(services)

    payload = []
    for service in services:
        print(service)
        print(service.active)
        data = {
            'name': service.service,
            'start_time': service.start_time,
            'end_time': service.end_time,
            'active': service.active,
            'id': service.id
        }
        payload.append(data)
    return jsonify({'arduino': payload})


@app.route('/delete_service', methods=['POST'])
def delete_service():
    key = request.json.get('api_key')
    print(key)
    if not _get_user(key):
        return jsonify({'status': 'Error'})

    service_id = request.json.get('service_id')
    service = Schedule.query.filter_by(id=service_id).first()
    db.session.delete(service)
    db.session.commit()
    return jsonify({'response': 'ok'})

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