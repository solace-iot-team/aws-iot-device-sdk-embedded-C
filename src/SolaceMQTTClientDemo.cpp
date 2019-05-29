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

#define ENABLE_IOT_DEBUG
#define ENABLE_IOT_INFO
#define ENABLE_IOT_ERROR

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <cstring>
#include "aws_iot_config.h"
#include "aws_iot_log.h"
#include "aws_iot_version.h"
#include "aws_iot_mqtt_client_interface.h"
#include <wiringPi.h>

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <tuple>
#include <memory>

#include "scc-toolkit.h"
#include "Modem_Connector.h"

#define HOST_ADDRESS_SIZE 255
#define GET_ENTRY_BY_ID 0
#define GET_ENTRY_BY_LABEL 1
#define GET_CERT_BY_ID 2
#define GET_CERT_BY_LABEL 3

namespace {

const char* PUBLISH_TOPIC = "$create/iot-control/CA/ON/device/%s/status";

}

static const int USER_LED_PHYS = 13;
static const int USER_BTN_PHYS = 18;

/**
 * @brief Default MQTT HOST URL is pulled from the aws_iot_config.h
 */
char HostAddress[HOST_ADDRESS_SIZE] = AWS_IOT_MQTT_HOST;

/**
 * @brief Default MQTT port is pulled from the aws_iot_config.h
 */
uint32_t port = AWS_IOT_MQTT_PORT;

/**
 * @brief This parameter will avoid infinite loop of publish and exit the program after certain number of publishes
 */
uint32_t publishCount = 1000;

const char* SCC_DEVICE_CERT = "device-cert";

const char* SCC_DIGICERT_CA = "digicert-ca";

const char* SCC_DEVICE_PRIVATE_KEY = "device-private-key";

/**
 * Utility functions to access storage on the device/SIM
 *
 */
int get_scc_internal(scc::SCC_Application *sccApp, int source,
		const char* label, int id, unsigned char* buffer) {
	try {
		std::__cxx11::string entryOrCert = "";
		int size = 0;
		switch (source) {
		case GET_ENTRY_BY_ID:
			entryOrCert = sccApp->getEntry(id).asString();
			size = sccApp->getEntry(id).getData().size();
			break;
		case GET_ENTRY_BY_LABEL:
			entryOrCert = sccApp->getEntry(label).asString();
			size = sccApp->getEntry(label).getData().size();
			break;
		case GET_CERT_BY_ID:
			entryOrCert = sccApp->getCert(id).asString();
			size = sccApp->getCert(id).getData().size();
			break;
		case GET_CERT_BY_LABEL:
			entryOrCert = sccApp->getCert(label).asString();
			size = sccApp->getCert(label).getData().size();
			break;
		default:
			break;
		}
		std::copy(entryOrCert.begin(), entryOrCert.end(), buffer);
		buffer[entryOrCert.length()] = 0;
		return size;
	} catch (const scc::UICC_Connector_Exception &e) {
		IOT_INFO("Error copying from scc [%s]", e.getError().c_str());
		throw &e;
	}

}

int scc_get_entry(scc::SCC_Application *sccApp, int id, unsigned char* buffer) {
	return get_scc_internal(sccApp, GET_ENTRY_BY_ID, "", id, buffer);
}
int scc_get_certificate(scc::SCC_Application *sccApp, int id,
		unsigned char* buffer) {
	return get_scc_internal(sccApp, GET_CERT_BY_ID, "", id, buffer);
}

int scc_get_entry_by_label(scc::SCC_Application *sccApp, char* label,
		unsigned char* buffer) {
	return get_scc_internal(sccApp, GET_ENTRY_BY_LABEL, label, -1, buffer);
}
int scc_get_certificate_by_label(scc::SCC_Application *sccApp,
		const char* label, unsigned char* buffer) {
	return get_scc_internal(sccApp, GET_CERT_BY_LABEL, label, -1, buffer);
}

