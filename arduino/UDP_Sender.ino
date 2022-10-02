

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <stdlib.h>

int status = WL_IDLE_STATUS;
#include "adruino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 2390;      // local port to listen on

#define send_port 9090
#define send_ip "192.168.1.223"

int serial_enabled = 0;


char packetBuffer[256]; //buffer to hold incoming packet
char  ReplyBuffer[] = "Hello, from Arduino.";       // a string to send back

WiFiUDP Udp;

void printWifiStatus(void);

void setup() {

  //Initialize serial and wait for port to open:

  Serial.begin(9600);

  // Wait 5 seconds for serial
  Serial.setTimeout(5000);

  if (Serial)
  {
    serial_enabled = 1;
  }

  // check for the WiFi module:

  if (WiFi.status() == WL_NO_MODULE) {

    if (serial_enabled != 0)
      Serial.println("Communication with WiFi module failed!");

    // don't continue

    while (true);

  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    if (serial_enabled != 0)
      Serial.println("Please upgrade the firmware");

  }

  // attempt to connect to Wifi network:

  while (status != WL_CONNECTED) {

    if (serial_enabled != 0)
    {
      Serial.print("Attempting to connect to SSID: ");

      Serial.println(ssid);
    }
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:

    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:

    delay(10000);

  }

  if (serial_enabled != 0)
    Serial.println("Connected to wifi");

  printWifiStatus();

  Serial.println("\nStarting connection to server...");

  // if you get a connection, report back via serial:

  Udp.begin(localPort);
  if (serial_enabled != 0)
    Serial.println("UDP initialized.");
}

unsigned int msg_cnt = 0;
void loop() {

  Udp.beginPacket(send_ip, send_port);

  itoa(msg_cnt, ReplyBuffer, 10);
  Udp.write(ReplyBuffer);
  msg_cnt++;

  Udp.endPacket();
  delay(1000);
}

void printWifiStatus() {

  if (serial_enabled == 0)
  {
    return;
  }
  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  // print your board's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);

  // print the received signal strength:

  long rssi = WiFi.RSSI();

  Serial.print("signal strength (RSSI):");

  Serial.print(rssi);

  Serial.println(" dBm");
}