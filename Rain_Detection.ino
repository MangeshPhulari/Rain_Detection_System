#include <SoftwareSerial.h>

String _buffer;
String number = "+918482878905";

#define LIQUID_SENSOR A1

int liquid_value = 0;

int liquid_default = 100;
//liquid_flag = 0 means liquid not detected
//liquid_flag = 1 means liquid is detected
boolean liquid_flag = 0;

SoftwareSerial mySerial(2, 3);

#define BUZZER_PIN 9

void setup()
{
  mySerial.begin(9600);
  //Begin serial communication: SIM800L
  Serial.begin(9600);
  //Begin serial communication: Arduino IDE (Serial Monitor)

  pinMode(LIQUID_SENSOR, INPUT);
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

  liquid_value = analogRead(LIQUID_SENSOR);
  liquid_value = map(liquid_value, 0, 1023, 225, 0);
  Serial.println(liquid_value);
  //The liquid is detected, trigger Alarm and send sms
  if (liquid_value >= liquid_default) {
    digitalWrite(BUZZER_PIN, HIGH);
    liquid_flag = 1;
    if (liquid_flag == 1) {
      Serial.println("Liquid is Detected.");
      send_sms();
      make_call();
      liquid_flag = 0;

    }
  }
  //No liquid is detected, turn OFF Alarm
  else {
    digitalWrite(BUZZER_PIN, LOW);
    if (liquid_flag == 0) {
      Serial.println("Liquid is not Detected.");
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
  mySerial.print("Liquid has been detected");
  delay(100);
  mySerial.write(26); //ascii code for ctrl-26 //Serial2.println((char)26); //ascii code for ctrl-26
  delay(5000);
}
