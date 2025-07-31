from flask import Flask, render_template, request, redirect, url_for
import serial
import time

# Configure your COM port and baud rate here
COM_PORT = 'COM9'     # Replace with your COM port
BAUD_RATE = 115200      # Match with the device

# ✅ Create app first
app = Flask(__name__)
#define MODE_IDLE           0u
#define MODE_CONFIG_INIT    1u
#define MODE_CONFIG         2u
#define MODE_NORMAL         3u
#define MODE_SHUTDOWN       4u

# In-memory variables
evm_state = "INIT"
candidates = {
    "Candidate 1": "",
    "Candidate 2": "",
    "Candidate 3": "",
    "Candidate 4": "",
    "Candidate 5": ""
}

# In-memory votes
votes = {
    "Candidate 1": 0,
    "Candidate 2": 0,
    "Candidate 3": 0,
    "Candidate 4": 0,
    "Candidate 5": 0
}

# ✅ Then define routes
@app.route("/", methods=["GET", "POST"])
def home():
    return render_template("index.html", evm_state=evm_state, candidates=candidates)



@app.route("/create", methods=["GET", "POST"])
def create():
    global evm_state, candidates, votes

    votes_display = None  # Used to show votes when button is pressed

    if request.method == "POST":
        action = request.form.get("action")
        print(f"[INFO] Button pressed: {action}")

        if action == "get_state":
            print(f"[INFO] Current EVM state: {evm_state}")

        elif action == "set_state":
            evm_state = request.form.get("evm_state", evm_state)
            print(f"[INFO] EVM state set to: {evm_state}")

        elif action == "reset_evm":
            evm_state = "INIT"
            print("[INFO] EVM Reset!")

        elif action == "configure_candidate":
            selected = request.form.get("candidate_select")
            name = request.form.get("candidate_name", "").strip()
            if selected in candidates and name:
                candidates[selected] = name
                print(f"[INFO] Configured {selected} as {name}")

        elif action == "erase_config":
            for key in candidates:
                candidates[key] = ""
            print("[INFO] All candidates erased.")

        elif action == "read_votes":
            # Simulate reading votes (replace with your actual EVM logic)
            votes_display = {c: votes[c] for c in votes}
            print("[INFO] Votes read:", votes_display)

        return render_template(
            "create.html",
            evm_state=evm_state,
            candidates=candidates,
            votes_display=votes_display
        )

    return render_template(
        "create.html",
        evm_state=evm_state,
        candidates=candidates,
        votes_display=None
    )


@app.route("/gallery")
def gallery():
    return render_template("gallery.html")


@app.route("/evm", methods=["GET", "POST"])
def evm():
    global evm_state, candidates

    if request.method == "POST":
        action = request.form.get("action")
        print(f"[INFO] Button pressed: {action}")

        if action == "get_state":
            reqCreate = [0x00, 0x03, 0x22, 0xF1, 0x11] 
            send_data(reqCreate)
            wait_response()

        elif action == "set_state":
            evm_state = request.form.get("evm_state", evm_state)
            reqCreate = [0x00, 0x04, 0x2E, 0xF1, 0x11] 
            if(evm_state == "MODE_IDLE"):
                reqCreate.append(0x00)
            elif(evm_state == "MODE_CONFIG_INIT"):
                reqCreate.append(0x01)
            elif(evm_state == "MODE_CONFIG"):
                reqCreate.append(0x02)
            elif(evm_state == "MODE_NORMAL"):
                reqCreate.append(0x03)
            elif(evm_state == "MODE_SHUTDOWN"):
                reqCreate.append(0x04)
            else:
                reqCreate.append(0x00)
                
            send_data(reqCreate)
            print(f"[INFO] EVM state set to: {evm_state}")

        elif action == "reset_evm":
            evm_state = "INIT"
            reqCreate = [0x00, 0x04, 0x6E, 0xF1, 0x11, 0x04] 
            send_data(reqCreate)
            wait_response()
            print("[INFO] EVM Reset!")

        elif action == "configure_candidate":
            selected = request.form.get("candidate_select")
            name = request.form.get("candidate_name", "").strip()
            if selected in candidates and name:
                candidates[selected] = name
                print(f"[INFO] Configured {selected} as {name}")

        elif action == "erase_config":
            for key in candidates:
                candidates[key] = ""
            
            reqCreate = [0x00, 0x04, 0x31, 0x01, 0xFF, 0x00] 
            send_data(reqCreate)
            wait_response()
            print("[INFO] All candidates erased.")

        return redirect(url_for('evm'))

    return render_template("evm.html", evm_state=evm_state, candidates=candidates)


if __name__ == "__main__":
    app.run(debug=True)
    
    try:
        # Open the serial port
        ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)
        print(f"[✓] Connected to {COM_PORT} at {BAUD_RATE} baud.\n")

        while True:
            # READ incoming data
            if ser.in_waiting:
                data = ser.readline().decode('utf-8', errors='ignore').strip()
                if data:
                    print(f"[Received] {data}")

            # SEND data
            '''msg = input("Enter message to send (or 'exit' to quit): ")
            if msg.lower() == 'exit':
                break
            ser.write((msg + '\n').encode('utf-8'))'''

    except serial.SerialException as e:
        print(f"[ERROR] Could not open port {COM_PORT}: {e}")
    except KeyboardInterrupt:
        print("\n[INFO] Exiting on user interrupt.")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print(f"[✓] Port {COM_PORT} closed.")


def send_data(request):
    """Send data to the EVM via serial."""
    #byte 0 and byte 1 of request are the length of the request
    len_request = [0, 0]
    len_request[0] = request[0]  # High byte of length
    len_request[1] = request[1]   # Low byte of length

    try:
        if 'ser' in locals() and ser.is_open:
            ser.write(len_request)
            time.sleep(0.01)  # 10 ms = 0.01 seconds
            print(f"[INFO] Data sent: {len_request}")
        else:
            print("[ERROR] Serial port is not open.")
    except Exception as e:
        print(f"[ERROR] Failed to send data: {e}")

    request.pop(0)  # Remove the first byte (length high byte
    request.pop(0)  # Remove the second byte (length low byte)
    
    try:
        if 'ser' in locals() and ser.is_open:
            ser.write(request)
            print(f"[INFO] Data sent: {request}")
        else:
            print("[ERROR] Serial port is not open.")
    except Exception as e:
        print(f"[ERROR] Failed to send data: {e}")

def wait_response():
    """Wait for a response from the EVM."""
    try:
        if 'ser' in locals() and ser.is_open:
            if ser.in_waiting:
                response = ser.readline().decode('utf-8', errors='ignore').strip()
                print(f"[Received] {response}")
            else:
                print("[INFO] No data received.")
        else:
            print("[ERROR] Serial port is not open.")
    except Exception as e:
        print(f"[ERROR] Failed to read response: {e}")