int scc_get_device_cert(scc::SCC_Application *sccApp, unsigned char* buffer) {
	return get_scc_internal(sccApp, GET_CERT_BY_LABEL, SCC_DEVICE_CERT, -1,
			buffer);
}
int scc_get_device_key(scc::SCC_Application *sccApp, unsigned char* buffer) {
	return get_scc_internal(sccApp, GET_ENTRY_BY_LABEL, SCC_DEVICE_PRIVATE_KEY,
			-1, buffer);
}

/**
 * Subscribe callback, triggered when messages re received via MQTT
 */
void iot_subscribe_callback_handler(AWS_IoT_Client *pClient, char *topicName,
		uint16_t topicNameLen, IoT_Publish_Message_Params *params,
		void *pData) {
	IOT_UNUSED(pData);
	IOT_UNUSED(pClient);
	IOT_INFO("Subscribe callback");
	IOT_INFO("%.*s\t%.*s", topicNameLen, topicName, (int ) params->payloadLen,
			(char * ) params->payload);
	// Blink the user led on the IOT SHield
	for (int i = 0; i < 3; i++) {
		digitalWrite(USER_LED_PHYS, HIGH);
		delay(500);
		digitalWrite(USER_LED_PHYS, LOW);
		delay(200);
	}
}
/**
 * MQTT Disconect handler
 */
void disconnectCallbackHandler(AWS_IoT_Client *pClient, void *data) {
	IOT_WARN("MQTT Disconnect");
	IoT_Error_t rc = FAILURE;

	if (NULL == pClient) {
		return;
	}

	IOT_UNUSED(data);

	if (aws_iot_is_autoreconnect_enabled(pClient)) {
		IOT_INFO(
				"Auto Reconnect is enabled, Reconnecting attempt will start now");
	} else {
		IOT_WARN("Auto Reconnect not enabled. Starting manual reconnect...");
		rc = aws_iot_mqtt_attempt_reconnect(pClient);
		if (NETWORK_RECONNECTED == rc) {
			IOT_WARN("Manual Reconnect Successful");
		} else {
			IOT_WARN("Manual Reconnect Failed - %d", rc);
		}
	}
}

