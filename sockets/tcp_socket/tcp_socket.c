#include "tcp_socket.h"

// Initializes a UDP socket that can send and recv datagrams
uint8_t tcp_open (   
                        struct TCP_Socket* tcp_socket,
                        char* dest_addr, 
                        char* local_addr,
                        int dest_port, int local_port, 
                        char* log_name)                    
{
    // Create UDP socket:
    tcp_socket->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(tcp_socket->socket < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // Set port and IP:
    tcp_socket->local_ip_addr.sin_family = AF_INET;
    tcp_socket->local_ip_addr.sin_port = htons(local_port);
    tcp_socket->local_ip_addr.sin_addr.s_addr = inet_addr(local_addr);
    tcp_socket->local_ip = local_addr;
    tcp_socket->local_port = local_port;
    
    // Bind to the set port and IP:
    if(bind(tcp_socket->socket, (struct sockaddr*)&tcp_socket->local_ip_addr, sizeof(tcp_socket->local_ip_addr)) < 0){
        printf("Couldn't bind to the port\n");
        return -1;
    }

    // Set port and IP:
    tcp_socket->dest_ip_addr.sin_family = AF_INET;
    tcp_socket->dest_ip_addr.sin_port = htons(dest_port);
    tcp_socket->dest_ip_addr.sin_addr.s_addr = inet_addr(dest_addr);
    tcp_socket->dest_ip = dest_addr;
    tcp_socket->dest_port = dest_port;

    tcp_socket->file_name = log_name;

}
// Writes a buffer to the sending destination port and ip
uint8_t tcp_write(struct TCP_Socket* tcp_socket, 
                        uint8_t* data, 
                        int data_len)
{
    // Send the message to server:
    int bytes_sent = sendto(tcp_socket->socket, data, data_len, 0,
                            (struct sockaddr*)&tcp_socket->dest_ip_addr, 
                            sizeof(tcp_socket->dest_ip_addr));

    if (bytes_sent < 0)
    {
        // error with sending
        return -1;
    }

    log_to_file(tcp_socket->file_name, data, "Tx", tcp_socket->dest_ip, tcp_socket->dest_port);   
}
// Reads any incoming bytes and puts it into the buffer
uint8_t tcp_read (struct TCP_Socket* tcp_socket, 
                        uint8_t* read_buffer, 
                        int data_len)
{
    // Receive the server's response:
    int bytes_recvd = recv(tcp_socket->socket, read_buffer, data_len, 0);


    if (bytes_recvd < 0)
    {
        // error with sending
        return -1;
    }

    log_to_file(tcp_socket->file_name, read_buffer, "Rx", tcp_socket->dest_ip, tcp_socket->dest_port);   
}
// Closes the socket
uint8_t tcp_close(struct TCP_Socket* tcp_socket)
{
    printf("Closing socket.\n");
    close(tcp_socket->socket);
}

void log_to_file(char* file_name, uint8_t* buffer, char* msg_type, char* ip_msg, int port_msg)
{
    FILE* f;
    f = fopen(file_name, "a");

    if (!f)
    {
        printf("Cannot open %s.\n", file_name);
        return;
    }
    char prefix[1024];

    // prefix = [RX or TX] [RX or TX IP] [RX or TX Port]
    sprintf(prefix, "%s,%s,%d,",msg_type,ip_msg,port_msg);

    fputs(prefix,f);
    fputs(buffer,f);
    fputs("\n", f);

    fclose(f);
}
