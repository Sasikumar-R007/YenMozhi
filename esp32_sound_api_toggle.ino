#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#define SOUND_SENSOR 23   // LM393 D0 connected to GPIO23
#define LED_PIN 2         // Onboard LED (GPIO2)

// ======== WiFi Settings ========
const char* WIFI_SSID     = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// ======== Server / Security ========
const char* TOGGLE_URL    = "https://yenmozhi.vercel.app/api/toggle"; 
const char* TOGGLE_SECRET = "YOUR_LONG_RANDOM_SECRET"; // Same as Vercel env

bool ledState = false;
unsigned long lastTrigger = 0;
const unsigned long debounceDelay = 1000; // 1 sec gap

void setup() {
  pinMode(SOUND_SENSOR, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  int soundDetected = digitalRead(SOUND_SENSOR);

  if (soundDetected == LOW) { // LOW means sound detected
    unsigned long now = millis();
    if (now - lastTrigger > debounceDelay) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);

      Serial.println("Sound Detected! Sending toggle request...");
      sendToggleRequest();
      lastTrigger = now;
    }
  }
}

// ======== Function to send HTTPS POST ========
void sendToggleRequest() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure(); // Skip certificate check

  HTTPClient http;
  if (!http.begin(client, TOGGLE_URL)) {
    Serial.println("Unable to connect to server");
    return;
  }

  // Add headers
  http.addHeader("Authorization", String("Bearer ") + TOGGLE_SECRET);
  http.addHeader("Content-Type", "application/json");

  // Send minimal payload
  String payload = "{\"event\":\"sound_detected\"}";

  int httpResponseCode = http.POST(payload);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Server Response: " + response);
  } else {
    Serial.println("Error sending request, code: " + String(httpResponseCode));
  }

  http.end();
}
