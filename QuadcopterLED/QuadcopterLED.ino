#include <Adafruit_NeoPixel.h>

#define ELEVATOR_PIN 4
#define AILERON_PIN 5
#define FORWARD_PIN 6
#define BACKWARD_PIN 7
#define LEFT_PIN 8
#define RIGHT_PIN 9

#define MAX_SIGNAL_MS 1900
#define CENTER_SIGNAL_MS 1500
#define MIN_SIGNAL_MS 1100
#define LED_COUNT 16


Adafruit_NeoPixel forwardStrip = Adafruit_NeoPixel(LED_COUNT, FORWARD_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel backwardStrip = Adafruit_NeoPixel(LED_COUNT, BACKWARD_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leftStrip = Adafruit_NeoPixel(LED_COUNT, LEFT_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rightStrip = Adafruit_NeoPixel(LED_COUNT, RIGHT_PIN, NEO_GRB + NEO_KHZ800);

uint16_t aileron; 
uint16_t elevator;

uint16_t lastAileron = 0;
uint16_t lastElevator = 0;
uint16_t lastForward = 0;
uint16_t lastBackward = 0;
uint16_t lastLeft = 0;
uint16_t lastRight = 0;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  
  pinMode(ELEVATOR_PIN, INPUT); // Set our input pins as such
  pinMode(AILERON_PIN, INPUT);

  Serial.begin(9600); // Pour a bowl of Serial



  forwardStrip.begin();
  forwardStrip.show(); // Initialize all pixels to 'off'
  backwardStrip.begin();
  backwardStrip.show();
  leftStrip.begin();
  leftStrip.show();
  rightStrip.begin();
  rightStrip.show();
}

void loop() {
  
    aileron = pulseIn(AILERON_PIN, HIGH, 2200); // each channel
    if(aileron != 0){
      if(aileron != lastAileron){
        if(aileron >= CENTER_SIGNAL_MS){
          uint16_t delta = aileron - CENTER_SIGNAL_MS;
          int left = map(delta, 0, MAX_SIGNAL_MS - CENTER_SIGNAL_MS, 0, LED_COUNT);
          leftStripMagnitude(left, leftStrip.Color(0, 0, 255), 10); 
          lastLeft = left;
          if(lastRight != 0){
            lastRight = 0;
            rightStripMagnitude(0, rightStrip.Color(0, 0, 0), 10);
          }
        } else {
          uint16_t delta = CENTER_SIGNAL_MS - aileron;
          int right = map(delta, 0, MAX_SIGNAL_MS - CENTER_SIGNAL_MS, 0, LED_COUNT);
          rightStripMagnitude(right, rightStrip.Color(255, 0, 255), 10);
          lastRight = right;
          if(lastLeft != 0){
            lastLeft = 0;
            leftStripMagnitude(0, leftStrip.Color(0, 0, 0), 10); 
          }
        } 
        Serial.print("Aileron: "); 
        Serial.println(aileron);        
        lastAileron = aileron;
      }
    }

//    delay(1);


    elevator = pulseIn(ELEVATOR_PIN, HIGH, 2200); // Read the pulse width of 
    if(elevator != 0){
      if(elevator != lastElevator){
        if(elevator >= CENTER_SIGNAL_MS){
          uint16_t delta = elevator - CENTER_SIGNAL_MS;
          int forward = map(delta, 0, MAX_SIGNAL_MS - CENTER_SIGNAL_MS, 0, LED_COUNT);
          forwardStripMagnitude(forward, forwardStrip.Color(255, 0, 0), 10); 
          lastForward = forward;
          if(lastBackward != 0){
            lastBackward = 0;
            backwardStripMagnitude(0, forwardStrip.Color(0, 0, 0), 10);
          }
        } else {
          uint16_t delta = CENTER_SIGNAL_MS - elevator;
          int backward = map(delta, 0, MAX_SIGNAL_MS - CENTER_SIGNAL_MS, 0, LED_COUNT);
          backwardStripMagnitude(backward, forwardStrip.Color(0, 255, 0), 10);
          lastBackward = backward;
          if(lastForward != 0){
            lastForward = 0;
            forwardStripMagnitude(0, forwardStrip.Color(0, 0, 0), 10); 
          }
        }
        Serial.print("Elevator: "); 
        Serial.println(elevator);        
        lastElevator = elevator;
      }
    }



}


void forwardStripMagnitude(uint32_t m, uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<forwardStrip.numPixels(); i++){
    forwardStrip.setPixelColor(i, forwardStrip.Color(0, 0, 0));
  }

  for(uint16_t i=0; i<m; i++) {
    forwardStrip.setPixelColor(i, c);
  }
  forwardStrip.show();
}

void backwardStripMagnitude(uint32_t m, uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<backwardStrip.numPixels(); i++){
    backwardStrip.setPixelColor(i, backwardStrip.Color(0, 0, 0));
  }

  for(uint16_t i=0; i<m; i++) {
    backwardStrip.setPixelColor(i, c);
  }
  backwardStrip.show();
}
void leftStripMagnitude(uint32_t m, uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<leftStrip.numPixels(); i++){
    leftStrip.setPixelColor(i, leftStrip.Color(0, 0, 0));
  }

  for(uint16_t i=0; i<m; i++) {
    leftStrip.setPixelColor(i, c);
  }
  leftStrip.show();
}
void rightStripMagnitude(uint32_t m, uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<rightStrip.numPixels(); i++){
    rightStrip.setPixelColor(i, rightStrip.Color(0, 0, 0));
  }

  for(uint16_t i=0; i<m; i++) {
    rightStrip.setPixelColor(i, c);
  }
  rightStrip.show();
}

