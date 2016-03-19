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
#include "mysql/mysql.h"
#include <stdio.h>


void HandleUserRegister(cJSON *message, MYSQL *connect);



void HandleUserRegister(cJSON *message, MYSQL *connect) {
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

    char UserRegisterSql[256];
    strcpy(UserRegisterSql, "insert into UserInfo (account, secret, nickname, sex, age) values (\"");
    strcat(UserRegisterSql, account);
    strcat(UserRegisterSql, "\", \"");
    strcat(UserRegisterSql, secret);
    strcat(UserRegisterSql, "\", \"");
    strcat(UserRegisterSql, nickname);
    strcat(UserRegisterSql, "\", \"");
    strcat(UserRegisterSql, sex);
    strcat(UserRegisterSql, "\", \"");
    strcat(UserRegisterSql, "18");
    strcat(UserRegisterSql, "\");");

    printf("SQL:%s\n", UserRegisterSql);

    if(mysql_query(connect, UserRegisterSql)){
        perror("查询失败");
    }else {
        printf("查询完毕....\n");
    }
}

#endif
