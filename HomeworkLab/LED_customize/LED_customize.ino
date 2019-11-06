
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

int sensorValueRed;
int sensorValueGreen;
int sensorValueBlue;

int voltageRed;
int voltageGreen;
int voltageBlue;

void setup() {
  // put your setup code here, to run once:

  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

//  analogWrite(redPin, 255);
//  analogWrite(greenPin, 255);
//  analogWrite(bluePin, 255);
  
  sensorValueRed = analogRead(A0);
  sensorValueGreen = analogRead(A1);
  sensorValueBlue = analogRead(A2); 

  voltageRed = sensorValueRed * (5.0 / 1023.0);
  voltageGreen = sensorValueGreen * (5.0 / 1023.0);
  voltageBlue = sensorValueBlue * (5.0 / 1023.0);

  analogWrite(redPin, ( (voltageRed * 255.0) / 5.0) );
  analogWrite(greenPin,( (voltageGreen * 255.0) / 5.0) );
  analogWrite(bluePin,( (voltageBlue * 255.0) / 5.0) );
  
}
