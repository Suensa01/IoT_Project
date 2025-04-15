import requests
import random
import time

API_URL = "http://127.0.0.1:5000/api/farm"

while True:
    data = {
        "soil_moisture": round(random.uniform(10.0, 90.0), 2),
        "temperature": round(random.uniform(15.0, 40.0), 1),
        "humidity": round(random.uniform(30.0, 90.0), 1),
        "rain": random.choice(["yes", "no"])
    }

    response = requests.post(API_URL, json=data)
    print("Response:", response.json())
    time.sleep(5)
