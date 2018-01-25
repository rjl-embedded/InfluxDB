# InfluxDB Particle library

A Particle library for InfluxDB.

## About InfluxDB Particle library

InfluxDB is a time series database built to handle high write and query loads. It's a good backing store for timestamped IoT sensor data. More information can be found [here](https://docs.influxdata.com/influxdb/v1.4/).
InfluxDB Particle library is a simple client for writing Particle data to an InfluxDB
database.  

## Usage

Create an InfluxDB database called `test`, add the InfluxDB library to your project, and follow this simple example:

```
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
```

See the [examples](examples) folder for more details.

## Documentation

Create file `authenticate.h` with your InfluxDB authentication details as follows:

    #define USERNAME "your_username"
    #define PASSWORD "your_password"

Initialise the client with your InfluxDB authentication details:

    InfluxDB idb = InfluxDB(USERNAME, PASSWORD);


InfluxDB supports a built-in HTTP API (described [here](https://docs.influxdata.com/influxdb/v1.4/guides/writing_data/)). InfluxDB uses the Particle HTTPClient library to send a `post` request to the `/write` endpoint.

By default, InfluxDB library writes to InfluxDB database `sensordata` i.e.

    /write?db=sensordata

Change the target database with:

    idb.setDatabase("test");        // defaults to 'sensordata'

Ensure that `test` exists (future versions will check for you and create the database if necessary).

Points are stored as follows:

    DEVICENAME deviceID=DEVICEID VARIABLEID1=VARIABLEVALUE1(,VARIABLEID2=VARIABLEVALUE2 etc) UNIX_TIMESTAMP

`DEVICENAME` is the measurement name in the InfluxDB database. Future versions will retrieve your device name  from Particle Cloud and use it. It defaults currently to `particle`. For now, change it with:

    idb.setDeviceName("my_device");

`DEVICEID` is the ID of your device as returned by `System.deviceID()`. Each point is tagged in the database with the device ID.

`VARIABLEID` and `VARIABLEVALUE` are the id and value of variables.

`UNIX_TIMESTAMP` is the Unix time of the sample in seconds

Points are buffered prior to sending. There are two ways of adding points to the buffer.

    idb.add("photoresistor", analogvalue);

will cause a single point to be written to the database, with the timestamp corresponding to the time `add()` is called.

    timestamp = Time.now();
    idb.add("batch1", 2.0, timestamp);
    idb.add("batch2", 3.0, timestamp);

will cause all the points added at timestamp to be written to the datanbase in a single request, with the timestamp corresponding to `timestamp`.

Up to `MAX_VALUES` (currently, 10) can be added between updates to the database. Values added beyond this limit are ignored. (This is an arbitrary limit pending further investigation of the API limits.)

To write the point buffer to the database i.e. make the API request, call:

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
