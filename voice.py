from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from webdriver_manager.chrome import ChromeDriverManager
import serial
import time

# ---- Arduino Serial Setup ----
arduino_port = 'COM5'  # Change to your Arduino port
baud_rate = 9600
ser = serial.Serial(arduino_port, baud_rate, timeout=1)
time.sleep(2)  # allow Arduino to reset

# ---- Chrome Setup ----
url = "https://voice-reg-modal.vercel.app"
chrome_options = Options()
chrome_options.add_argument("--use-fake-ui-for-media-stream")  # auto-allow mic
chrome_options.add_argument("--log-level=3")                     # suppress logs
chrome_options.add_experimental_option("excludeSwitches", ["enable-logging"])
# chrome_options.add_argument("--headless=new")                 # optional: headless mode

website_open = False
driver = None

print("Listening for sound from Arduino...")

try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            if line == "Sound Detected!":
                if not website_open:
                    print("Opening website and starting mic...")
                    driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()), options=chrome_options)
                    driver.get(url)
                    # Mic auto-start is already handled in website JS, but ensure:
                    driver.execute_script("startListening();")
                    website_open = True
                else:
                    print("Stopping mic and closing website...")
                    if driver:
                        driver.execute_script("stopListening();")  # stop mic
                        driver.quit()
                        driver = None
                    website_open = False
                time.sleep(1)  # debounce to avoid double triggers

except KeyboardInterrupt:
    print("Program stopped by user.")

finally:
    if ser.is_open:
        ser.close()
    if driver:
        driver.quit()
