#ifndef UDP_SOCKET_H_
#define UDP_SOCKET_H_

#include "arduino_secrets.h"
#include <unistd.h>
#include <stdint.h>
#ifdef NANOIOT33
    #include <SPI.h>
    #include <WiFiNINA.h>
    #include <WiFiUdp.h>
    #include <stdlib.h>
#endif // NANOIOT33
#ifdef ESP32
    #include <SPI.h>
    #include <WiFi.h>
    #include <WiFiUdp.h>
    #include <stdlib.h>
#endif // ESP32

struct UDP_socket{
    WiFiUDP udp;

    char* dest_ip;
    int dest_port;

    char* local_ip;
    int local_port;

}UDP_socket;

int udp_open(struct UDP_socket* sock, char* dest_ip, int dest_port, char* local_ip, int local_port);
int udp_send(struct UDP_socket* sock, uint8_t* buffer, int n_bytes_to_send);
int udp_recv(struct UDP_socket* sock, uint8_t* buffer, int n_bytes_to_recv);
int udp_close(struct UDP_socket* sock);

void printWifiStatus(void);

#endif // UDP_SOCKET_H_