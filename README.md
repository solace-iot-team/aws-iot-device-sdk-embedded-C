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
* SDK and sample application installation
* Solace Cloud Access and Connectivity
* Run sample application

## Prerequisites
### Set up the hardware and modem connectivity
See this [tutorial](https://github.com/TELUS-Emerging-IoT/TELUS-Devkit-Hardware-Tutorial/blob/master/README.md)

### Obtain Telus Modem / Secure Storage API
TBC - may be packaged with Operating System. If building from source will supply C14/C++14 compatible source code on request.

### Optional - Apply BlackBerry security patch to Raspbian
TBC - add link

### Optional - provision certificates and keys onto the secure storage
The repository contains a sample application () that uploads files to the secure storage.
Participants wil be eithe rprovided with a pre-configured SIM or will be supplied with the device certificate, key and Digicert Rooot CA Certificate for Solace Cloud.

## Development directly on Device
Source code is provided as is with a sample make file. No further instrucitons are available at this point.

## Set up Eclipse Development Environment
### Install Eclipse for C/CPP
Download and install Eclipse [IDE for C/C++ Developers](https://www.eclipse.org/downloads/packages/release/2019-03/r/eclipse-ide-cc-developers) for your platform.

### Obtain and install Raspberry Pi Tool Chain
Tool chains for different platforms are available:
* Windows: http://gnutoolchains.com/raspberry/
* MacOS (verified during SDK adaption): https://www.jaredwolff.com/toolchains/#show1
* Linux: https://github.com/raspberrypi/tools/tree/master/arm-bcm2708

Instructions on setting up a cross compile tool chain for RaspBerry PI in Eclipse - (https://dontpressthat.wordpress.com/2017/11/15/using-eclipse-to-write-c-c-programs-for-the-raspberrypi/)


### Set up remote debugging using GDB

Eclipse includes support to create remote debug configurations that execute applications via SSH and GDB on a target system. This requires GDB server to run on the Raspberry PI.
Install gdbserver on Raspberry PI, execute the following commands on the Raspberry PI:

```
$ sudo apt-get install gdbserver

$ gdbserver --multi 192.168.0.131:2345
```

(http://geomodule.com/sw-eng-notes/2017/03/25/raspberry-pi-debugging-with-gdb-command-line/)

### Import required libraries from Raspberry PI

In order to compile code that relies on libraries installe don the Raspberry PI these need to be added to the Tool Chain.
The basic process is explained here: (https://raspberry-projects.com/pi/programming-in-c/compilers-and-ides/eclipse/adding-libraries-to-eclipse-for-windows)

The sample application requires Wiring PI to interact with the shield's user button and LED.
Add the header files and libraries to the toolchain as described above, obtain these from the raspberry PI via SCP:
```
$ scp {user}@{host or ip}:/usr/lib/libwiringPi* {local toolchain lib directory}
$ scp {user}@{host or ip}:/usr/include/wiringPi* {local toolchain include directory}
```

TBC - Download Telus API lib and include if provided with OS.

## SDK and Sample Installation
This section explains the individual steps to retrieve the necessary SDK files and be able to build the sample application.

Steps:

 * Download the released ZIP from ...  
 * Unzip the file to a new directory
 * Import into Eclipse
 * Add the Secure Storage API source code
 
## Solace Cloud Access 7 Connectivity

Login to Solace Cloud
Get Cloud connection parameters
(must be an Enterprise plan currently for mutual TLS authentication)

Verify client username

## Running the sample application

### Setup Connectivity & Security


Set the Solace Cloud service host
Adjust the certificate and key locations if required (provide custom keys for retrieval from secure storage if applicable)
### Create a remote debug configuration

### Run
Start debugger

### Verify MQTT connectivity
* Outbound message from the device
* Inbound message to the device

#### Verify messages sent from device

#### Verify messages are received by device



## Resources
[API Documentation](http://aws-iot-device-sdk-embedded-c-docs.s3-website-us-east-1.amazonaws.com/index.html)

[MQTT 3.1.1 Spec](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/csprd02/mqtt-v3.1.1-csprd02.html)
