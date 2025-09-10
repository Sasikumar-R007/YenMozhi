from flask import Flask, request, jsonify
import os
import subprocess

app = Flask(__name__)

# Track current state
current_state = {
    "browser": "off",
    "mic": "off"
}

# Change this to your actual site
SITE_URL = "http://yenmozhi.vercel.app"

@app.route("/open_site")
def open_site():
    global current_state
    action = request.args.get("action", "open")

    if action == "open":
        # Open Edge only if not already open
        if current_state["browser"] == "off":
            subprocess.Popen(["start", "msedge", SITE_URL], shell=True)
            current_state["browser"] = "on"
            current_state["mic"] = "on"
            return "✅ Website opened + Mic ON"
        else:
            return "⚠️ Website already open"

    elif action == "close":
        # Kill all Edge processes
        os.system("taskkill /im msedge.exe /f >nul 2>&1")
        current_state["browser"] = "off"
        current_state["mic"] = "off"
        return "🛑 Website closed + Mic OFF"

    return "⚠️ Invalid action"


@app.route("/api/state")
def api_state():
    return jsonify({"micState": current_state["mic"]})


if __name__ == "__main__":
    print("✅ Flask server running at http://0.0.0.0:8000")
    app.run(host="0.0.0.0", port=8000)
