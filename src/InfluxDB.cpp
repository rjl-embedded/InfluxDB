/* InfluxDB library by Richard Lyon
 */

 #include "HttpClient.h"
 #include "Influxdb.h"

 Influxdb::Influxdb(char* username, char* password)
 {
   _dsID = System.deviceID();
   _dsName = String("particle");
   _databaseID = "sensordata";
   request.port = PORT;  // influxdb port
   request.ip = IP; // DigitalOcean
   request.path = String::format("/write?db=%s&u=%s&p=%s",_databaseID.c_str(),username,password);
   pvalue = (Value *)malloc(MAX_VALUES * sizeof(Value));
   _currentValue = 0;
 }

 void Influxdb::add(char *variable_id, double value)
 {
   (pvalue + _currentValue)->idName = variable_id;
   (pvalue + _currentValue)->idValue = value;
   _currentValue++;
   if (_currentValue > MAX_VALUES) {
     Serial.println("You are sending more than the maximum of consecutive variables");
     _currentValue = MAX_VALUES;
   }
 }

 bool Influxdb::sendAll()
 {
   String idMeasurement = _dsName;
   String tag_set = String::format("deviceID=%s", _dsID.c_str());
   String field_set;
   String requestString;

   for (int i = 0; i < _currentValue; i++) {
     String tempString = String::format("%s=%.1f", (pvalue + i)->idName, (pvalue + i)->idValue);
     if (i>0) {
       field_set.concat(",");
     }
     field_set.concat(tempString);
   }

   requestString = String::format("%s,%s %s", idMeasurement.c_str(), tag_set.c_str(), field_set.c_str());
   request.body = requestString;
   http.post(request, response);
   if(_debug) {
     Serial.println(request.body);
     printResponse(response);
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

 void Influxdb::printResponse(http_response_t &response)
 {
   Serial.print("HTTP Response: ");
   Serial.println(response.status);
   Serial.println(response.body);
 }

 void Influxdb::setDeviceName(char* deviceName)
 {
   _dsName = deviceName;
 }


 /**
  * Turns on or off debug messages
  * @debug is a bool flag to activate or deactivate messages
  */

 void Influxdb::setDebug(bool debug) {
     _debug = debug;
 }
