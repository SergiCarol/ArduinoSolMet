from flask import Flask, request, jsonify
from mongoengine import connect
from flask_cors import CORS
#from flask_migrate import Migrate
import os
import secrets
import datetime
import pytz

app = Flask(__name__)
#app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///C:\\Users\\Sergi\\Documents\\Projects\\arduinosolo-webpage\\ArduinoServer\\Server\\DB\\test.db'
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
    print(email, password)
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
    key = request.json.get('api_key')
    if not _get_user(key):
        return "False"

    key = request.json.get('arduino_key')

    service_name = request.json.get('service_name')
    if service_name not in SERVICE_OPTIONS:
        print("No service")
        return "Service not found",
    start_time = request.json.get('start_time')
    end_time = request.json.get('end_time')
    if start_time:
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
    else:
        try:
            print("Inserting new service")
            schedule = Schedule(
                service=service_name,
                active=True,
                arduino=_get_arduino(key)
            )
            db.session.add(schedule)
            db.session.commit()
            return "ok"
        except Exception as e:
            return "Error"

@app.route('/get_arduinos', methods=['GET'])
def get_arduinos():
    key = request.args.get('api_key')
    user = _get_user(key)
    if not user:
        return "False"
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
        return "False"

    key = request.args.get('arduino_key')
    arduino = _get_arduino(key)

    services = Schedule.query.filter_by(arduino_id=arduino.id)
    
    payload = []
    for service in services:
        data = {
            'name': service.service,
            'start_time': service.start_time,
            'end_time': service.end_time,
            'active': service.active,
            'id': service.id
        }
        payload.append(data)
    return jsonify({'arduino': payload})

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