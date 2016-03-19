/*======================================================
    > File Name: register.c
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月16日 星期三 01时23分11秒
 =======================================================*/


#include <mysql/mysql.h>
#include "UserInfo.h"
#include <stdio.h>

int _RegisterUserInfo(UserInfo *user, int fd);
int _UserLogin(UserInfo *user, int fd);

int _RegisterUserInfo(UserInfo *user, int fd) {
    cJSON *userinfo;
    cJSON *retmsg;
    int ret = 0;
    char *buffer;
    char retbuffer[256];

    bzero(retbuffer, 256);
    printf("用户注册\n");
    
    userinfo = cJSON_CreateObject();
    cJSON_AddStringToObject(userinfo, "account", user->account);
    cJSON_AddStringToObject(userinfo, "secret", user->secret);
    cJSON_AddStringToObject(userinfo, "nickname", user->nickname);
    cJSON_AddStringToObject(userinfo, "sex", user->sex);
    cJSON_AddNumberToObject(userinfo, "age", user->age);
    cJSON_AddNumberToObject(userinfo, "type", 1);

    buffer = cJSON_Print(userinfo);

    size_t length = strlen(buffer);
    ssize_t size = send(fd, buffer, length, 0);
    if(length == size) {
        printf("send success!\n");
    }else {
        printf("send error!\n");
    }

    if(recv(fd, retbuffer, 256, 0) <= 0) {
        printf("系统错误...，请稍后再试...\n");
    }else {
        printf("%s\n", retbuffer);
        retmsg = cJSON_Parse(retbuffer);
        ret = cJSON_GetObjectItem(retmsg, "ret")->valueint;
        if(ret == 0) {
            printf("注册成功...\n");
        } else {
            printf("注册失败...\n");
        }
        system("clear");
        printf("%s\n", cJSON_GetObjectItem(retmsg, "value")->valuestring);
        sleep(2);
    }

    return 0;

}

int _UserLogin(UserInfo *user, int fd) {
    cJSON *userinfo;
    cJSON *retmsg;
    int ret = 0;
    char *buffer;
    char retbuffer[256];

    printf("user login...\n");

    userinfo = cJSON_CreateObject();
    cJSON_AddStringToObject(userinfo, "account", user->account);
    cJSON_AddStringToObject(userinfo, "secret", user->secret);
    cJSON_AddNumberToObject(userinfo, "type", 2);
    buffer = cJSON_Print(userinfo);
    size_t length = strlen(buffer);
    ssize_t size = send(fd, buffer, length, 0);
    if(length == size) {
        printf("send success!\n");
    }else {
        printf("send error!\n");
    }

    if(recv(fd, retbuffer, 256, 0) <= 0) {
        printf("系统错误...请稍后再试...\n");
    }else {
        printf("%s\n", retbuffer);
        retmsg = cJSON_Parse(retbuffer);
        ret = cJSON_GetObjectItem(retmsg, "ret")->valueint;
        if(ret == 0) {
            printf("登录成功...\n");
        } else {
            printf("登录失败...\n");
        }
        system("clear");
        printf("%s\n", cJSON_GetObjectItem(retmsg, "value")->valuestring);
        sleep(2);
    }

    return 0;
}



