from flask import Flask, request, render_template, jsonify
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///agriculture.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)

class FarmData(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    soil_moisture = db.Column(db.Float)
    temperature = db.Column(db.Float)
    humidity = db.Column(db.Float)
    rain = db.Column(db.String(10))
    timestamp = db.Column(db.DateTime, default=datetime.utcnow)

@app.before_first_request
def create_tables():
    db.create_all()

@app.route('/')
def index():
    data = FarmData.query.order_by(FarmData.timestamp.desc()).limit(10).all()
    return render_template('index.html', data=data)

@app.route('/api/farm', methods=['POST'])
def receive_data():
    content = request.get_json()
    try:
        entry = FarmData(
            soil_moisture=content['soil_moisture'],
            temperature=content['temperature'],
            humidity=content['humidity'],
            rain=content['rain']
        )
        db.session.add(entry)
        db.session.commit()
        return jsonify({'message': 'Data saved'}), 201
    except Exception as e:
        return jsonify({'error': str(e)}), 400

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
