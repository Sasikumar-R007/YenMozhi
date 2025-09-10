from flask import Flask, request
import os

app = Flask(__name__)

@app.route("/open_site")
def open_site():
    action = request.args.get("action")

    if action == "open":
        # Open website in default browser (Windows)
        os.system("start chrome http://yenmozhi.vercel.app")  # Chrome
        # os.system("start msedge http://yenmozhi.vercel.app")  # Edge (use if needed)
        return "Website opened!"

    elif action == "close":
        # Force close Chrome/Edge
        os.system("taskkill /im chrome.exe /f")
        os.system("taskkill /im msedge.exe /f")
        return "Website closed!"

    return "Invalid action!"


if __name__ == "__main__":
    # Run on all interfaces so ESP32 can access
    app.run(host="0.0.0.0", port=8000)
