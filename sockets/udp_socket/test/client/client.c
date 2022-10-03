#include "../../udp_socket.h"
#include <string.h>

struct UDP_Socket udp_sender_;

#define LOCAL_IP "192.168.1.223"
#define LOCAL_PORT 53535

#define DEST_IP  "192.168.1.223"
#define DEST_PORT 9090

#define FILENAME "client.log"
#define BUFF_SIZE 1024

int main(void){

    udp_open(&udp_sender_,
        DEST_IP  , LOCAL_IP,
        DEST_PORT, LOCAL_PORT,
        FILENAME
    );

    char Test_Message[]= "MORTON";
    char buffer[BUFF_SIZE];

    while (1){
        memset(buffer,0,sizeof(buffer));

        char echo_str[1024];
        printf("Enter a string: ");
        scanf("%s", echo_str);
        
        udp_write(&udp_sender_,
                echo_str,
                strlen(echo_str));

        udp_read(&udp_sender_,
            buffer, 
            BUFF_SIZE);

        printf("Recvd: %s\n", buffer); 
    }
       


    udp_close(&udp_sender_);
    return 0;
}