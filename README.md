## Solace MQTT IoT Device SDK for the L-Spark Secure IoT Accelerator

The Secure IoT Accelerator program is intended to enable Canadian Internet of Things (IoT) ventures by providing them with global market reach capability with a Secure by Design technology.

For more information about the accelerator program, visit: https://www.l-spark.com/press-release/l-spark-telus-blackberry-and-solace-launch-accelerator-for-emerging-canadian-iot-ventures/

Youtube: https://www.youtube.com/watch?v=ZL2ADLHl-XQ&feature=youtu.be

The Solace MQTT IoT SDK provides secure MQTT connectivity on the IoT Accelerator Device Platform. It includes an example on how to utilise the Telus SIM Secure Storage element to establish a TLS connection to Solace Cloud.

It is based on [aws-iot Embedded C Device SDK](https://github.com/aws/aws-iot-device-sdk-embedded-C/) and includes the following modifications:

* Removed AWS IoT Hub specific feature (Device Shadow)
* Added support to pass in Root CA,Device Certificate and Device Key to the library rather than reading from files
* Added example of how to access the Telus Secure Storage to read certificates and keys and pass these into the MQTT client library to establish a TLS connection

Changes have been added to the master branch of this repository.

Find a getting started tutorial below, the original release branch [readme.md](https://github.com/aws/aws-iot-device-sdk-embedded-C/blob/release/README.md) applies. 





## Installation
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
    * `samples` - The sample applications
    * `src` - The AWS IoT SDK source files
    * `tests` - Contains tests for verifying that the SDK is functioning as expected. More information can be found [here](https://github.com/aws/aws-iot-device-sdk-embedded-c/blob/master/tests/README.md)
 * View further information on how to use the SDK in the Readme file for samples that can be found [here](https://github.com/aws/aws-iot-device-sdk-embedded-c/blob/master/samples/README.md)

Also, for a guided example on getting started with the Thing Shadow, visit the AWS IoT Console's [Interactive Guide](https://console.aws.amazon.com/iot).


## Resources
[API Documentation](http://aws-iot-device-sdk-embedded-c-docs.s3-website-us-east-1.amazonaws.com/index.html)

[MQTT 3.1.1 Spec](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/csprd02/mqtt-v3.1.1-csprd02.html)
