#include "../../udp_socket.h"
#include <string.h>

struct UDP_Socket udp_recv_;

#define LOCAL_IP "192.168.1.223"
#define LOCAL_PORT 9090

#define DEST_IP  "192.168.1.223"
#define DEST_PORT 53535

#define FILENAME "server.log"

#define BUFF_SIZE 1024

int main(void){

    udp_open(&udp_recv_,
        DEST_IP  , LOCAL_IP,
        DEST_PORT, LOCAL_PORT,
        FILENAME
    );

    char Test_Message[BUFF_SIZE];
    char send_buff[]="DEADBEEF BRENDEN\0";
    printf("Listening for incoming messages on\n\tIP: %s\tPort: %d\n\n",LOCAL_IP,LOCAL_PORT);
    while(1)
    {
        udp_read(&udp_recv_,
                Test_Message, 
                BUFF_SIZE);

        printf("Recvd: %s\n", Test_Message);

        udp_write(&udp_recv_,
            send_buff,
            strlen(send_buff));
    }

    udp_close(&udp_recv_);
    return 0;
}