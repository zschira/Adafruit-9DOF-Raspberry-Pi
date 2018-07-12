#include "ADAFRUIT_9DOF.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define PORT 8080

int startServer();

int main() {
    float quaternion[4] = {1.0, 0.0, 0.0, 0.0};
    ADAFRUIT_9DOF imu;
    int socketID = startServer();
    imu.setAutoRange();
    for(int i=0; i < 1000; i++) {
        imu.calcCoord(quaternion);
        send(socketID, quaternion, 4*sizeof(float), 0);
    }
    if(shutdown(socketID, 2) < 0) {
        perror("socket shutdown failed");
        exit(EXIT_FAILURE);  
    }
    return 0;
}

int startServer() {
    int server_fd,socketID;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
      
    // Creating socket file descriptor
    if(!(server_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((socketID = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    return socketID;
}