#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9090
// #define IP_ADDR "127.0.0.1"
#define IP_ADDR "192.168.1.223"

char server_message[100], client_message[100];

void clear_buffers(){
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
}

void append_to_file(FILE* f, uint8_t* buffer)
{
    f = fopen("data.log", "a");

    if (!f)
    {
        printf("Cannot open data.log.\n");
        exit(1);
    }
    fputs(buffer,f);
    fclose(f);
}

int main(void){
    FILE* fptr;
    int socket_desc;
    struct sockaddr_in server_addr, client_addr;
    int client_struct_length = sizeof(client_addr);
    
    clear_buffers();

    // Create UDP socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP_ADDR);
    
    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Finished binding\n");
    
    printf("Listening for incoming messages...\n\n");

    while (1)
    {
        clear_buffers();
        // Receive client's message:
        if (recvfrom(socket_desc, client_message, sizeof(client_message), 0,
            (struct sockaddr*)&client_addr, &client_struct_length) < 0){
            // Nothing received so try again
            continue;
        }
        printf("Received message from IP: %s and port: %i\n",
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        printf("Msg from client: %s\n", client_message);
        append_to_file(fptr, client_message);
        append_to_file(fptr, "\n");
        
        // Change to uppercase:
        for(int i = 0; client_message[i]; ++i)
            client_message[i] = toupper(client_message[i]);
        
        // Respond to client:
        strcpy(server_message, client_message);
        
        if (sendto(socket_desc, server_message, strlen(server_message), 0,
            (struct sockaddr*)&client_addr, client_struct_length) < 0){
            printf("Can't send\n");
        }else{
            printf("Sent message\n");
        }
    }
    
    // Close the socket:
    close(socket_desc);

}
