// Example usage for InfluxDB library by Richard Lyon.
/*
 * Remember to create "authentication.h" in src with your InfluxDB credentials
 * as follows:
 *
 * #define USERNAME "my_username"
 * #define PASSWORD "my_password"
 *
 * This will be included by InfluxDB.h - don't add it here.
 */

#include "Influxdb.h"
Influxdb idb = Influxdb(USERNAME, PASSWORD);

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
  if (idb.sendAll()) {            // call this to add points to InfluxDB
    Serial.println("Success");
  }
  delay(1000);
}
