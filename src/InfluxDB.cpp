/* InfluxDB library
   Copyright 2018 Richard Lyon
   Licensed under the GNU General Public license
 */

 #include "HttpClient.h"
 #include "InfluxDB.h"

 InfluxDB::InfluxDB(const char* username, const char* password)
 {
   _username = username;
   _password = password;
   _databaseName = setDatabase(DATABASE);
   _deviceID = System.deviceID();
   _deviceName = String("particle");
   _databaseName = "sensordata";
   _currentValue = 0;
   request.port = PORT;  // influxdb port
   request.ip = IP; // DigitalOcean
   request.path = String::format("/write?db=%s&precision=s&u=%s&p=%s",_databaseName.c_str(),username,password);
   pvalue = (Value *)malloc(MAX_VALUES * sizeof(Value));
 }

 void InfluxDB::add(const char *variable_id, double value)
 {
   return add(variable_id, value, 0UL);
 }

 void InfluxDB::add(const char *variable_id, double value, unsigned long timestamp)
 {
   (pvalue + _currentValue)->idName = variable_id;
   (pvalue + _currentValue)->idValue = value;

   if (timestamp) {
     // process batch of values
     (pvalue + _currentValue)->timestamp_val = timestamp;
   } else {
     // process single value
     (pvalue + _currentValue)->timestamp_val = Time.now();
   }

   _currentValue++;
   if (_currentValue > MAX_VALUES) {
     Serial.println("You are sending more than the maximum of consecutive variables");
     _currentValue = MAX_VALUES;
   }
 }

 bool InfluxDB::sendAll()
 {
   unsigned long currentTimestamp;
   String idMeasurement = _deviceName; // e.g. particle
   String tag_set = String::format("deviceID=%s", _deviceID.c_str()); // e.g. deviceID=54395594308

   for (int i=0; i<_currentValue; i++) {
     String field_set = makeFieldSet(pvalue + i); // e.g. temperature=21.3
     currentTimestamp = (pvalue+i)->timestamp_val;
     while ( (i+1) < _currentValue && currentTimestamp == (pvalue+i+1)->timestamp_val) { // batch
       i++;
       field_set.concat(",");
       field_set.concat(makeFieldSet(pvalue+i));
       currentTimestamp = (pvalue+i)->timestamp_val;
     }

     // e.g. particle,deviceID=54395594308 temperature=21.3,humidity=34.5 435234783725
     String requestString = String::format("%s,%s %s %d", idMeasurement.c_str(), tag_set.c_str(), field_set.c_str(), currentTimestamp);

     // send it
     request.path = String::format("/write?db=%s&precision=s&u=%s&p=%s",_databaseName.c_str(),_username,_password);
     request.body = requestString;
     http.post(request, response);

   }
   _currentValue = 0;
   if (response.status == 204) {
     return true;
   } else {
     printDebug(request, response);
     Particle.publish("ERROR", response.body, PRIVATE);
     return false;
   }
 }

 String InfluxDB::makeFieldSet(Value * pvalue) {
   return String::format("%s=%.1f", (pvalue)->idName, (pvalue)->idValue); // e.g. temperature=21.3
 }

 void InfluxDB::printDebug(http_request_t &request, http_response_t &response)
 {
   Serial.println(request.path);
   Serial.println(request.body);
   Serial.print("HTTP Response: ");
   Serial.println(response.status);
   Serial.println(response.body);
 }

 String InfluxDB::setDeviceName(String deviceName)
 {
   _deviceName = deviceName;
   return deviceName;
 }

 String InfluxDB::setDatabase(String databaseName)
 {
   // TODO: add check that database exists or create
   _databaseName = databaseName;
   return databaseName;
 }

 void InfluxDB::setDebug(bool debug) {
     _debug = debug;
 }
