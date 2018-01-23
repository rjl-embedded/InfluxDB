#pragma once

/*
 InfluxDB library
 Copyright 2018 Richard Lyon
 Licensed under the GNU General Public license
 */

#ifndef _INFLUXDB_
#define _INFLUXDB_
#endif
#ifndef VERSION
#define VERSION 0.1.0
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
#ifndef DATABASE
#define DATABASE "sensordata"
#endif

#include "Particle.h"
#include "HttpClient.h"

struct Value {
    char  *idName;
    float idValue;
};

class InfluxDB
{
public:
  InfluxDB(char* username, char* password);
  void add(char *variable_id, double value);
  bool sendAll();
  void printResponse(http_response_t &response);
  void setDeviceName(char* deviceName);
  String setDatabase(String databaseName);
  void setDebug(bool debug);
private:
  char* _username;
  char* _password;
  String _deviceID;
  String _deviceName;
  String _databaseName;
  Value* pvalue;
  uint8_t _currentValue;
  HttpClient http;
  http_request_t request;
  http_response_t response;
  bool _debug = false;
};
