#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

#include "config.h"
#include "HivemqRootCA.h"
#include <TinyGsmClient.h>
#include "proto/test.pb.h"
#include "pb_common.h"
#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "base64.hpp"

#ifdef DUMP_AT_COMMANDS // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

String IMEI;

// Current connection index, range 0~1
const uint8_t mqtt_client_id = 0;
uint32_t check_connect_millis = 0;

void mqtt_callback(const char *topic, const uint8_t *payload, uint32_t len)
{
  Serial.println();
  Serial.println("======mqtt_callback======");
  Serial.print("Topic:");
  Serial.println(topic);
  Serial.println("Payload:");

  unsigned char payloadCharArray[len + 1]; // +1 for the null terminator
  for (int i = 0; i < len; ++i)
  {
    // Serial.print(payload[i], HEX);
    payloadCharArray[i] = static_cast<char>(payload[i]);
    Serial.print(char(payloadCharArray[i]));
  }
  payloadCharArray[len] = '\0'; // Null-terminate the char array
  Serial.println(len);
  Serial.println("=========================");

  // Convert the received payload (Base64 string) back to binary data
  unsigned char binary[len + 1];
  unsigned int binary_length = decode_base64(payloadCharArray, binary);

  DeviceInfo info = DeviceInfo_init_zero;
  pb_istream_t stream = pb_istream_from_buffer(binary, binary_length);

  // Decode the message
  bool status = pb_decode(&stream, DeviceInfo_fields, &info);

  if (!status)
  {
    Serial.println("Decoding failed");
    return;
  }

  // Access the decoded fields
  Serial.print("Test Number: ");
  Serial.println(info.mac_id);
}

bool mqtt_connect()
{
  IMEI = modem.getIMEI();
  Serial.println(IMEI);

  Serial.print("Connecting to ");
  Serial.print(broker);

  bool ret = modem.mqtt_connect(mqtt_client_id, broker, broker_port, IMEI.c_str(), broker_username, broker_password); // client_id is the IMEI number
  if (!ret)
  {
    Serial.println(" Failed!");
    return false;
  }
  Serial.println(" Success.");

  if (modem.mqtt_connected())
  {
    Serial.println("MQTT has connected!");
  }
  else
  {
    return false;
  }

  String subscribe_topic = "devices/" + IMEI + "/devicebound/#";

  // Set MQTT processing callback
  modem.mqtt_set_callback(mqtt_callback);
  // Subscribe to topic
  modem.mqtt_subscribe(mqtt_client_id, subscribe_topic.c_str());

  return true;
}

void setup()
{
  Serial.begin(115200); // Set console baud rate

  Serial.println("Start Sketch");

  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);

#ifdef BOARD_POWERON_PIN
  pinMode(BOARD_POWERON_PIN, OUTPUT);
  digitalWrite(BOARD_POWERON_PIN, HIGH);
#endif

  // Set modem reset pin ,reset modem
  pinMode(MODEM_RESET_PIN, OUTPUT);
  digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);
  delay(100);
  digitalWrite(MODEM_RESET_PIN, MODEM_RESET_LEVEL);
  delay(2600);
  digitalWrite(MODEM_RESET_PIN, !MODEM_RESET_LEVEL);

  pinMode(BOARD_PWRKEY_PIN, OUTPUT);
  digitalWrite(BOARD_PWRKEY_PIN, LOW);
  delay(100);
  digitalWrite(BOARD_PWRKEY_PIN, HIGH);
  delay(100);
  digitalWrite(BOARD_PWRKEY_PIN, LOW);

  // Check if the modem is online
  Serial.println("Start modem...");

  int retry = 0;
  while (!modem.testAT(1000))
  {
    Serial.println(".");
    if (retry++ > 10)
    {
      digitalWrite(BOARD_PWRKEY_PIN, LOW);
      delay(100);
      digitalWrite(BOARD_PWRKEY_PIN, HIGH);
      delay(1000);
      digitalWrite(BOARD_PWRKEY_PIN, LOW);
      retry = 0;
    }
  }
  Serial.println();

  // Check if SIM card is online
  SimStatus sim = SIM_ERROR;
  while (sim != SIM_READY)
  {
    sim = modem.getSimStatus();
    switch (sim)
    {
    case SIM_READY:
      Serial.println("SIM card online");
      break;
    case SIM_LOCKED:
      Serial.println("The SIM card is locked. Please unlock the SIM card first.");
      // const char *SIMCARD_PIN_CODE = "123456";
      // modem.simUnlock(SIMCARD_PIN_CODE);
      break;
    default:
      break;
    }
    delay(1000);
  }

  // SIM7672G Can't set network mode
