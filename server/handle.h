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
#include <malloc.h>
#include <pthread.h>
#include "list.h"
#include "UserList.h"

#define MAXCON 100

typedef struct OnlinePeople {
    char name[128];
    int sockfd;
    int flag;
}OnlinePeople;

OnlinePeople onlinePeople[MAXCON];
int onlineCount = 0;

void HandleUserRegister(cJSON *message, MYSQL *connect, int fd);
void HandleUserLogin(cJSON *message, MYSQL *connect, int fd);
void HandleViewUserInfo(cJSON *message, MYSQL *connect, int fd);
void HandleShowAllFriend(cJSON *message, MYSQL *connect, int fd);
void HandleShowLifeFriend(cJSON *message, MYSQL *connect, int fd);
void HandleViewOnlineGroup(cJSON *message, MYSQL *connect, int fd);
void HandleCreateGroup(cJSON *message, MYSQL *connect, int fd);
void HandleUserMessage(cJSON *message, MYSQL *connect, int fd);
void HandleChat(cJSON *message, MYSQL *connect, int fd, User *head);
void HandleChatMessage(cJSON *message, MYSQL *connect, int fd, User *head);
void HandleJoinGroupChat(cJSON *message, MYSQL *connect, int fd);
void HandleGroupMessage(cJSON *message, MYSQL *connect, int fd);

/* 将消息转发给群组的每一个人，除本人外
 * input quit, recv quit */
void HandleGroupMessage(cJSON *message, MYSQL *connect, int fd) {
    printf("Handler Group Message\n");
    cJSON *ret;
    ret = cJSON_CreateObject();
    char *name = cJSON_GetObjectItem(message, "name")->valuestring;
    char *account = cJSON_GetObjectItem(message, "account")->valuestring;
    char *info = cJSON_GetObjectItem(message, "info")->valuestring;
    int index = 0;
    cJSON_AddStringToObject(ret, "info", info);
    cJSON_AddStringToObject(ret, "account", account);
    char *buffer = cJSON_Print(ret);
    for (; index < MAXGROUP; ++index) {
        if (strcmp(onlineGroup.group[index].name, name) == 0) {
            struct Node *head = NULL;
            head = onlineGroup.group[index].head;
            while (head != NULL) {
                send(head->sockfd, buffer, strlen(buffer), 0);
                head = head->next;
            }
            break;
        }
    }
}

/* 将用户加入 OnlineGroup 里*/
void HandleJoinGroupChat(cJSON *message, MYSQL *connect, int fd) {
    char *account = cJSON_GetObjectItem(message, "account")->valuestring;
    char *name = cJSON_GetObjectItem(message, "name")->valuestring;
    int index = 0;
    printf("Handler Join Group Chat\n");
    for (; index < MAXGROUP; ++index) {
        if (strcmp(onlineGroup.group[index].name, name) == 0) {
            break;
        }
    }
    printf("index:%d\n", index);
    printf("1\n");
    if (index == MAXGROUP) {
        printf("not found...\n");
    }else {
        if (SearchGroup(onlineGroup.group[index].head, account) == 0) {
            printf("2\n");
            struct Node *node = (struct Node*)malloc(sizeof(struct Node));
            strcpy(node->account, account);
            node->sockfd = fd;
            node->next = NULL;
            printf("3\n");
            AddGroup(onlineGroup.group[index].head, node);
            printf("4\n");
        } 
    }
}

//-------------------------------------
void HandleChatMessage(cJSON *message, MYSQL *connect, int fd, User *head) {
    printf("Handle Chat Message\n");
    cJSON *ret;

    char *name = cJSON_GetObjectItem(message, "name")->valuestring;
    char *account = cJSON_GetObjectItem(message, "account")->valuestring;
    char *info = cJSON_GetObjectItem(message, "info")->valuestring;
 
    if (strcmp("quit", info) == 0) {
        QuitUserList(head, account);
    }
    ret = cJSON_CreateObject();
    User* one = SearchUser(head, name);
    if (one == NULL) {
        printf("对方不在线！\n");
        /* 对方不在线，退出 */
        cJSON_AddStringToObject(ret, "info", "quit");
    }else {
        cJSON_AddStringToObject(ret, "info", info);
    }
    cJSON_AddStringToObject(ret, "account", account);
    char* buffer = cJSON_Print(ret);
    size_t length = strlen(buffer);
    ssize_t size = send(one->socket, buffer, length, 0);
    if (length == size) {
        printf("%s->%s 发送消息成功\n", name, account);
    }else {
        printf("%s->%s 发送消息失败\n", name, account);
    }
}

