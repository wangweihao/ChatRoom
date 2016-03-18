/*======================================================
    > File Name: handle.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月18日 星期五 16时43分31秒
 =======================================================*/

#ifndef _HANDLE_H_
#define _HANDLE_H_

#include "cJSON.h"
#include <stdio.h>

void HandleUserRegister(cJSON *message);



void HandleUserRegister(cJSON *message) {
    char *account = cJSON_GetObjectItem(message, "account")->valuestring;
    char *secret = cJSON_GetObjectItem(message, "secret")->valuestring;
    char *nickname = cJSON_GetObjectItem(message, "nickname")->valuestring;
    char *sex = cJSON_GetObjectItem(message, "sex")->valuestring;
    uint16_t age = cJSON_GetObjectItem(message, "age")->valueint;
    printf("account:%s\n", account);
    printf("secret:%s\n", secret);
    printf("nickname:%s\n", nickname);
    printf("sex:%s\n", sex);
    printf("age:%hd\n", age);
}

#endif
