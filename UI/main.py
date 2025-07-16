from flask import Flask, render_template, request, jsonify
import serial
import serial.tools.list_ports

app = Flask(__name__)
import serial

ports = serial.tools.list_ports.comports()
for port in ports:
    print(f"{port.device} - {port.description}")

try:
    ser = serial.Serial('COM5', 115200, timeout=1)
except serial.SerialException as e:
    print(f"[ERROR] Could not open serial port: {e}")
    ser = None


@app.route("/")
def index():
    return render_template("index.html")

@app.route("/send", methods=["POST"])
def send():
    cmd = request.json["command"]
    ser.write((cmd + "\n").encode())
    response = ser.readline().decode().strip()
    return jsonify({"response": response})

if __name__ == "__main__":
    app.run(debug=True)
