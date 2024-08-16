#pragma once
#include "Arduino.h"

#define MODEM_BAUDRATE (115200)
#define MODEM_DTR_PIN (25)
#define MODEM_TX_PIN (26)
#define MODEM_RX_PIN (27)
// The modem boot pin needs to follow the startup sequence.
#define BOARD_PWRKEY_PIN (4)
#define BOARD_ADC_PIN (35)
// The modem power switch must be set to HIGH for the modem to supply power.
#define BOARD_POWERON_PIN (12)
#define MODEM_RING_PIN (33)
#define MODEM_RESET_PIN (5)
#define BOARD_MISO_PIN (2)
#define BOARD_MOSI_PIN (15)
#define BOARD_SCK_PIN (14)
#define BOARD_SD_CS_PIN (13)
#define BOARD_BAT_ADC_PIN (35)
#define MODEM_RESET_LEVEL HIGH
#define SerialAT Serial1

#define MODEM_GPS_ENABLE_GPIO (-1)

/* MQTT Params */
// Pay attention to the validity period of the certificate. You can use the following command to obtain the latest certificate again.
// $ openssl s_client -connect brokerurl.s2.eu.hivemq.cloud:8883 -showcerts < /dev/null 2> /dev/null | sed -n '/BEGIN/,/END/p' > server.pem

const char *broker = "fb9170c9f17a43c59786f4337d7dc9e9.s1.eu.hivemq.cloud";
const uint16_t broker_port = 8883;
const char *broker_username = "testMQTTBox";
const char *broker_password = "Test@123";