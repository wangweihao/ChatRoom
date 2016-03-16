/*======================================================
    > File Name: client.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月16日 星期三 09时33分44秒
 =======================================================*/


#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include "interface.h"


void ClientInit(int fd);
int ConnectServer();
void Start(char *ip, int port);

void ClientInit(int fd) {
    MainInterface(fd);
}

void Start(char *ip, int port) {
    int fd = ConnectServer(ip, port);
    ClientInit(fd);
}

int ConnectServer(char *ip, int port) {
    int sockfd;
    struct sockaddr_in server;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }

    bzero(&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = port;
    inet_pton(AF_INET, ip, &server.sin_addr);

    if(connect(sockfd, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("connect error");
        exit(1);
    }

    return sockfd;
}

#endif