void HandleChat(cJSON *message, MYSQL *connect, int fd, User *head) {
    printf("Handle Chat\n");
    char *account = cJSON_GetObjectItem(message, "account")->valuestring;
    AddUserList(head, account, fd);
}

void HandleUserMessage(cJSON *message, MYSQL *connect, int fd) {
    char *account = cJSON_GetObjectItem(message, "account")->valuestring;
    cJSON *ret;
    cJSON *array;

    char GetUserIdSql[128];
    char GetUserAccountSql[128];
    char GetUserMessageSql[128];
    MYSQL_RES *res;
    MYSQL_ROW row;
    
    MYSQL_RES *ress;
    MYSQL_ROW rows;

    ret = cJSON_CreateObject();
    array = cJSON_CreateObject();

    strcpy(GetUserIdSql, "select uid from UserInfo where account = \"");
    strcat(GetUserIdSql, account);
    strcat(GetUserIdSql, "\";");

    if(mysql_query(connect, GetUserIdSql)) {
        printf("查询失败...\n");
    }else {
        printf("查询成功...\n");
    }

    if(!(res = mysql_store_result(connect))) {
        printf("获取结果失败...\n");
    }else {
        printf("获取结果成功...\n");
    }
    

    row = mysql_fetch_row(res);
    printf("%s\n", row[0]);
    
    strcpy(GetUserMessageSql, "select friendId from UserMessage where uid = \"");
    strcat(GetUserMessageSql, row[0]);
    strcat(GetUserMessageSql, "\";");

    if(mysql_query(connect, GetUserMessageSql)) {
        printf("查询失败...\n");
    }else {
        printf("查询成功...\n");
    }

    if(!(ress = mysql_store_result(connect))) {
        printf("获取结果失败...\n");
    }else {
        printf("获取结果成功...\n");
    }
    int retinfo[128];
    int number = 0;
    while(rows = mysql_fetch_row(ress)) {
        
        number++;
        MYSQL_RES *rest;
        MYSQL_ROW rowt;

        cJSON *one;
        one = cJSON_CreateObject();
        strcpy(GetUserAccountSql, "select account, nickname from UserInfo where uid = \"");
        strcat(GetUserAccountSql, rows[0]);
        strcat(GetUserAccountSql, "\";");
        if(mysql_query(connect, GetUserAccountSql)) {
            printf("查询消息信息失败...\n");
        }else {
            printf("查询消息信息成功...\n");
        }
        if(!(rest = mysql_store_result(connect))) {
            printf("获取结果失败...\n");
        }else {
            printf("获取结果成功...\n");
        }

        rowt = mysql_fetch_row(rest);
        char buf[128];
        printf("rowt[0]:%s\n", rowt[0]);
        printf("rowt[1]:%s\n", rowt[1]);
        strcpy(buf, "帐号:");
        strcat(buf, rowt[0]);
        strcat(buf, " 用户名:");
        strcat(buf, rowt[1]);
        strcat(buf, " 请求添加您为好友");
        cJSON_AddStringToObject(one, "msg", buf);
        cJSON_AddItemToArray(array, one);
        bzero(buf, 128);
        bzero(GetUserAccountSql, 128);
    }
    cJSON_AddNumberToObject(ret, "number", number);
    cJSON_AddItemToObject(ret, "message", array);
    printf("retJSON:%s\n", cJSON_Print(ret));

    char *buffer = cJSON_Print(ret);

    size_t length = strlen(buffer);
    ssize_t size = send(fd, buffer, length, 0);
    if(length == size) {
        printf("查询用户消息成功...\n");
    }else {
        printf("查询用户消息失败...\n");
    }
}

