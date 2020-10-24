from flask import Flask, request
import os
from models import db
from mongoengine import connect
import secrets

app = Flask(__name__)
db.init_app(app)
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
SQLALCHEMY_DATABASE_URI = "postgresql:///arduino"
client = connect('arduino_data')


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
	    return(str(e))

@app.route("/login", methods=['POST'])
def login_user():
    email = request.args.get('email')
    password = request.args.get('password')
    try:
        user = User.query.filter_by(email=email, password=password)
        if len(user) == 1:
            return user.api_key
        return
    except Exception as e:
	    return(str(e))

@app.route('/register_arduino', methods=['POST'])
def register_arduino():
    api_key = secrets.token_urlsafe(32)
    request.args.get('password')
    user = _get_user(api_key)
    if user == False :
        return False
    arduino = Arduino(
        api_key=api_key,
        arduino_name = request.args.get('arduino_name'),
        user = user
    )
    db.session.add(arduino)
    db.session.commit()
    return api_key

@app.route('/upload', methods=['GET', 'POST'])
def upload():
    print("Receiving data", request.args)
    key = request.args.get('api_key')
    if not _get_arduino(key):
        return "not_logged"
    try:
        client.deleteOne({"api_key": key})
    except:
        pass
    record = Data(**request.args)
    record.save()
    return "ok", 200

@app.route('/get', methods=['GET', 'POST'])
def get_data():
    key = request.args.get('api_key')
    if not _get_user(key):
        return False
    record = Data(api_key=key)
    print("Getting record ", record)
    return record, 200

def _get_user(api_key):
    user = User.query.filter_by(api_key=api_key)
    if len(user) == 0:
        return False
    return user[0]

def _get_arduino(api_key):
    arduino = Arduino.query.filter_by(api_key=api_key)
    if len(arduino) == 0:
        return False
    return arduino[0]