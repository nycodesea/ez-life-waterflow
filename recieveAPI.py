from flask import Flask, request
import json
from datetime import datetime

app = Flask(__name__)


@app.route("/api/data", methods=["POST"])
def receive_data():
    data = request.json

    print("受信！")
    data["timestamp"] = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    with open("sensor_data.json", "w", encoding="utf-8") as f:
        json.dump(data, f, ensure_ascii=False, indent=2)
    print(data)

    return {"status": "ok"}


app.run(host="0.0.0.0", port=5000)
