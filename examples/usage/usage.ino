/*
 Example usage for InfluxDB library by Richard Lyon.
 */

// InfluxDB credentials
#define USERNAME "my_username"
#define PASSWORD "my_password"

#include "InfluxDB.h"
InfluxDB idb = InfluxDB(USERNAME, PASSWORD);

int photoresistor = A0;
int power = A5;
double analogvalue;

void setup() {
  Serial.begin(9600);
  pinMode(photoresistor,INPUT);
  pinMode(power,OUTPUT);
  digitalWrite(power,HIGH);

  // initialise InfluxDB
  idb.setDebug(true);             // defaults to false
  idb.setDeviceName("my_device"); // defaults to "particle"
}

void loop() {
  analogvalue = analogRead(photoresistor) * 1.0;
  idb.add("photoresistor", analogvalue);
  idb.add("dummy", 3.1415);
  if (idb.sendAll()) {            // call this to send points to InfluxDB
    Serial.println("InfluxDB updated");
  }
  delay(1000);
}
