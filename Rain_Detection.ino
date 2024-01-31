
int rainAnalogPin = A0;
int rainDigitalPin = 2;
int redLED = 3;
int alarm = 5; //PWM (~) Pin

void setup(){
  pinMode(rainAnalogPin, INPUT);
  pinMode(rainDigitalPin, INPUT);
  pinMode(alarm, OUTPUT);
  pinMode(redLED, OUTPUT);
  digitalWrite(alarm, LOW);
  digitalWrite(redLED, LOW);
  Serial.begin(9600);
}

void loop() {
  // read the input on analog pin 0:
  int sensorAnalogValue = analogRead(rainAnalogPin);
  Serial.print("Rain sensor analog value: ");
  Serial.println(sensorAnalogValue);
  
  // read the input on digital pin 2:
  int sensorDigitalValue = digitalRead(rainDigitalPin);
  Serial.print("Rain sensor digital value: ");
  Serial.println(sensorDigitalValue);
  
//digital output
 if(sensorAnalogValue <=500) //High means rain detected; In some case vise versa
 {
  digitalWrite(redLED, HIGH);
  tone(alarm, 5000); //the buzzer sound frequency at 5000 Hz
  Serial.println("Rain Detected");
 }
 else {
  digitalWrite(redLED, LOW);
  noTone(alarm);
  Serial.println("No Rain Detected");
 }
 delay(250);
}
