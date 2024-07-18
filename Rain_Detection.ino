#include <SoftwareSerial.h>

String _buffer;
String number = "+918482878905";

#define Rain_SENSOR A1

int rain_value = 0;

int rain_default = 100;
//rain_flag = 0 means rain not detected
//rain_flag = 1 means rain is detected
boolean rain_flag = 0;

SoftwareSerial mySerial(2, 3);

#define BUZZER_PIN 9

void setup()
{
  mySerial.begin(9600);
  //Begin serial communication: SIM800L
  Serial.begin(9600);
  //Begin serial communication: Arduino IDE (Serial Monitor)

  pinMode(Rain_SENSOR, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  //by default the BUZZER is OFF
  digitalWrite(BUZZER_PIN, LOW); 
  Serial.println("Initializing...");
  //Once the handshake test is successful, it will back to OK
  mySerial.println("AT");
  delay(1000);
  mySerial.println("AT+CMGF=1");
  delay(1000);

}

void loop()
{
  while (mySerial.available()) {
    Serial.println(mySerial.readString());
  }

  rain_value = analogRead(Rain_SENSOR);
  rain_value = map(rain_value, 0, 1023, 225, 0);
  Serial.println(rain_value);
  //The rain is detected, trigger Alarm and send sms
  if (rain_value >= rain_default) {
    digitalWrite(BUZZER_PIN, HIGH);
    rain_flag = 1;
    if (rain_flag == 1) {
      Serial.println("Rain is Detected.");
      send_sms();
      make_call();
      rain_flag = 0;

    }
  }
  //No rain is detected, turn OFF Alarm
  else {
    digitalWrite(BUZZER_PIN, LOW);
    if (rain_flag == 0) {
      Serial.println("Rain is not Detected.");
    }
  }

}
void make_call() {
  mySerial.print (F("ATD"));
  mySerial.print (number);
  mySerial.print (F(";\r\n"));
  //_buffer = _readSerial();
  Serial.println(_buffer);
}

void send_sms()
{
  Serial.println("sending sms....");
  delay(50);
  mySerial.print("AT+CMGF=1\r");
  delay(1000);
  mySerial.print("AT+CMGS=\"" + number + "\"\r");
  delay(1000);
  mySerial.print("Rain has been detected");
  delay(100);
  mySerial.write(26); //ascii code for ctrl-26 //Serial2.println((char)26); //ascii code for ctrl-26
  delay(5000);
}
