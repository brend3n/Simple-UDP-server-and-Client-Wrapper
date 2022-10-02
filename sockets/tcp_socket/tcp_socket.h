#ifndef TCP_SOCKET_H_
#define TCP_SOCKET_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

struct TCP_Socket
{
    int socket;
    char* file_name;

    char* local_ip;
    int local_port;

    char* dest_ip;
    int dest_port;

    struct sockaddr_in local_ip_addr;
    struct sockaddr_in dest_ip_addr;

}TCP_Socket;

// Initializes a UDP socket that can send and recv datagrams
uint8_t tcp_open (   struct TCP_Socket* udp_socket,
                        char* dest_addr, 
                        char* local_addr,
                        int dest_port, int local_port,
                        char* log_name
                    );

// Writes a buffer to the sending destination port and ip
uint8_t tcp_write(struct TCP_Socket* udp_socket, 
                        uint8_t* data, 
                        int data_len
                    );

// Reads any incoming bytes and puts it into the buffer
uint8_t tcp_read (struct TCP_Socket* udp_socket, 
                        uint8_t* read_buffer, 
                        int data_len
                    );
// Closes the socket
uint8_t tcp_close(struct TCP_Socket* udp_socket);

void log_to_file(char* file_name, uint8_t* buffer, char* msg_type, char* ip, int port);


#endif // TCP_SOCKET_H_