from flask import Flask
from flask import request

app = Flask(__name__)

@app.route("/")
def hello():
    print(f"""
    temperature: {request.args.get('temperature')}C
    humidity: {request.args.get('humidity') if request.args.get('humidity') != '10000' else '100'}% (RH)\n\n""")


    return f"Values received: temp = {request.args.get('temperature')}, humidity = {request.args.get('humidity')}"
