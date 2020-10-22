from .app import db
from mongoengine import *

class User(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    email = db.Column(db.String(100), unique=True)
    password = db.Column(db.String(100))
    api_key = db.Column(db.String(1000))
    
    def __repr__(self):
        return '<email {}>'.format(self.email)

class Arduino(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    arduino_id = db.Column(db.String(100), unique=True)
    user = db.relationship('User', backref='email', lazy=True)

class Data(Document):
    temperature = FloatField(required=True)
    humidity = FloatField(max_length=50)
    water_temp = FloatField(max_length=50)
    water_ph = FloatField(max_length=50)
    water_electrodes = FloatField(max_length=50)
    api_key = StringField(max_length=50)