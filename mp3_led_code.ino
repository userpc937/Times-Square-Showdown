#include <DFRobot_DF1201S.h>
#include <SoftwareSerial.h>
#include <FastLED.h>

// MP3 Module Setup
#define SENSORPIN 4  // Breakbeam sensor pin
SoftwareSerial DF1201SSerial(2, 3);  // RX, TX for MP3 module
DFRobot_DF1201S DF1201S;

// LED Setup
#define DATA_PIN 6
#define NUM_LEDS 10
CRGB leds[NUM_LEDS];

unsigned long lastColorChange = 0;
int colorStep = 0;

void setup() {
  // Serial for debugging
  Serial.begin(115200);

  // Initialize MP3 player
  DF1201SSerial.begin(115200);
  int attempts = 0;
  while (!DF1201S.begin(DF1201SSerial) && attempts < 5) {
    Serial.println("MP3 Init failed, retrying...");
    delay(1000);
    attempts++;
  }
  if (attempts >= 5) {
    Serial.println("MP3 module failed to initialize.");
    return;
  }

  DF1201S.setVol(0);
  DF1201S.switchFunction(DF1201S.MUSIC);
  delay(2000);
  DF1201S.setPlayMode(DF1201S.SINGLE);

  Serial.print("Total MP3 files: ");
  Serial.println(DF1201S.getTotalFile());

  // Breakbeam setup
  pinMode(SENSORPIN, INPUT_PULLUP);

  // LED setup
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  // LED FLASHING
  if (millis() - lastColorChange > 500) {
    CRGB color;
    switch (colorStep % 3) {
      case 0: color = CRGB::Red; break;
      case 1: color = CRGB::Green; break;
      case 2: color = CRGB::Blue; break;
    }
    fill_solid(leds, NUM_LEDS, color);
    FastLED.show();

    colorStep++;
    lastColorChange = millis();
  }

  // BREAKBEAM SENSOR
  int sensorState = digitalRead(SENSORPIN);
  Serial.print("Sensor State: ");
  Serial.println(sensorState);

  if (sensorState == LOW) { 
    if (!DF1201S.isPlaying()) {
      Serial.println("Breakbeam triggered: Playing track 1");
      DF1201S.playFileNum(1);
    }
  } else {
    if (DF1201S.isPlaying()) {
      Serial.println("Beam unbroken: Pausing audio");
      DF1201S.pause();
    }
  }

  delay(50);  
}
