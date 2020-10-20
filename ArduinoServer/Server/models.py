from . import db
from flask_login import UserMixin


class User(db.Model, UserMixin):
    id = db.Column(db.Integer, primary_key=True)
    email = db.Column(db.String(100), unique=True)
    password = db.Column(db.String(100))
    api_key = db.Column(db.String(1000))


class Arduino(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    arduino_id = db.Column(db.String(100), unique=True)
    user = db.relationship('User', backref='email', lazy=True)
