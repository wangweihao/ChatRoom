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


/* c mysql 
 * apt-get install libmysqlclient-dev 
 * */
#include <mysql/mysql.h>

#define BACKLOG 1024

MYSQL* ConnectMysql();
void InitServer(char *ip, int port);
void* HandleClient(void*);
void* HandleMessage(char *buffer);

MYSQL* ConnectMysql() {
    MYSQL *connect;

    connect = mysql_init(NULL);
    if(connect == NULL) {
        perror("mysql_init failed\n");
        exit(1);
    }

    connect = mysql_real_connect(connect, "localhost", "root", "w13659218813", "ChatRoom", 0, NULL, 0);
    if(connect != NULL) {
        printf("connect mysql success!\n");
    }else {
        perror("connect mysql error\n");
        exit(1);
    }
}

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
    MYSQL *connect = ConnectMysql();
    printf("hello world\n");
    char buffer[256];
    bzero(buffer, 256);

    while (1) {
        HandleMessage(buffer);
    }
}

void HandleMessage(char* buffer) {

}

#endif
