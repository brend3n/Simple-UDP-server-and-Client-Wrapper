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

    char Test_Message[]="DEADBEEF BRENDEN\0";
    char buffer[BUFF_SIZE];

    udp_write(&udp_sender_,
            Test_Message,
            strlen(Test_Message));

    udp_read(&udp_sender_,
        buffer, 
        BUFF_SIZE);

    printf("Recvd: %s\n", buffer);    

    udp_close(&udp_sender_);
    return 0;
}