int main(int argc, char** argv) {

	try {
		// connect to the modem and the SIM app, read device IMEI to use as device identification in client id, topics etc.
		std::unique_ptr<scc::Modem_Connector> modem;
		modem = std::make_unique<scc::Modem_Connector>(SCC_PORT);
		scc::SCC_Application sccApp(*modem);
		std::__cxx11::string deviceIMSIStr = modem->readImsi();
		char *deviceIMSI = new char[deviceIMSIStr.size() + 1];
		std::strcpy(deviceIMSI, deviceIMSIStr.c_str());

		IOT_INFO("****************************************");
		IOT_INFO("IMSI / Device id [%s]", deviceIMSI);
		IOT_INFO("****************************************");

		// initialise the wiring pi library that allows to access GPIO pins - used to interact with user led and user button
		wiringPiSetupPhys();
		pinMode(USER_LED_PHYS, OUTPUT);
		pinMode(USER_BTN_PHYS, INPUT);

		bool infinitePublishFlag = true;

		char cPayload[100];

		int32_t i = 0;

		IoT_Error_t rc = FAILURE;

		// set up the MQTT CLient and connect
		AWS_IoT_Client client;
		IoT_Client_Init_Params mqttInitParams = iotClientInitParamsDefault;
		IoT_Client_Connect_Params connectParams = iotClientConnectParamsDefault;

		IoT_Publish_Message_Params paramsQOS0;

		// echo security paramaters set in aws_iot_config
		IOT_DEBUG("rootCA %s", AWS_IOT_ROOT_CA_FILENAME);
		IOT_DEBUG("clientCRT %s", AWS_IOT_CERTIFICATE_FILENAME);
		IOT_DEBUG("clientKey %s", AWS_IOT_PRIVATE_KEY_FILENAME);

		mqttInitParams.enableAutoReconnect = false; // We enable this later below
		mqttInitParams.pHostURL = HostAddress;
		mqttInitParams.port = port;

		/**
		 * Obtain the location of the certificates and key to use.
		 * First initialise the MQTT connection with these locations allowing them to be treated as file locations.
		 * Then attempt to use the locations as keys on the secure storage and load entries and certificates form the card into the mqttInitParams
		 * The MQTT client library will attempt to use the location as file locations and if unsusccessful uses the payload and length parameters instead.
		 */
		mqttInitParams.pRootCALocation = (char *) AWS_IOT_ROOT_CA_FILENAME;
		unsigned char buffer[64000];
		try {
			mqttInitParams.rootCAPayloadLength = scc_get_certificate_by_label(
					&sccApp, AWS_IOT_ROOT_CA_FILENAME, buffer);
			mqttInitParams.pRootCAPayload = buffer;
		} catch (scc::UICC_Exception &e) {
			IOT_INFO("Root CA not loaded from SCC [%s]",
					AWS_IOT_PRIVATE_KEY_FILENAME);
		}
		try {
			unsigned char bufferDeviceCert[64000];
			mqttInitParams.deviceCertPayloadLength =
					scc_get_certificate_by_label(&sccApp,
							AWS_IOT_CERTIFICATE_FILENAME, bufferDeviceCert);
			mqttInitParams.pDeviceCertPayload = bufferDeviceCert;
		} catch (scc::UICC_Exception &e) {
			IOT_INFO("Device cert not loaded from SCC [%s]",
					AWS_IOT_PRIVATE_KEY_FILENAME);
		}
		mqttInitParams.pDeviceCertLocation =
				(char *) AWS_IOT_CERTIFICATE_FILENAME;

		unsigned char bufferDevicePrivateKey[64000];
		mqttInitParams.pDevicePrivateKeyLocation =
				(char *) AWS_IOT_PRIVATE_KEY_FILENAME;
		try {
			mqttInitParams.devicePrivatePayloadLength = scc_get_entry_by_label(
					&sccApp, (char *) AWS_IOT_PRIVATE_KEY_FILENAME,
					bufferDevicePrivateKey);
			mqttInitParams.pDevicePrivatePayload = bufferDevicePrivateKey;
		} catch (scc::UICC_Exception &e) {
			IOT_INFO("Private key not loaded from SCC [%s]",
					AWS_IOT_PRIVATE_KEY_FILENAME);
		}

		/* we are done with the modem / scc app, destroy associated objects.
		 Not doing this may lock up the shield/SCC Connector and require a hard reset of the shield
		 */
		sccApp.~SCC_Application();
		modem.get()->~Modem_Connector();
		modem.release();

		mqttInitParams.mqttCommandTimeout_ms = 20000;
		mqttInitParams.tlsHandshakeTimeout_ms = 5000;
		mqttInitParams.isSSLHostnameVerify = true;
		mqttInitParams.disconnectHandler = disconnectCallbackHandler;
		mqttInitParams.disconnectHandlerData = NULL;
		rc = aws_iot_mqtt_init(&client, &mqttInitParams);
		if (SUCCESS != rc) {
			IOT_ERROR("aws_iot_mqtt_init returned error : %d ", rc);
			return rc;
		}
		connectParams.keepAliveIntervalInSec = 600;
		connectParams.isCleanSession = true;
		connectParams.MQTTVersion = MQTT_3_1_1;
		connectParams.pClientID = deviceIMSI;
		connectParams.clientIDLen = (uint16_t) strlen(deviceIMSI);
		connectParams.isWillMsgPresent = false;
		connectParams.pUsername = deviceIMSI;

		IOT_INFO("Connecting...");
		rc = aws_iot_mqtt_connect(&client, &connectParams);
		if (SUCCESS != rc) {
			IOT_ERROR("Error(%d) connecting to %s:%d", rc,
					mqttInitParams.pHostURL, mqttInitParams.port);
			return rc;
		}
		/*
		 * Enable Auto Reconnect functionality. Minimum and Maximum time of Exponential backoff are set in aws_iot_config.h
		 *  #AWS_IOT_MQTT_MIN_RECONNECT_WAIT_INTERVAL
		 *  #AWS_IOT_MQTT_MAX_RECONNECT_WAIT_INTERVAL
		 */
		rc = aws_iot_mqtt_autoreconnect_set_status(&client, true);
		if (SUCCESS != rc) {
			IOT_ERROR("Unable to set Auto Reconnect to true - %d", rc);
			return rc;
		}

		IOT_INFO("****************************************");
		char cTopicDevice[100];
		sprintf(cTopicDevice, "$create/iot-control/CA/ON/device/%s/command",
				deviceIMSI);
		IOT_INFO("Subscribing to device command... [%s]", cTopicDevice);
		rc = aws_iot_mqtt_subscribe(&client, cTopicDevice, strlen(cTopicDevice),
				QOS0, iot_subscribe_callback_handler, NULL);
		if (SUCCESS != rc) {
			IOT_ERROR("Error subscribing : %d ", rc);
			return rc;
		}
		char cTopicRegion[100];
		sprintf(cTopicRegion, "$create/iot-control/CA/ON/device/command");
		IOT_INFO("Subscribing to regional command... [%s]", cTopicRegion);

		rc = aws_iot_mqtt_subscribe(&client, cTopicRegion, strlen(cTopicRegion),
				QOS0, iot_subscribe_callback_handler, NULL);
		if (SUCCESS != rc) {
			IOT_ERROR("Error subscribing : %d ", rc);
			return rc;
		}
		char cTopicNation[100];
		sprintf(cTopicNation, "$create/iot-control/CA/device/command");
		IOT_INFO("Subscribing to national command ... [%s]", cTopicNation);

		rc = aws_iot_mqtt_subscribe(&client, cTopicNation, strlen(cTopicNation),
				QOS0, iot_subscribe_callback_handler, NULL);
		if (SUCCESS != rc) {
			IOT_ERROR("Error subscribing : %d ", rc);
			return rc;
		}
		IOT_INFO("****************************************");

		sprintf(cPayload, "%s : %d ", "hello from SDK", i);

		paramsQOS0.qos = QOS0;
		paramsQOS0.payload = (void *) cPayload;
		paramsQOS0.isRetained = 0;

		if (publishCount != 0) {
			infinitePublishFlag = false;
		}
		char topic[255];
		sprintf(topic, PUBLISH_TOPIC, deviceIMSI);
		IOT_INFO("****************************************");
		IOT_INFO("Publishing to topic [%s]", topic);
		IOT_INFO("****************************************");

		while ((NETWORK_ATTEMPTING_RECONNECT == rc || NETWORK_RECONNECTED == rc
				|| SUCCESS == rc) && (publishCount > 0 || infinitePublishFlag)) {

			//Max time the yield function will wait for read messages
			rc = aws_iot_mqtt_yield(&client, 100);
			if (NETWORK_ATTEMPTING_RECONNECT == rc) {
				// If the client is attempting to reconnect we will skip the rest of the loop.
				continue;
			}
			delay(50);
			// send message only if user button is pressed
			int btnPressed = !(digitalRead(USER_BTN_PHYS));
			if (btnPressed) {
				sprintf(cPayload, "{\"activity\":\"%s\", \"count\": %d }",
						"Button pressed", i++);
				paramsQOS0.payloadLen = strlen(cPayload);
				rc = aws_iot_mqtt_publish(&client, topic, strlen(topic),
						&paramsQOS0);
				if (publishCount > 0) {
					publishCount--;
				}
			}

			if (publishCount == 0 && !infinitePublishFlag) {
				break;
			}

			// Wait for all the messages to be received
			aws_iot_mqtt_yield(&client, 100);

			if (SUCCESS != rc) {
				IOT_ERROR("An error occurred in the loop.\n");
			}
		}
		return rc;
	} catch (const scc::UICC_Connector_Exception &e) {
		IOT_INFO("Error accessing cellular shield [%s]", e.getError().c_str());
		throw &e;
	}
}

