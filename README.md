## Solace MQTT IoT Device SDK for the L-Spark Secure IoT Accelerator

The Secure IoT Accelerator program is intended to enable Canadian Internet of Things (IoT) ventures by providing them with global market reach capability with a Secure by Design technology.

For more information about the accelerator program, visit: https://www.l-spark.com/press-release/l-spark-telus-blackberry-and-solace-launch-accelerator-for-emerging-canadian-iot-ventures/

Youtube: https://www.youtube.com/watch?v=ZL2ADLHl-XQ&feature=youtu.be

The Solace MQTT IoT SDK provides secure MQTT connectivity on the IoT Accelerator Device Platform. It includes an example on how to utilise the Telus SIM Secure Storage element to establish a TLS connection to Solace Cloud.

It is based on [aws-iot Embedded C Device SDK](https://github.com/aws/aws-iot-device-sdk-embedded-C/) and includes the following modifications:

* Removed AWS IoT Hub specific features - Device Shadow, Jobs
* Removed samples, tests
* Added mbedTLS source code, v [2.17](https://github.com/ARMmbed/mbedtls/tree/mbedtls-2.17)
* Added support to pass in Root CA,Device Certificate and Device Key to the library rather than reading from files
* Added example of how to access the Telus Secure Storage to read certificates and keys and pass these into the MQTT client library to establish a TLS connection

Changes have been added to the master branch of this repository.

Find a getting started tutorial below, the original release branch [readme.md](https://github.com/aws/aws-iot-device-sdk-embedded-C/blob/release/README.md) applies. 

## Overview
* Prerequisites
  * Set up the hardware and modem connectivity 
  * Obtain the Telus Modem / Secure Storage API (compiled versio may be delivered with BlackBerry OS,  
  * Optional - Apply BlackBerry security patch to Raspbian
  * Optional - provision certificates and keys onto the secure storage
* Set up a development environment:
  * Develop direct on Raspberry PI / device (not documented in this readme)
  * Eclipse on MacOs or Windows
* SDK and sample applicaiton installation
* Solace Cloud Access and Connectivity
* Run sample application

## Prerequisites
### Set up the hardware and modem connectivity
See this [tutorial](https://github.com/TELUS-Emerging-IoT/TELUS-Devkit-Hardware-Tutorial/blob/master/README.md)

### Obtain Telus Modem / Secure Storage API
TBC - may be packaged with Operating System. If building from source will supply C14/C++14 compatible source code on request.
## Development directly on Device
Source code is provided as is with a sample make file. No further instrucitons are available at this point.

## Set up Eclipse Development Environment
### Install Eclipe for C/CPP
Windows or Mac
### Obtain and install Raspberry Pi Tool Chain
Links to MacOS and Windows based tool chains
### Set up remote debugging using GDB

### Import required libraries form Raspberry PI
Currently only Wiring PI required.
TBC - Telus API if provided with OS

## SDK and Sample Installation
This section explains the individual steps to retrieve the necessary files and be able to build your first application using the AWS IoT device SDK for embedded C.

Steps:

 * Create a directory to hold your application e.g. (/home/<user>/aws_iot/my_app)
 * Change directory to this new directory
 * Download the SDK to device and place in the newly created directory
 * Expand the tarball (tar -xf <tarball.tar>).  This will create the below directories:
    * `certs` - TLS certificates directory
    * `docs` - SDK API and file documentation. This folder is not present on GitHub. You can access the documentation [here](http://aws-iot-device-sdk-embedded-c-docs.s3-website-us-east-1.amazonaws.com/index.html)
    * `external_libs` - The mbedTLS and jsmn source files
    * `include` - The AWS IoT SDK header files
    * `platform` - Platform specific files for timer, TLS and threading layers
    * `src` - The AWS IoT SDK source files

## Solace Cloud Access 7 Connectivity

## Running the sample application


## Resources
[API Documentation](http://aws-iot-device-sdk-embedded-c-docs.s3-website-us-east-1.amazonaws.com/index.html)

[MQTT 3.1.1 Spec](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/csprd02/mqtt-v3.1.1-csprd02.html)
