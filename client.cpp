#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define PORT 8080
  
int connectServer();

int main() {
    float quaternion[4];
    int socketID = connectServer();
    while(recv(socketID , quaternion, 4*sizeof(float), 0) >= 0) {
        printf("%f %f %f %f\n", quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
    }
    return 0;
}

int connectServer() {
    struct sockaddr_in address;
    int socketID = 0;
    struct sockaddr_in serv_addr;
    if ((socketID = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "192.168.1.7", &serv_addr.sin_addr)<=0)  {
        perror("Invalid ip address");
        exit(EXIT_FAILURE);
    }
  
    if (connect(socketID, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    return socketID;
}