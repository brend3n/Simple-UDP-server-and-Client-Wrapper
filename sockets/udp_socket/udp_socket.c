#include "udp_socket.h"


// Initializes a UDP socket that can send and recv datagrams
uint8_t udp_open (   
                        struct UDP_Socket* udp_socket,
                        char* dest_addr, 
                        char* local_addr,
                        int dest_port, int local_port, 
                        char* log_name)                    
{
    // Create UDP socket:
    udp_socket->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(udp_socket->socket < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // Set port and IP:
    udp_socket->local_ip_addr.sin_family = AF_INET;
    udp_socket->local_ip_addr.sin_port = htons(local_port);
    udp_socket->local_ip_addr.sin_addr.s_addr = inet_addr(local_addr);
    udp_socket->local_ip = local_addr;
    udp_socket->local_port = local_port;
    
    // Bind to the set port and IP:
    if(bind(udp_socket->socket, (struct sockaddr*)&udp_socket->local_ip_addr, sizeof(udp_socket->local_ip_addr)) < 0){
        printf("Couldn't bind to the port\n");
        return -1;
    }

    // Set port and IP:
    udp_socket->dest_ip_addr.sin_family = AF_INET;
    udp_socket->dest_ip_addr.sin_port = htons(dest_port);
    udp_socket->dest_ip_addr.sin_addr.s_addr = inet_addr(dest_addr);
    udp_socket->dest_ip = dest_addr;
    udp_socket->dest_port = dest_port;

    udp_socket->file_name = log_name;

}
// Writes a buffer to the sending destination port and ip
uint8_t udp_write(struct UDP_Socket* udp_socket, 
                        uint8_t* data, 
                        int data_len)
{
    // Send the message to server:
    int bytes_sent = sendto(udp_socket->socket, data, data_len, 0,
                            (struct sockaddr*)&udp_socket->dest_ip_addr, 
                            sizeof(udp_socket->dest_ip_addr));

    if (bytes_sent < 0)
    {
        // error with sending
        return -1;
    }

    log_to_file(udp_socket->file_name, data, "Tx", udp_socket->dest_ip, udp_socket->dest_port);   
}
// Reads any incoming bytes and puts it into the buffer
uint8_t udp_read (struct UDP_Socket* udp_socket, 
                        uint8_t* read_buffer, 
                        int data_len)
{
    struct sockaddr_in temp_addr;
    int struct_len = sizeof(temp_addr);
    // memset(&udp_socket->dest_ip_addr,0, sizeof(udp_socket->dest_ip_addr));


    // Receive the server's response:
    // int bytes_recvd = recv(udp_socket->socket, read_buffer, data_len, 0);
    int bytes_recvd = recvfrom(udp_socket->socket, read_buffer, data_len, 0,
            (struct sockaddr*)&temp_addr, &struct_len);


    if (bytes_recvd < 0)
    {
        // error with sending
        return -1;
    }

    log_to_file(udp_socket->file_name, read_buffer, "Rx", inet_ntoa(temp_addr.sin_addr), ntohs(temp_addr.sin_port));   
}
// Closes the socket
uint8_t udp_close(struct UDP_Socket* udp_socket)
{
    printf("Closing socket.\n");
    close(udp_socket->socket);
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
