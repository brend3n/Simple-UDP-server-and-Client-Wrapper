#include "SPI.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <stdlib.h>

int status = WL_IDLE_STATUS;
#include "adruino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int keyIndex = 0;         

unsigned int localPort = 2390;      // local port to listen on

#define send_port 9090
#define send_ip "192.168.1.223"

int serial_enabled = 0;


char packetBuffer[256];
char  ReplyBuffer[] = "Hello, from Arduino.";

WiFiUDP Udp;

void printWifiStatus(void);

void setup() {

  //Initialize serial and wait for port to open:

  Serial.begin(115200);
  Serial.setDebugOutput(true);

  // Wait 5 seconds for serial
  Serial.setTimeout(5000);

  if (Serial)
  {
    serial_enabled = 1;
  }

  WiFi.begin(SECRET_SSID, SECRET_PASS);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.printf("On\n\tIP: %s\tPort: %d\n",WiFi.localIP(), localPort);

  // if (serial_enabled != 0)
  //   Serial.println("Connected to wifi");

  printWifiStatus();

  Serial.println("\nStarting connection to server...");

  // if you get a connection, report back via serial:

  Serial.println("Establishing UDP socket");
  while (!Udp.begin(localPort))
  {
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("UDP socket established");
  // if (serial_enabled != 0)
  //   Serial.println("UDP initialized.");
}

unsigned int msg_cnt = 0;
void loop() {

  uint8_t buffer[1024] = "hello";
  Udp.beginPacket(send_ip, send_port);

  itoa(msg_cnt, ReplyBuffer, 10);
  Udp.write(buffer, 5);
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
