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

typedef struct UserInfo {
    char account[20];
    char secret[20];
    char nickname[20];
    char sex[4];
    uint64_t age;
}UserInfo;

#endif