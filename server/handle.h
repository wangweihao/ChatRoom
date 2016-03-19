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


void HandleUserRegister(cJSON *message, MYSQL *connect, int fd);
void HandleUserLogin(cJSON *message, MYSQL *connect, int fd);


void HandleUserRegister(cJSON *message, MYSQL *connect, int fd) {
    char *account = cJSON_GetObjectItem(message, "account")->valuestring;
    char *secret = cJSON_GetObjectItem(message, "secret")->valuestring;
    char *nickname = cJSON_GetObjectItem(message, "nickname")->valuestring;
    char *sex = cJSON_GetObjectItem(message, "sex")->valuestring;
    uint16_t age = cJSON_GetObjectItem(message, "age")->valueint;

    char UserRegisterSql[256];
    char retbuf[256];
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
        strcpy(retbuf, "注册失败，请稍后再试...");
    }else {
        strcpy(retbuf, "注册用户信息成功...");
    }
    send(fd, retbuf, strlen(retbuf), 0);
}


void HandleUserLogin(cJSON *message, MYSQL *connect, int fd) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    char *account = cJSON_GetObjectItem(message, "account")->valuestring;
    char *secret  = cJSON_GetObjectItem(message, "secret")->valuestring;

    char UserLoginSql[256];
    char retbuf[256];

    bzero(UserLoginSql, 256);
    bzero(retbuf, 256);

    strcpy(UserLoginSql, "select count(*) from UserInfo where account = \"");
    strcat(UserLoginSql, account);
    strcat(UserLoginSql, "\" and secret = \"");
    strcat(UserLoginSql, secret);
    strcat(UserLoginSql, "\";");

    printf("SQL:%s\n", UserLoginSql);

    if(mysql_query(connect, UserLoginSql)) {
        printf("SQL 执行失败...\n");
    }else {
        printf("SQL 执行成功...\n");
    }
    
    if(!(res = mysql_store_result(connect))) {
        printf("获取结果失败\n");
        return;
    }else {
        printf("获取结果成功\n");
    }
    int num = mysql_num_fields(res);
    printf("number:%d\n", num);
    row = mysql_fetch_row(res);
    printf("row:%s\n", row[0]);
    int ret = strncmp(row[0], "1", 1);
    printf("ret:%d\n", ret);
    if(strncmp(row[0], "1", 1) == 0) {
        printf("query ok\n");
        strcpy(retbuf, "login success.");
    }else {
        printf("query failed\n");
        strcpy(retbuf, "login failed..");
    }
    send(fd, retbuf, strlen(retbuf), 0);
}

#endif
