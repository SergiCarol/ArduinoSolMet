from flask import Flask
from flask_sqlalchemy import SQLAlchemy
import os


app = Flask(__name__)
app.config.from_object(os.environ['APP_SETTINGS'])
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)

@api.route('/')
def index():
    return 'Hello'


@api.route('/api', methods=['GET', 'POST'])
def home():
    json_data = request.get_json()
    api.logger.info("Receiving data from %s", request.remote_addr)
    api.logger.info("Received data %s", json_data)
    return "ok", 200

@api.route('/add', methods=['GET'])
def add():
    print(request.args.get('api_key'))
    return "data"
