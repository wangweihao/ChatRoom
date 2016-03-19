/*======================================================
    > File Name: server.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月16日 星期三 09时24分20秒
 =======================================================*/

#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <strings.h>
#include "UserInfo.h"
#include "cJSON.h"
#include "handle.h"

/* c mysql 
 * apt-get install libmysqlclient-dev 
 * */
#include <mysql/mysql.h>

#define BACKLOG 1024

void InitServer(char *ip, int port);
void* HandleClient(void*);
int HandleMessage(int sockfd, MYSQL *connect);


void InitServer(char *ip, int port) {
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int sockfd;


    printf("%s\n", ip);
    printf("%d\n", port);

    server.sin_port = port;
    server.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server.sin_addr);

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("create server socket error\n");
        exit(1);
    }

    if(bind(sockfd, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("bind port error");
        exit(1);
    }

    if(listen(sockfd, BACKLOG) == -1) {
        perror("listen error");
        exit(1);
    }

    while (1) {
        int accept_fd = accept(sockfd, (struct sockaddr*)&client, &len);
        if(accept_fd == -1) {
            perror("accept error");
            continue;
        }else {
            printf("accept fd:%d\n", accept_fd);
            pthread_t tid;
            if(pthread_create(&tid, NULL, &HandleClient, &accept_fd) != 0) {
                perror("thread_create error");
                continue;
            } 
        }
    } 
}

void* HandleClient(void *args) {
    MYSQL mysql, *connect;
    

    mysql_init(&mysql);
    if(!(connect = mysql_real_connect(&mysql, "localhost", "root", "w13659218813", "ChatRoom", 0, NULL, 0))) {
        perror("mysql connect error");
        return NULL;
    }

    int sockfd = *(int*)(args);
    
    printf("--------hello world------------\n");
    printf("sockfd:%d\n", sockfd);

    while(1) {
        if(HandleMessage(sockfd, connect) == 0) {
            break;
        }
    }
}

int HandleMessage(int sockfd, MYSQL *connect) {
    cJSON *message;
    char buffer[256];

    bzero(buffer, 256);
    if(recv(sockfd, buffer, 256, 0) == 0) {
        printf("客户端退出...\n");
        return 0;
    }
    printf("JSON info:%s\n", buffer);
    message = cJSON_Parse(buffer);
    int type = cJSON_GetObjectItem(message, "type")->valueint;
    
    switch(type) {
        case 1:
            printf("user register\n");
            HandleUserRegister(message, connect);
            break;
        case 2:
            printf("user login\n");
            break;
        default:
            break;
    }
    return 1;
}

#endif
