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
void HandleViewUserInfo(cJSON *message, MYSQL *connect, int fd);

void HandleViewUserInfo(cJSON *message, MYSQL *connect, int fd) {
    printf("HandleViewUserInfo\n");    
    cJSON *ret;
    MYSQL_RES *res;
    MYSQL_ROW row;

    ret = cJSON_CreateObject();
    char *account = cJSON_GetObjectItem(message, "account")->valuestring;

    char ViewUserInfoSql[256];
    char *retbuf;

    strcpy(ViewUserInfoSql, "select account, nickname, sex, age from UserInfo where account = \"");
    strcat(ViewUserInfoSql, account);
    strcat(ViewUserInfoSql, "\";");

    printf("SQL:%s\n", ViewUserInfoSql);
    
    if(mysql_query(connect, ViewUserInfoSql)) {
        printf("SQL 执行失败...\n");
    }else {
        printf("SQL 执行成功...\n");
    }

    if(!(res = mysql_store_result(connect))) {
        printf("获取结果失败\n");
    }else {
        printf("获取结果成功\n");
    }
    row = mysql_fetch_row(res);
    printf("account:%s\n", row[0]);
    printf("nickname:%s\n", row[1]);
    printf("sex:%s\n", row[2]);
    printf("age:%s\n", row[3]);
    cJSON_AddStringToObject(ret, "info", "用户信息如下：");
    cJSON_AddStringToObject(ret, "ret", "0");
    cJSON_AddStringToObject(ret, "account", account);
    cJSON_AddStringToObject(ret, "nickname", row[1]);
    cJSON_AddStringToObject(ret, "sex", row[2]);
    cJSON_AddStringToObject(ret, "age", row[3]);
    retbuf = cJSON_Print(ret);
    if(send(fd, retbuf, strlen(retbuf), 0) <= 0) {
        perror("send error");
    }
}

void HandleUserRegister(cJSON *message, MYSQL *connect, int fd) {
    cJSON *ret;

    ret = cJSON_CreateObject();
    char *account = cJSON_GetObjectItem(message, "account")->valuestring;
    char *secret = cJSON_GetObjectItem(message, "secret")->valuestring;
    char *nickname = cJSON_GetObjectItem(message, "nickname")->valuestring;
    char *sex = cJSON_GetObjectItem(message, "sex")->valuestring;
    uint16_t age = cJSON_GetObjectItem(message, "age")->valueint;

    char UserRegisterSql[256];
    char *retbuf;
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
        cJSON_AddNumberToObject(ret, "ret", 1);
        cJSON_AddStringToObject(ret, "value", "注册失败，请稍后再试...");
    }else {
        cJSON_AddNumberToObject(ret, "ret", 0);
        cJSON_AddStringToObject(ret, "value", "注册用户成功...请登录");
    }
    retbuf = cJSON_Print(ret);
    send(fd, retbuf, strlen(retbuf), 0);
}


void HandleUserLogin(cJSON *message, MYSQL *connect, int fd) {
    cJSON *ret;
    MYSQL_RES *res;
    MYSQL_ROW row;

    char *account = cJSON_GetObjectItem(message, "account")->valuestring;
    char *secret  = cJSON_GetObjectItem(message, "secret")->valuestring;

    char UserLoginSql[256];
    char *retbuf;

    ret = cJSON_CreateObject();
    
    bzero(UserLoginSql, 256);

    strcpy(UserLoginSql, "select count(*) from UserInfo where account = \"");
    strcat(UserLoginSql, account);
    strcat(UserLoginSql, "\" and secret = \"");
    strcat(UserLoginSql, secret);
    strcat(UserLoginSql, "\";");

    printf("SQL:%s\n", UserLoginSql);

    printf("HandleUserLogin\n");
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
    int retint = strncmp(row[0], "1", 1);
    printf("ret:%d\n", retint);
    if(strncmp(row[0], "1", 1) == 0) {
        cJSON_AddNumberToObject(ret, "ret", 0);
        cJSON_AddStringToObject(ret, "value", "登录成功，正在载入信息...请稍后");
        printf("query ok\n");
    }else {
        cJSON_AddNumberToObject(ret, "ret", 1);
        cJSON_AddStringToObject(ret, "value", "登录失败，帐号或密码不正确...");
        printf("query failed\n");
    }
    retbuf = cJSON_Print(ret);
    send(fd, retbuf, strlen(retbuf), 0);
}

#endif
