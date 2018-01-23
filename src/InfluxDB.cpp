/* InfluxDB library
   Copyright 2018 Richard Lyon
   Licensed under the GNU General Public license
 */

 #include "HttpClient.h"
 #include "InfluxDB.h"

 InfluxDB::InfluxDB(char* username, char* password)
 {
   _username = username;
   _password = password;
   _databaseName = setDatabase(DATABASE);
   _deviceID = System.deviceID();
   _deviceName = String("particle");
   _databaseName = "sensordata";
   request.port = PORT;  // influxdb port
   request.ip = IP; // DigitalOcean
   request.path = String::format("/write?db=%s&precision=s&u=%s&p=%s",_databaseName.c_str(),username,password);
   pvalue = (Value *)malloc(MAX_VALUES * sizeof(Value));
   _currentValue = 0;
 }

 void InfluxDB::add(char *variable_id, double value)
 {
   return add(variable_id, value, NULL);
 }

 void InfluxDB::add(char *variable_id, double value, unsigned long timestamp)
 {
   (pvalue + _currentValue)->idName = variable_id;
   (pvalue + _currentValue)->idValue = value;

   if (timestamp) {
     // process batch of values
   } else {
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
   String idMeasurement = _deviceName;
   String tag_set = String::format("deviceID=%s", _deviceID.c_str());
   String field_set;
   String requestString;

   // build field set e.g. temperature=21.3,humidity=34.5
   for (int i = 0; i < _currentValue; i++) {
     // e.g. temperature=21.3
     String tempString = String::format("%s=%.1f", (pvalue + i)->idName, (pvalue + i)->idValue);
     if (i>0) {
       field_set.concat(",");
     }
     field_set.concat(tempString);
   }

   requestString = String::format("%s,%s %s", idMeasurement.c_str(), tag_set.c_str(), field_set.c_str());
   // e.g. particle,deviceID=54395594308 temperature=21.3,humidity=34.5
   request.body = requestString;
   request.path = String::format("/write?db=%s&u=%s&p=%s",_databaseName.c_str(),_username,_password);
   http.post(request, response);
   if(_debug) {
     printDebug(request, response);
   }
   _currentValue = 0;
   if (response.status == 204) {
     return true;
   } else {
     Particle.publish("ERROR", response.body, PRIVATE);
     return false;
   }
   return (response.status == 204) ? true : false;
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
