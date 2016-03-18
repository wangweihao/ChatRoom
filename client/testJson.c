/*======================================================
    > File Name: testJson.c
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月18日 星期五 16时03分59秒
 =======================================================*/

#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include <stdint.h>

int main() {
    cJSON *user;
    uint16_t age = 123;

    user = cJSON_CreateObject();

    cJSON_AddStringToObject(user, "account", "wangweihao");
    cJSON_AddStringToObject(user, "secret", "123123123");
    cJSON_AddNumberToObject(user, "age", age);

    char *p = cJSON_Print(user);
    printf("JSON:%s\n", p);
    printf("length:%d\n", strlen(p));
}

