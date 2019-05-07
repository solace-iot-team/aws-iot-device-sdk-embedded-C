/*
 * Copyright 2019 Solace Corporation. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  https://www.apache.org/licenses/LICENSE-2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
 //Based on aws-iot-sdk for embedded C
/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

/**
 * @file aws_iot_config.h
 * @brief AWS IoT specific configuration file
 */

// Get from console
// =================================================
#define AWS_IOT_MQTT_HOST              "mrjfgwkeg1cer.messaging.solace.cloud" ///< Customer specific MQTT HOST.
#define AWS_IOT_MQTT_PORT              8883 ///< default port for MQTT/S
#define AWS_IOT_MQTT_CLIENT_ID         "default" ///< MQTT client ID should be unique for every device
#define AWS_IOT_ROOT_CA_FILENAME       "digicert-ca" ///< Root CA file name
#define AWS_IOT_CERTIFICATE_FILENAME   "device-cert" ///< device signed certificate file name
#define AWS_IOT_PRIVATE_KEY_FILENAME   "device-private-key" ///< Device private key filename
//#define AWS_IOT_ROOT_CA_FILENAME       "/home/pi/digicert-ca.pem" ///< Root CA file name
//#define AWS_IOT_CERTIFICATE_FILENAME   "/home/pi/01-LSPARK.pem" ///< device signed certificate file name
//#define AWS_IOT_PRIVATE_KEY_FILENAME   "/home/pi/01-LSPARK.key" ///< Device private key filename
// =================================================

// MQTT PubSub
#define AWS_IOT_MQTT_TX_BUF_LEN 512 ///< Any time a message is sent out through the MQTT layer. The message is copied into this buffer anytime a publish is done.
#define AWS_IOT_MQTT_RX_BUF_LEN 512 ///< Any message that comes into the device should be less than this buffer size. If a received message is bigger than this buffer size the message will be dropped.
#define AWS_IOT_MQTT_NUM_SUBSCRIBE_HANDLERS 5 ///< Maximum number of topic filters the MQTT client can handle at any given time.

// Auto Reconnect specific config
#define AWS_IOT_MQTT_MIN_RECONNECT_WAIT_INTERVAL 1000 ///< Minimum time before the First reconnect attempt is made as part of the exponential back-off algorithm
#define AWS_IOT_MQTT_MAX_RECONNECT_WAIT_INTERVAL 128000 ///< Maximum time interval after which exponential back-off will stop attempting to reconnect.

#define DISABLE_METRICS false ///< Disable the collection of metrics by setting this to true

#define STORAGE_FILE 0;
#define STORAGE_SCC 1

#define SCC_PORT "/dev/ttyUSB2"

