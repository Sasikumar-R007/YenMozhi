from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from webdriver_manager.chrome import ChromeDriverManager
import serial
import time

# ---- ESP32 Serial Setup ----
esp_port = 'COM10'  # Change this to your ESP32 COM port
baud_rate = 9600
ser = serial.Serial(esp_port, baud_rate, timeout=1)
time.sleep(2)  # allow ESP32 to reset

# ---- Chrome Setup ----
url = "https://yenmozhi.vercel.app/"
chrome_options = Options()
chrome_options.add_argument("--use-fake-ui-for-media-stream")  # auto-allow mic
chrome_options.add_argument("--log-level=3")                     
chrome_options.add_experimental_option("excludeSwitches", ["enable-logging"])
# chrome_options.add_argument("--headless=new")   # optional: run headless

website_open = False
driver = None

print("Listening for sound from ESP32...")

try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if line == "Sound Detected!":
                if not website_open:
                    print("Opening website and starting mic...")
                    driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()), options=chrome_options)
                    driver.get(url)
                    driver.execute_script("startListening();")
                    website_open = True
                else:
                    print("Stopping mic and closing website...")
                    if driver:
                        driver.execute_script("stopListening();")
                        driver.quit()
                        driver = None
                    website_open = False
                time.sleep(1)  # debounce

except KeyboardInterrupt:
    print("Program stopped by user.")

finally:
    if ser.is_open:
        ser.close()
    if driver:
        driver.quit()