#ifndef TINY_GSM_MODEM_SIM7672
  if (!modem.setNetworkMode(MODEM_NETWORK_AUTO))
  {
    Serial.println("Set network mode failed!");
  }
  String mode = modem.getNetworkModes();
  Serial.print("Current network mode : ");
  Serial.println(mode);
#endif

  // Check network registration status and network signal status
  int16_t sq;
  Serial.print("Wait for the modem to register with the network.");
  RegStatus status = REG_NO_RESULT;
  while (status == REG_NO_RESULT || status == REG_SEARCHING || status == REG_UNREGISTERED)
  {
    status = modem.getRegistrationStatus();
    switch (status)
    {
    case REG_UNREGISTERED:
    case REG_SEARCHING:
      sq = modem.getSignalQuality();
      Serial.printf("[%lu] Signal Quality:%d", millis() / 1000, sq);
      delay(1000);
      break;
    case REG_DENIED:
      Serial.println("Network registration was rejected, please check if the APN is correct");
      return;
    case REG_OK_HOME:
      Serial.println("Online registration successful");
      break;
    case REG_OK_ROAMING:
      Serial.println("Network registration successful, currently in roaming mode");
      break;
    default:
      Serial.printf("Registration Status:%d\n", status);
      delay(1000);
      break;
    }
  }
  Serial.println();

  Serial.printf("Registration Status:%d\n", status);
  delay(1000);

  String ueInfo;
  if (modem.getSystemInformation(ueInfo))
  {
    Serial.print("Inquiring UE system information:");
    Serial.println(ueInfo);
  }

  if (!modem.enableNetwork())
  {
    Serial.println("Enable network failed!");
  }

  delay(5000);

  String ipAddress = modem.getLocalIP();
  Serial.print("Network IP:");
  Serial.println(ipAddress);

  Serial.println(ESP.getSketchMD5());
  Serial.println(modem.getModemName());

  // Initialize MQTT
  // Hivemq needs to enable sni before it can be run and connected.
  bool enableSSL = true;
  bool enableSNI = true;
  modem.mqtt_begin(enableSSL, enableSNI);

  // Set up Hivemq  ROOT certificate
  modem.mqtt_set_certificate(HivemqRootCA);

  if (!mqtt_connect())
  {
    Serial.print("MQTT connection failed!");
    delay(1000);
  }

  /*Send the device info */
  uint8_t buffer[300];
  DeviceInfo info = DeviceInfo_init_zero;
  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  strcpy(info.identification_type, "imei");
  strcpy(info.identification_value, IMEI.c_str());
  strcpy(info.imei, IMEI.c_str());
  strcpy(info.sw_version, "2.1.231211");
  strcpy(info.esp_sdk_ver, ESP.getSdkVersion());
  info.mac_id = ESP.getEfuseMac();
  strcpy(info.chip_model, ESP.getChipModel());
  info.chip_rev = ESP.getChipRevision();
  info.sketch_size = ESP.getSketchSize();
  info.sketch_free_size = ESP.getFreeSketchSpace();
  strcpy(info.sketch_md5, ESP.getSketchMD5().c_str());
  strcpy(info.modem_name, modem.getModemName().c_str());
  strcpy(info.sim_ccid, modem.getSimCCID().c_str());
  strcpy(info.imsi, modem.getIMSI().c_str());
  strcpy(info.net_operator, modem.getOperator().c_str());
  info.net_signal = modem.getSignalQuality();
  strcpy(info.net_ip, modem.getLocalIP().c_str());

  bool res = pb_encode(&stream, DeviceInfo_fields, &info);
  if (!res)
  {
    Serial.println("Failed to encode");
    return;
  }
  Serial.print("Message Length: ");
  Serial.println(stream.bytes_written);

  // Convert binary data to Base64 string
  unsigned char base64[300];

  unsigned int base64_length = encode_base64(buffer, stream.bytes_written, base64);
  String publishTopic = "devices/" + IMEI + "/info";

  // modem.mqtt_publish(0, publishTopic.c_str(), payload.c_str());
  modem.mqtt_publish(0, publishTopic.c_str(), (char *)base64);
}

void loop()
{
  // Check the connection every ten seconds
  if (millis() > check_connect_millis)
  {
    check_connect_millis = millis() + 10000UL;
    if (!modem.mqtt_connected())
    {
      mqtt_connect();
    }
    else
    {
      // If connected, send information once every ten seconds
      // String payload = "RunTime:" + String(millis() / 1000);
      // modem.mqtt_publish(0, publish_topic, payload.c_str());
    }
  }
  // MQTT handling
  modem.mqtt_handle();
  delay(5);
}