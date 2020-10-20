from flask import Blueprint
from flask import request, logging


api = Blueprint('main', __name__)  # Needs to be moved into a separate file


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
