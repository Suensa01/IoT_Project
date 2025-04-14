from flask import Flask, request, render_template, jsonify
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///healthcare.db'
db = SQLAlchemy(app)

class PatientData(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    heart_rate = db.Column(db.Integer)
    temperature = db.Column(db.Float)
    timestamp = db.Column(db.DateTime, default=datetime.utcnow)

@app.route('/')
def index():
    data = PatientData.query.order_by(PatientData.timestamp.desc()).limit(10).all()
    return render_template('index.html', data=data)

@app.route('/api/health', methods=['POST'])
def receive_data():
    content = request.get_json()
    try:
        new_entry = PatientData(
            heart_rate=content['heart_rate'],
            temperature=content['temperature']
        )
        db.session.add(new_entry)
        db.session.commit()
        return jsonify({'message': 'Health data received'}), 201
    except Exception as e:
        return jsonify({'error': str(e)}), 400

if __name__ == '__main__':
    with app.app_context():
        db.create_all()
    app.run(debug=True)
