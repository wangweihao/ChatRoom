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


int _RegisterUserInfo(UserInfo *user, int fd) {
    cJSON *userinfo;
    char *buffer;

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

    return 0;

}

