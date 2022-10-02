#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define server_PORT 9090
// #define server_IP_ADDR "127.0.0.1"
#define server_IP_ADDR "192.168.1.223"
char server_message[100], client_message[100];

void clear_buffers(){
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
}

int main(void){
    int socket_desc;
    struct sockaddr_in server_addr;
    int server_struct_length = sizeof(server_addr);
    
    // Clean buffers:
    clear_buffers();
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_PORT);
    server_addr.sin_addr.s_addr = inet_addr(server_IP_ADDR);
    
    while (1){
        clear_buffers();
        // Get input from the user:
        printf("Enter message: ");
        scanf("%s",client_message);
        
        // Send the message to server:
        if(sendto(socket_desc, client_message, strlen(client_message), 0,
            (struct sockaddr*)&server_addr, server_struct_length) < 0){
            printf("Unable to send message\n");
            continue;
        }
        
        // Receive the server's response:
        if(recvfrom(socket_desc, server_message, sizeof(server_message), 0,
            (struct sockaddr*)&server_addr, &server_struct_length) < 0){
            printf("Error while receiving server's msg\n");
            continue;            
        }
        
        printf("Server's response: %s\n", server_message);
    }
    
    // Close the socket:
    close(socket_desc);
    
    return 0;
}

