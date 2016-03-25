/*======================================================
    > File Name: UserInfo.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月16日 星期三 09时08分50秒
 =======================================================*/

#ifndef _USERINFO_H_
#define _USERINFO_H_

#include <stdint.h>
#include <string.h>

#define MAXGROUP 100

typedef struct UserInfo {
    char account[20];
    char secret[20];
    char nickname[20];
    char sex[4];
    uint16_t age;
}UserInfo;

/* Message form
 * type = 1 UserRegister*/
typedef struct Message {
    uint16_t type;
    uint16_t length;
    char msg[256];
} Message;

typedef struct Group {
    char name[40];
    struct Node *head;
    int number;
} Group;


typedef struct OnlineGroup {
    Group group[MAXGROUP];
    int groupnum;
    pthread_mutex_t mutex;
} OnlineGroup;

OnlineGroup onlineGroup;

#endif
