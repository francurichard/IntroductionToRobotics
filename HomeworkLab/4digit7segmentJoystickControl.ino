// declare all the joystick pins
const int pinSW = A2;
const int pinX = A0;
const int pinY = A1;

// declare all the 4 digit 7-segment display pins
const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int segSize = 8;

const int noOfDisplays = 4;
const int noOfDigits = 10;

int current_display = 0; // 0..3

unsigned long lastBlink = 0;
int blinkState = LOW;

//states of the button press
int swState = LOW;
int lastSwState = LOW;

int xValue = 0;
int yValue = 0;

bool joyMoved = false;
int digit = 0;
int minThreshold = 400;
int maxThreshold = 600;

bool locked_display[noOfDisplays] = {
  false, false, false, false
}; 

int display_value[noOfDisplays] = {
  0, 0, 0, 0
};
// segments array
int segments[segSize] = {
pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

// digits array, to switch between them easily
int digits[noOfDisplays] = {
pinD1, pinD2, pinD3, pinD4
};

byte digitMatrix[noOfDigits][segSize - 1] = {
// a b c d e f g
{1, 1, 1, 1, 1, 1, 0}, // 0
{0, 1, 1, 0, 0, 0, 0}, // 1
{1, 1, 0, 1, 1, 0, 1}, // 2
{1, 1, 1, 1, 0, 0, 1}, // 3
{0, 1, 1, 0, 0, 1, 1}, // 4
{1, 0, 1, 1, 0, 1, 1}, // 5
{1, 0, 1, 1, 1, 1, 1}, // 6
{1, 1, 1, 0, 0, 0, 0}, // 7
{1, 1, 1, 1, 1, 1, 1}, // 8
{1, 1, 1, 1, 0, 1, 1} // 9
};

void displayNumber(byte digit, int num) {
  
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }

  // set decimal point state
  if (num == current_display && locked_display[num] == true) {
    if (millis() - lastBlink > 200) {
      blinkState = (blinkState == LOW) ? HIGH:LOW;
      lastBlink = millis();
    }
    digitalWrite(segments[segSize - 1], blinkState);
  } else if (num == current_display && locked_display[num] == false) {
    digitalWrite(segments[segSize - 1], HIGH);
  } else {
    digitalWrite(segments[segSize - 1], LOW);
  }
  

}

void showDigit(int num) {
  for (int i = 0; i < noOfDisplays; i++) {
    digitalWrite(digits[i], HIGH);
  }

  digitalWrite(digits[num], LOW);
}

void setup() {
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }

  for (int i = 0; i < noOfDisplays; i++) {
    pinMode(digits[i], OUTPUT);
  }

  pinMode(pinSW, INPUT_PULLUP);

  current_display = 0;

  Serial.begin(9600);
}

void loop() {

    Serial.println(current_display);
    Serial.println(display_value[current_display]);
  xValue = analogRead(pinX);

  if (xValue < minThreshold && joyMoved == false) {
    if (locked_display[current_display] == false){

      if (current_display == 0) {
        current_display = 3;
      } else {
        current_display--;  
      }
    }

    joyMoved = true;
  }

  if (xValue > maxThreshold && joyMoved == false) {
    if (locked_display[current_display] == false) {
      if (current_display == 3) {
        current_display = 0;
      } else {
        current_display++;
      }
    }


    joyMoved = true;
  }

  yValue = analogRead(pinY);

  if (yValue < minThreshold && joyMoved == false) {
    if(locked_display[current_display] == true) {

      if (display_value[current_display] == 0) {
        display_value[current_display] = 9;
      } else {
        display_value[current_display]--;
      }
    }

    Serial.println("moved od oy");

    joyMoved = true;
  }

  if (yValue > maxThreshold && joyMoved == false) {
    if(locked_display[current_display] == true) {

      if (display_value[current_display] == 9) {
        display_value[current_display] = 0;
      } else {
        display_value[current_display]++;
      }

      
    }
    Serial.println("moved od oy");
    joyMoved = true;
  }

  if (yValue <= maxThreshold && yValue >= minThreshold && xValue <= maxThreshold && xValue >= minThreshold) {
    joyMoved = false;
  }

  swState = digitalRead(pinSW);

  if (swState != lastSwState) {
    if (swState == LOW) {
      locked_display[current_display] = !locked_display[current_display];
      Serial.println("Presses");
    }

  }
  lastSwState = swState;

  for (int i = 0; i < noOfDisplays; i++) {
    showDigit(i);
    displayNumber(display_value[i], i);
    delay(5);
  }
}
