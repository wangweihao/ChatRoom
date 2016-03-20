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
int _ViewMyInfo(char *account, int fd);
int _ShowAllFriend(char *account, int fd);

int _ShowAllFriend(char *account, int fd) {
    cJSON *userinfo;
    cJSON *retmsg;
    char *buffer;
    char retbuffer[1024];
    int ret = 0;

    bzero(retbuffer, 1024);
    userinfo = cJSON_CreateObject();
    cJSON_AddStringToObject(userinfo, "account", account);
    cJSON_AddNumberToObject(userinfo, "type", 4);

    buffer = cJSON_Print(userinfo);

    size_t length = strlen(buffer);
    ssize_t size = send(fd, buffer, length, 0);
    if(length == size) {
        printf("send success!\n");
    }else {
        printf("send error!\n");
    }

    if(recv(fd, retbuffer, 1024, 0) <= 0) {
        printf("系统错误...请稍后再试...\n");
    }else {
        //printf("retbuffer:%s\n", retbuffer);       
        cJSON *friendarray;
        cJSON *info = cJSON_Parse(retbuffer);
        int arraysize;
        /* 获取 JSON 数组 */
        friendarray = cJSON_GetObjectItem(info, "friend");
        /* 获取数组大小 */
        arraysize = cJSON_GetArraySize(friendarray);
        cJSON *arraylist = friendarray->child;
        system("clear");
        printf(" 我的好友\n\n\n");
        int i = 1;
        while(arraylist != NULL) {
            printf("%d.%s\n", i, cJSON_GetObjectItem(arraylist, "nickname")->valuestring);
            arraylist = arraylist->next;
            ++i;
        }
    }
}

/* type 3 view self infomation */
int _ViewMyInfo(char *account, int fd) {
    cJSON *userinfo;
    cJSON *retmsg;
    char *buffer;
    char retbuffer[256];
    int ret = 0;
    
    userinfo = cJSON_CreateObject();
    cJSON_AddStringToObject(userinfo, "account", account);
    cJSON_AddNumberToObject(userinfo, "type", 3);

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
            printf("获取信息成功...\n");
            system("clear");
            printf("Info:%s\n", cJSON_GetObjectItem(retmsg, "info")->valuestring);
            printf("account:%s\n", cJSON_GetObjectItem(retmsg, "account")->valuestring);
            printf("nickname:%s\n", cJSON_GetObjectItem(retmsg, "nickname")->valuestring);
            printf("sex:%s\n", cJSON_GetObjectItem(retmsg, "sex")->valuestring);
            printf("age:%s\n", cJSON_GetObjectItem(retmsg, "age")->valuestring);
        }else {
            printf("获取信息失败\n...");
        }
    }
}

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



