#include "UDP_socket.h"

// Uncomment your board (only one should be uncommented)

// #define NANOIOT33 
// #define ESP32


char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int udp_open(struct UDP_socket* sock, char* dest_ip, int dest_port, char* local_ip, int local_port)
{
    #ifdef NANOIOT33
    // attempt to connect to Wifi network:
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    WiFi.setSleep(false);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.printf("On\n\tIP: %s\tPort: %d\n",WiFi.localIP(), local_port);


    String(WiFi.localIP()).toCharArray(local_ip, String(WiFi.localIP()).length());

    printWifiStatus();
    sock->udp.begin(local_port);
    #endif
}
int udp_send(struct UDP_socket* sock, uint8_t* buffer, int n_bytes_to_send)
{
    sock->udp.beginPacket(sock->dest_ip, sock->dest_port);
    sock->udp.write(buffer, n_bytes_to_send);
    sock->udp.endPacket();
}
int udp_recv(struct UDP_socket* sock, char* buffer, int n_bytes_to_recv)
{
    // Waiting for a packet
    while(!sock->udp.parsePacket()){;};

    return sock->udp.read(buffer, n_bytes_to_recv);
}
int udp_close(struct UDP_socket* sock)
{
    sock->udp.stop();
}

void printWifiStatus() {
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