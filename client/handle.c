/*======================================================
    > File Name: handle.c
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年05月03日 星期二 18时54分51秒
 =======================================================*/

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include "cJSON.h"

void* thread_start_chat(void *arg) {
    int fd = *(int*)(arg);
    char buffer[256];
    cJSON *mesg;

    while(1) {
        bzero(buffer, 256);
        recv(fd, buffer, 256, 0);
        mesg = cJSON_Parse(buffer);
        char *info = cJSON_GetObjectItem(mesg, "info")->valuestring;
        if(strcmp(info, "quit") == 0) {
            break;
        }
        printf("%s\n", buffer);
    }
}

void ChatAndOneFriend(char *account, char *name, int fd) {
    pthread_t tid; 
    char buffer[256];
    cJSON *info;
    char *buf;
    
    if(pthread_create(&tid, NULL, thread_start_chat, &fd) != 0) {
        perror("pthread_create() error\n");
        return;
    }  

    while(1) {
        bzero(buffer, 256);
        scanf("%s", buffer);
        info = cJSON_CreateObject();
        cJSON_AddNumberToObject(info, "type", 11);
        cJSON_AddStringToObject(info, "name", name);
        cJSON_AddStringToObject(info, "myName", account);
        cJSON_AddStringToObject(info, "info", buffer);
        buf = cJSON_Print(info);

        send(fd, buf, strlen(buf), 0);
        if(strcmp(buffer, "quit") == 0) {
            pthread_join(tid, NULL);
            break;
        }
    }
}

