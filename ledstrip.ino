//                     +--\/--+
// RESET (A0/D5) PB5  1|      |8  VCC
//   (A3/D3 PWM) PB3  2|      |7  PB2 (A1/D2)
//   (A2/D4 PWM) PB4  3|      |6  PB1 (D1 PWM)
//               GND  4|      |5  PB0 (D0 PWM)
//                     +------+
int redPin = 0; // the green LED channel
int bluePin = 1;   // the red LED channel
int greenPin = 4;  // the blue LED channel

int brightnessPin = 1;
int fadeDelayPin = 3;
int switchModePin = 5;

int fadeMode = true;
int fadeAmount = 1;    // how many points to fade the LED by
volatile int fadeDelay = 1;
volatile int maxBrightness = 255;

// the setup routine runs once when you press reset
void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  pinMode(brightnessPin, INPUT);
  pinMode(fadeDelayPin, INPUT);
  pinMode(switchModePin, INPUT);

  PWM4_init();
}

void fade(int led, void (*func)(unsigned char,int)) {
  int brightness = 0;
  int fading = fadeAmount;
  while(fadeMode) {
    func(led, brightness);
    brightness += fading;
    if (brightness >= maxBrightness)
      fading = -fadeAmount;
    else if (brightness < 0)
      break;
    //maxBrightness = map(analogRead(brightnessPin), 0, 512, 5, 255);
    //fadeMode = digitalRead(switchModePin) == LOW ? false : true;
    delay(map(analogRead(fadeDelayPin), 0, 512, 1, 30));
  }
}

// the loop routine runs over and over again forever:
void loop() {
  //analogWrite4(4, map(analogRead(3), 0, 1024, 0, 255));
  /*if (digitalRead(switchModePin) == LOW) {
    fadeMode = !fadeMode;
    delay(1000);
  }*/
  
  //maxBrightness = map(analogRead(brightnessPin), 0, 1024, 0, 255);
  //fadeDelay = map(analogRead(fadeDelayPin), 0, 1024, 0, 255);

  if (fadeMode) {
    fade(redPin, analogWrite);
    fade(greenPin, analogWrite4);
    fade(bluePin, analogWrite);
    //digitalWrite(redPin, HIGH);
    //delay(1000);
    //digitalWrite(redPin, LOW);
    //delay(1000);
  } else {
    //analogWrite(redPin, maxBrightness);
    //analogWrite4(greenPin, maxBrightness);
    analogWrite(bluePin, maxBrightness);
    
    maxBrightness = map(analogRead(brightnessPin), 0, 1024, 0, 255);
    //fadeMode = ( == LOW ? true : false);
    //if (analogRead(0) < 100)
      //digitalWrite(bluePin, LOW);
      //digitalWrite(redPin, HIGH);
      //digitalWrite(greenPin, LOW);
    //else
      //digitalWrite(bluePin, LOW);
  }
}

void PWM4_init() {
  // Set up PWM on Trinket GPIO #4 (PB4, pin 3) using Timer 1
  GTCCR = _BV (COM1B1) | _BV (PWM1B);  //  clear OC1B on compare
  OCR1B = 0;                  // duty cycle initialize to 50%
  OCR1C = 255;                  // frequency
}

// Function to allow analogWrite on Trinket GPIO #4 
void analogWrite4(unsigned char pin, int duty_value) {  
  OCR1B = duty_value;  // duty may be 0 to 255 (0 to 100%)
}