void HandleCreateGroup(cJSON *message, MYSQL *connect, int fd) {
    char *account = cJSON_GetObjectItem(message, "account")->valuestring;
    char *name = cJSON_GetObjectItem(message, "name")->valuestring;
    cJSON *ret;
    struct Node* one;

    ret = cJSON_CreateObject();
    one = (struct Node*)malloc(sizeof(struct Node));
    if(one == NULL) {
        perror("malloc error!");
        return;
    }
    strcpy(one->account, account);
    one->sockfd = fd;
    one->next = NULL;
    pthread_mutex_lock(&onlineGroup.mutex);
    strcpy(onlineGroup.group[onlineGroup.groupnum].name, name);
    onlineGroup.group[onlineGroup.groupnum].number = 1;
    AddGroup(onlineGroup.group[onlineGroup.groupnum].head, one);
    onlineGroup.groupnum++;
    pthread_mutex_unlock(&onlineGroup.mutex);
    cJSON_AddNumberToObject(ret, "ret", 0);
    cJSON_AddStringToObject(ret, "info", "创建群组成功");
    char *buffer = cJSON_Print(ret);

    size_t length = strlen(buffer);
    ssize_t size = send(fd, buffer, length, 0);
    if(length == size) {
        printf("创建群组成功...\n");
    }else {
        printf("创建群组失败...\n");
    }
}

void HandleViewOnlineGroup(cJSON *message, MYSQL *connect, int fd) {
    printf("HandleViewUserInfo\n");
    printf("message:%s\n", cJSON_Print(message));
    cJSON *allGroup;
    cJSON *oneGroup;
    cJSON *ret;
    char *retbuffer;
    int num = onlineGroup.groupnum;
    int i;

    if(num == 0) {
        ret = cJSON_CreateObject();
        cJSON_AddNumberToObject(ret, "number", 0);
    }else {
        ret = cJSON_CreateObject();
        cJSON_AddNumberToObject(ret, "number", num);
        allGroup = cJSON_CreateArray();
        for(i = 0; i < num; ++i) {
            oneGroup = cJSON_CreateObject();
            cJSON_AddStringToObject(oneGroup, "name", onlineGroup.group[i].name);
            cJSON_AddNumberToObject(oneGroup, "number", onlineGroup.group[i].number);
            cJSON_AddItemToArray(allGroup, oneGroup);
        }
        cJSON_AddItemToObject(ret, "group", allGroup);
    }
    printf("group:%s\n", cJSON_Print(ret));

    retbuffer = cJSON_Print(ret);
    if(send(fd, retbuffer, strlen(retbuffer), 0) <= 0) {
        perror("send error");
    }
}

void HandleShowLifeFriend(cJSON *message, MYSQL *connect, int fd) {
    printf("HandleShowLifeFriend\n");
    cJSON *ret;
    cJSON *allfriend;
    cJSON *friendarray;
    MYSQL_RES *res;
    MYSQL_ROW row;

    char GetUserUidSql[256];
    char GetUserFriendInfo[256];

    ret = cJSON_CreateObject();
    allfriend = cJSON_CreateArray();
    friendarray = cJSON_CreateObject();
    char *account = cJSON_GetObjectItem(message, "account")->valuestring;
    strcpy(GetUserUidSql, "select uid from UserInfo where account = \"");
    strcat(GetUserUidSql, account);
    strcat(GetUserUidSql, "\";");
    printf("GetUserUidSql:%s\n", GetUserUidSql);
    if(mysql_query(connect, GetUserUidSql)) {
        printf("查询失败...\n");
    }else {
        printf("查询成功...\n");
    }

    if(!(res = mysql_store_result(connect))) {
        printf("获取结果失败...\n");
    }else {
        printf("获取结果成功...\n");
    }

    row = mysql_fetch_row(res);
    printf("%s\n", row[0]);

    strcpy(GetUserFriendInfo, "select nickname from UserFriend where uid = \"");
    strcat(GetUserFriendInfo, row[0]);
    strcat(GetUserFriendInfo, "\" and online = 1;");
    printf("GetUserFriendInfo:%s\n", GetUserFriendInfo);
    if(mysql_query(connect, GetUserFriendInfo)) {
        printf("查询全部好友失败...\n");
    }else {
        printf("查询全部好友成功...\n");
    }
    if(!(res = mysql_store_result(connect))) {
        printf("获取结果失败...\n");
    }else {
        printf("获取结果成功...\n");
    }

    while(row = mysql_fetch_row(res)) {
        cJSON *onefriend;
        onefriend = cJSON_CreateObject();
        printf("nickname:%s\n", row[0]);
        cJSON_AddStringToObject(onefriend, "nickname", row[0]);
        cJSON_AddItemToArray(allfriend, onefriend);
    }
    cJSON_AddItemToObject(friendarray, "friend", allfriend);
    char *friendbuf = cJSON_Print(friendarray);
    printf("Json All Friend:%s\n", friendbuf);
    if(send(fd, friendbuf, strlen(friendbuf), 0) <= 0) {
        perror("send error");
    }
}

