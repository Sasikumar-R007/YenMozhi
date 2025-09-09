#define SOUND_SENSOR 23
#define LED_PIN 2

bool ledState = false;
unsigned long lastTrigger = 0;
const unsigned long debounceDelay = 1000; // 1 second minimum gap

void setup() {
  pinMode(SOUND_SENSOR, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
}

void loop() {
  int soundDetected = digitalRead(SOUND_SENSOR);

  if (soundDetected == LOW) {
    unsigned long now = millis();

    // Only trigger if 1 second passed since last detection
    if (now - lastTrigger > debounceDelay) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      Serial.println("Sound Detected!");
      lastTrigger = now;
    }
  }
}
