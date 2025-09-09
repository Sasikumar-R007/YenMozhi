#define SOUND_SENSOR 2   // LM393 D0 connected to D2
#define LED_PIN 13       // Onboard LED on Arduino UNO

bool ledState = false;

void setup() {
  pinMode(SOUND_SENSOR, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
}

void loop() {
  int soundDetected = digitalRead(SOUND_SENSOR);

  if (soundDetected == LOW) {  // LM393 D0 goes LOW on clap
    ledState = !ledState;      // toggle LED
    digitalWrite(LED_PIN, ledState);
    Serial.println("Sound Detected!");
    delay(300); // debounce to avoid multiple toggles
  }
}