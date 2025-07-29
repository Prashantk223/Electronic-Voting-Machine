from flask import Flask, render_template, request, redirect, url_for

# ✅ Create app first
app = Flask(__name__)

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
            pass  # nothing to do

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

        return redirect(url_for('evm'))

    return render_template("evm.html", evm_state=evm_state, candidates=candidates)


if __name__ == "__main__":
    app.run(debug=True)
