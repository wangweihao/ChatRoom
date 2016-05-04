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
int _ShowLifeFriend(char *account, int fd);
int _ViewOnlineGroup(int fd);
int _CreateGroupChat(char *account, int fd);
int _UserMessage(char *account, int fd);
int _HandlerMessage(char *account, int fd);
int _HandlerChat(char *account, int fd);

int _HandlerChat(char *account, int fd) {
    cJSON *info;
    char name[128];
    char *buffer;

            
    printf("聊天开始\n");
    printf("输入\"quit\"退出\n");
    printf("请输入聊天好友姓名:");
    bzero(name, 128);
    scanf("%s", name);

    info = cJSON_CreateObject();
    cJSON_AddNumberToObject(info, "type", 10);
    cJSON_AddStringToObject(info, "account", account);
    cJSON_AddStringToObject(info, "friend", name);
    buffer = cJSON_Print(info);

    size_t length = strlen(buffer);
    ssize_t size = send(fd, buffer, length, 0);
    if(size == length) {
        printf("send success!\n");
    }else {
        printf("send error!\n");          
    }

    ChatAndOneFriend(account, name, fd);
}

int _HandlerMessage(char *account, int fd) {
    printf("处理消息：_HandlerMessage\n");
}

int _UserMessage(char *account, int fd) {
    cJSON *info;
    char *buffer;
    char retbuffer[1024];

    info = cJSON_CreateObject();
    cJSON_AddNumberToObject(info, "type", 8);
    cJSON_AddStringToObject(info, "account", account);
    buffer = cJSON_Print(info);
    //printf("sendjson:%s\n", buffer);

    size_t length = strlen(buffer);
    ssize_t size = send(fd, buffer, length, 0);
    if(size == length) {
        printf("send success!\n");
    }else {
        printf("send error!\n");
    }
    
    if(recv(fd, retbuffer, 1024, 0) <= 0) {
        printf("系统错误，请稍后再试...\n");
    }else {
        //printf("接受JSON:%s\n", retbuffer);
        cJSON *rec = cJSON_Parse(retbuffer);
        int num = cJSON_GetObjectItem(rec, "number")->valueint;
        if(num == 0) {
            printf("暂时无最新消息...\n");
            return 0;
        }
        cJSON *mesg = cJSON_GetObjectItem(rec, "message");
        cJSON *list = mesg->child;
        int i = 1;
        while(list != NULL) {
            printf("(%d).%s\n", i, cJSON_GetObjectItem(list, "msg")->valuestring);
            list = list->next;
            i++;
        }
        printf("\n\n\n");
    }
}

int _CreateGroupChat(char *account, int fd) {
    cJSON *info;
    char* buffer;
    char name[40];
    char retbuffer[1024];

    bzero(name, 0);
    bzero(retbuffer, 0);
    printf("请输入群组名字:");
    scanf("%s", name);
    info = cJSON_CreateObject();
    cJSON_AddNumberToObject(info, "type", 7);
    cJSON_AddStringToObject(info, "account", account);
    cJSON_AddStringToObject(info, "name", name);
    buffer = cJSON_Print(info);
    printf("sendjson:%s\n", buffer);

    size_t length = strlen(buffer);
    ssize_t size = send(fd, buffer, length, 0);
    if(size == length) {
        printf("send success!\n");
    }else {
        printf("send error!\n");
    }

    if(recv(fd, retbuffer, 1024, 0) <= 0) {
        printf("系统错误...请稍后再试\n");
        return -1;
    }else {
        printf("recv success:%s\n", retbuffer);
        cJSON *rec = cJSON_Parse(retbuffer);
        int ret = cJSON_GetObjectItem(rec, "ret")->valueint;
        if(ret == 0) {
            printf("%s\n", cJSON_GetObjectItem(rec, "info")->valuestring);
        }
    }

}

int _ViewOnlineGroup(int fd) {
    cJSON *info;
    cJSON *retmsg;
    char *buffer;
    char retbuffer[1024];

    bzero(retbuffer, 0);
    info = cJSON_CreateObject();
    cJSON_AddNumberToObject(info, "type", 6);
    
    buffer = cJSON_Print(info);

    size_t length = strlen(buffer);
    ssize_t size = send(fd, buffer, length, 0);
    if(length == size) {
        printf("send success!\n");
    }else {
        printf("send failure!\n");
    }

    if(recv(fd, retbuffer, 1024, 0) <= 0) {
        printf("系统错误...请稍后再试...\n");
    }else {
        printf("recv success:%s\n", retbuffer);
        cJSON *rec = cJSON_Parse(retbuffer);
        int num = cJSON_GetObjectItem(rec, "number")->valueint;
        system("clear");
        if(num == 0) {
            printf("\n群组 \n\n");
        }else {
            cJSON *group;
            int i = 1;
            group = cJSON_GetObjectItem(rec, "group");
            cJSON *list = group->child;
            printf("\n群组 \n\n\n\n");
            while(list != NULL) {
                printf("(%d).name:%20s  people:%d\n", i, cJSON_GetObjectItem(list, "name")->valuestring, cJSON_GetObjectItem(list, "number")->valueint);
                list = list->next;
                i++;
            }
            printf("\n\n\n");
        }
    }
}

int _ShowLifeFriend(char *account, int fd) {
    cJSON *userinfo;
    cJSON *retmsg;
    char *buffer;
    char retbuffer[1024];
    int ret = 0;

    bzero(retbuffer, 1024);
    userinfo = cJSON_CreateObject();
    cJSON_AddStringToObject(userinfo, "account", account);
    cJSON_AddNumberToObject(userinfo, "type", 5);

    buffer = cJSON_Print(userinfo);

    size_t length = strlen(buffer);
    ssize_t size = send(fd, buffer, length, 0);
    if(length == size) {
        printf("send success!!\n");
    }else {
        printf("send error!\n");
    }

    printf("233333\n");
    if(recv(fd, retbuffer, 1024, 0) <= 0) {
        printf("系统错误...请稍后再试...\n");
    }else {
        printf("233333\n");
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
        printf(" 在线好友\n\n\n");
        int i = 1;
        while(arraylist != NULL) {
            char *tname = cJSON_GetObjectItem(arraylist, "nickname")->valuestring;
            if (strcmp(tname, account) != 0) {
                printf("%d.%s\n", i, tname);
                ++i;
            }
            arraylist = arraylist->next;
        }
        if (i == 1) {
            return 1;
        }else {
            return 0;
        }
    }
}

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



