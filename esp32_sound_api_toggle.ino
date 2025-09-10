#include <WiFi.h>
#include <HTTPClient.h>

#define SOUND_SENSOR 23   // LM393 D0 connected to GPIO23
#define LED_PIN 2         // Onboard LED (GPIO2)

const char* WIFI_SSID     = "OnePlus Nord CE 3 Lite 5G";
const char* WIFI_PASSWORD = "sasikumar";
const char* TOGGLE_URL    = "http://192.168.1.105:8000/open_site"; 

bool toggleState = false;
unsigned long lastTrigger = 0;
const unsigned long debounceDelay = 2000; // 2 seconds gap (strong clap only)
int lastSoundState = HIGH;

void setup() {
  pinMode(SOUND_SENSOR, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);

  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Connected to WiFi!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int currentState = digitalRead(SOUND_SENSOR);

  // Trigger only when HIGH -> LOW (falling edge = strong clap)
  if (lastSoundState == HIGH && currentState == LOW) {
    unsigned long now = millis();
    if (now - lastTrigger > debounceDelay) {
      toggleState = !toggleState;
      digitalWrite(LED_PIN, toggleState);

      if (toggleState) {
        Serial.println("🔊 Strong Tap → OPEN website...");
        sendToggleRequest("open");
      } else {
        Serial.println("🔊 Strong Tap → CLOSE website...");
        sendToggleRequest("close");
      }
      lastTrigger = now;
    }
  }

  lastSoundState = currentState;
}

void sendToggleRequest(String action) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ WiFi not connected!");
    return;
  }

  HTTPClient http;
  String url = String(TOGGLE_URL) + "?action=" + action;

  if (!http.begin(url)) {
    Serial.println("❌ Unable to connect to server");
    return;
  }

  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("💻 Laptop Response: " + response);
  } else {
    Serial.println("❌ Error sending request, code: " + String(httpResponseCode));
  }

  http.end();
}
