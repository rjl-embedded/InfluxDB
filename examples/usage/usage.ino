/*
 Example usage for InfluxDB library by Richard Lyon.
 */

#include "authenticate.h" // InfluxDB credentials
#include "InfluxDB.h"
InfluxDB idb = InfluxDB(USERNAME, PASSWORD);

long int timestamp;

void setup() {
  Serial.begin(9600);
  // initialise InfluxDB
  idb.setDatabase("test");        // defaults to 'sensordata'
  idb.setDeviceName("elmon");     // defaults to "particle"
  idb.setDebug(true);             // defaults to false
}

int batchcount = 0;
void loop() {
  idb.add("single1", 1.0);
  delay(2000);

  timestamp = Time.now();
  idb.add("batch1", 2.0, timestamp);
  idb.add("batch2", 3.0, timestamp);
  idb.add("batch3", 4.0, timestamp);
  delay(2000);

  idb.add("single2", 5.0);
  delay(2000);

  if (idb.sendAll()) {            // call this to send points to InfluxDB
    // Serial.println("InfluxDB updated");
  }
  delay(1000);
}
