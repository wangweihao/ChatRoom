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


int _RegisterUserInfo(UserInfo *user, int fd) {
    printf("%s\n", user->account);
    printf("%s\n", user->nickname);
}

