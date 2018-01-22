# InfluxDB

A Particle library for InfluxDB.

## About InfluxDB Particle library

InfluxDB is a time series database built to handle high write and query loads. It's ideal as
a backing store for timestamped IoT sensor data. More information can be found [here](https://docs.influxdata.com/influxdb/v1.4/).
InfluxDB Particle library is a simple client for writing Particle data to an InfluxDB
database.  

## Usage

Build the photoresistor [tutorial](https://docs.particle.io/guide/getting-started/examples/photon/#read-your-photoresistor-function-and-variable), create an InfluxDB database called `sensordata`, add the InfluxDB library to your project and follow this simple example:

```
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
```

See the [examples](examples) folder for more details.

## Documentation

Initialise the client with your InfluxDB authentication details:

    InfluxDB idb = InfluxDB(USERNAME, PASSWORD);


InfluxDB supports a built-in HTTP API (described [here](https://docs.influxdata.com/influxdb/v1.4/guides/writing_data/)). InfluxDB uses the Particle HTTPClient library to send a `post` request to the `/write` endpoint.

InfluxDB library writes to InfluxDB database `sensordata` (this will be configurable in future versions) i.e.

    /write?db=sensordata

Points are stored as follows:

    DEVICENAME deviceID=DEVICEID VARIABLEID1=VARIABLEVALUE1(,VARIABLEID2=VARIABLEVALUE2 etc)

`DEVICENAME` is the measurement name in the InfluxDB database. This defaults to `particle` in InfluxDB Library. Change it with:

    idb.setDeviceName("my_device");

`DEVICEID` is the ID of your device as returned by `System.deviceID()`. Each point is tagged in the database with the device ID.

`VARIABLEID` and `VARIABLEVALUE` are the id and value of variables added with the command:

    idb.add("photoresistor", analogvalue);

Up to `MAX_VALUES` (currently, 10) can be added between updates to the database. Values added beyond this limit are ignored. (This is an arbitrary limit pending further investigation of the API limits.)

To write the points to the database i.e. make the API request, call:

    idb.sendAll();

This returns `true` if the HTTP response code 204 (successful write operation) is received, `false` otherwise.

Put the device in debug mode with:

    idb.setDebug(true);

This will print to the serial line the data being sent to InfluxDB.

The library has been tested on a Particle Photon (System Firmware 0.6.3) and Core (unknown firmware version but recently updated).

## Changelog

### 0.1.0
Initial release.

## Contributing

Here's how you can make changes to this library and eventually contribute those changes back.

To get started, [clone the library from GitHub to your local machine](https://help.github.com/articles/cloning-a-repository/).

Change the name of the library in `library.properties` to something different. You can add your name at then end.

Modify the sources in <src> and <examples> with the new behavior.

To compile an example, use `particle compile examples/usage` command in [Particle CLI](https://docs.particle.io/guide/tools-and-features/cli#update-your-device-remotely) or use our [Desktop IDE](https://docs.particle.io/guide/tools-and-features/dev/#compiling-code).

After your changes are done you can upload them with `particle library upload` or `Upload` command in the IDE. This will create a private (only visible by you) library that you can use in other projects. Do `particle library add InfluxDB_myname` to add the library to a project on your machine or add the InfluxDB_myname library to a project on the Web IDE or Desktop IDE.

At this point, you can create a [GitHub pull request](https://help.github.com/articles/about-pull-requests/) with your changes to the original library.

If you wish to make your library public, use `particle library publish` or `Publish` command.

## LICENSE
Copyright 2018 Richard Lyon

Licensed under the GNU General Public license
