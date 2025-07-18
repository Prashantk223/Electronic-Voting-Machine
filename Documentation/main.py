from flask import Flask, render_template
 
app = Flask(__name__)

@app.route("/")
def home():
    return render_template("index.html")

@app.route("/create")
def create():
    return render_template("create.html")

@app.route("/gallery")
def gallery():
    return render_template("gallery.html")



def get_evm_status():
    response = requests.get("http://java-backend-host/api/evm/status")
    return response.json()

@app.route("/reset_evm", methods=["POST"])
def reset_evm():
    # Add your EVM reset logic here
    # For example, call your Java backend or perform the reset
    # Simulate success response
    return jsonify({"status": "success", "message": "EVM Reset!"})

app.run(debug=True)