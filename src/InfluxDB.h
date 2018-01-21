#pragma once

/* InfluxDB library by Richard Lyon
 */

#ifndef _INFLUXDB_
#define _INFLUXDB_
#endif
#ifndef IP
#define IP IPAddress(139,59,166,111)
#endif
#ifndef PORT
#define PORT 8086
#endif
#ifndef MAX_VALUES
#define MAX_VALUES 10
#endif

#include "Particle.h"
#include "authentication.h"
#include "HttpClient.h"

struct Value {
    char  *idName;
    float idValue;
};

class InfluxDB
{
public:
  Influxdb(char* username, char* password);
  void add(char *variable_id, double value);
  bool sendAll();
  void printResponse(http_response_t &response);
  void setDeviceName(char* deviceName);
  void setDebug(bool debug);
private:
  String _dsID;
  String _dsName;
  String _databaseID;
  Value * pvalue;
  uint8_t _currentValue;
  HttpClient http;
  http_request_t request;
  http_response_t response;
  bool _debug = false;
};