void HandleShowAllFriend(cJSON *message, MYSQL *connect, int fd) {
    printf("HandleShowAllFriend\n");
    cJSON *ret;
    cJSON *allfriend;
    cJSON *friendarray;
    MYSQL_RES *res;
    MYSQL_ROW row;

    char GetUserUidSql[256];
    char GetUserFriendInfo[256];

    ret = cJSON_CreateObject();
    allfriend = cJSON_CreateArray();
    friendarray = cJSON_CreateObject();
    char *account = cJSON_GetObjectItem(message, "account")->valuestring;
    strcpy(GetUserUidSql, "select uid from UserInfo where account = \"");
    strcat(GetUserUidSql, account);
    strcat(GetUserUidSql, "\";");
    printf("GetUserUidSql:%s\n", GetUserUidSql);
    if(mysql_query(connect, GetUserUidSql)) {
        printf("查询失败...\n");
    }else {
        printf("查询成功...\n");
    }

    if(!(res = mysql_store_result(connect))) {
        printf("获取结果失败...\n");
    }else {
        printf("获取结果成功...\n");
    }

    row = mysql_fetch_row(res);
    printf("%s\n", row[0]);

    strcpy(GetUserFriendInfo, "select nickname from UserFriend where uid = \"");
    strcat(GetUserFriendInfo, row[0]);
    strcat(GetUserFriendInfo, "\";");
    printf("GetUserFriendInfo:%s\n", GetUserFriendInfo);
    if(mysql_query(connect, GetUserFriendInfo)) {
        printf("查询全部好友失败...\n");
    }else {
        printf("查询全部好友成功...\n");
    }
    if(!(res = mysql_store_result(connect))) {
        printf("获取结果失败...\n");
    }else {
        printf("获取结果成功...\n");
    }

    while(row = mysql_fetch_row(res)) {
        cJSON *onefriend;
        onefriend = cJSON_CreateObject();
        printf("nickname:%s\n", row[0]);
        cJSON_AddStringToObject(onefriend, "nickname", row[0]);
        cJSON_AddItemToArray(allfriend, onefriend);
    }
    cJSON_AddItemToObject(friendarray, "friend", allfriend);
    char *friendbuf = cJSON_Print(friendarray);
    printf("Json All Friend:%s\n", friendbuf);
    if(send(fd, friendbuf, strlen(friendbuf), 0) <= 0) {
        perror("send error");
    }

}


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
        if (onlineCount == MAXCON) {
            onlineCount = 0;
        }else {
            onlinePeople[onlineCount].sockfd = fd;
            onlinePeople[onlineCount].flag = 1;
            strcpy(onlinePeople[onlineCount].name, account);
        }
    }else {
        cJSON_AddNumberToObject(ret, "ret", 1);
        cJSON_AddStringToObject(ret, "value", "登录失败，帐号或密码不正确...");
        printf("query failed\n");
    }
    retbuf = cJSON_Print(ret);
    send(fd, retbuf, strlen(retbuf), 0);

}

#endif
