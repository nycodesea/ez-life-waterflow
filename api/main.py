from fastapi import FastAPI

app = FastAPI()

@app.post("/sensor")
def sensor(data: dict):
    print("受信:", data)
    return {"status": "ok"}
