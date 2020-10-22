from flask import Flask, request
from flask_sqlalchemy import SQLAlchemy
import os
from mongoengine import connect
from models import User, Arduino, Data
import secrets


api = Flask(__name__)
api.config.from_object(os.environ['APP_SETTINGS'])
api.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
SQLALCHEMY_DATABASE_URI = "postgresql:///arduino"
client = connect('arduino_data')
db = SQLAlchemy(api)

@api.route('/')
def index():
    return 'Hello'

@api.route('/register', methods=['POST'])
def register():
    email = request.args.get('email')
    password = request.args.get('password')
    api_key = secrets.token_urlsafe(16)
  
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

@api.route("/login", methods=['POST'])
def login():
    email = request.args.get('email')
    password = request.args.get('password')
    try:
        user = User.query.filter_by(email=email, password=password)
        if len(user) == 1:
            return user.api_key
        return
    except Exception as e:
	    return(str(e))

@api.route('/upload', methods=['GET', 'POST'])
def upload():
    print("Receiving data", request.args)
    key = request.args.get('api_key')
    user = User.query.filter_by(api_key=key)
    if len(user) == 0:
        return "not_logged"
    # Delete previous entry if it exists
    try:
        client.deleteOne({"api_key": key})
    except:
        pass
    record = Data(**request.args)
    record.save()
    return "ok", 200

@api.route('/get', methods=['GET', 'POST'])
def get_data():
    key = request.args.get('api_key')
    user = User.query.filter_by(api_key=key)
    if len(user) == 0:
        return "not_logged"
    record = Data(api_key=key)
    print("Getting record ", record)
    return record, 200
