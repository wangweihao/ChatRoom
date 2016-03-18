/*======================================================
    > File Name: interface.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月15日 星期二 23时37分34秒
 =======================================================*/

#ifndef _INTERFACE_H_
#define _INTERFACE_H_


/* c header */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>

/* my header */
#include "UserInfo.h"
/* decoder/encoder */
#include "tool.h"   


void MainInterface(int fd); 
void UserRegister(int fd);
void UserLogin(int fd);
void ViewMyInfo(int fd);
void ShowAllFriend(int fd);
void AddFriend(int fd);
void DeleteFriend(int fd);
void ShowLifeFriend(int fd);
void SelectOneFriendChat(int fd);
void ViewOnlineGroup(int fd);
void JoinGroupChat(int fd);
void CreateGroupChat(int fd);
void UserMessage(int fd);
void HandlerMessage(int fd);

void MainInterface(int fd) {
    int choose;

    system("clear");
    while (1) {
        printf("----------------------------------------\n");
        printf("欢迎使用 ChatRoom \n");
        printf("1.登录\n");
        printf("2.注册\n");
        printf("0.退出\n");
        printf("----------------------------------------\n");
        printf("\n\n\n\n\n请选择:");
        scanf("%d", &choose);
        if (choose == 1 || choose == 2) {
            
        }else if (choose == 0) {
            system("clear");
            printf("谢谢使用！！再见\n");
            sleep(2);
            break;
        }else {
            system("clear");
            printf("选择有误，请稍后再试...\n");
            sleep(1);
            continue;
        }
        switch (choose) {
            case 1:
                UserLogin(fd);
                break;
            case 2:
                UserRegister(fd);
                break;
            default:
                break;
        }
        choose = 0;
    }

}

/* UserRegister module */

void UserRegister(int fd) {
    UserInfo user;
    char tsecret[20];
    uint16_t tsex;
    uint16_t select_sex;

    printf("输入帐号:");
    scanf("%s", user.account);
    while (1){
        printf("输入密码:");
        scanf("%s", user.secret);
        printf("请再次输入密码:");
        scanf("%s", tsecret);
        if (strncmp(user.secret, tsecret, 20) == 0) {
            break;
        }else {
            bzero(user.secret, 20);
            bzero(tsecret, 20);
        }
    }
    printf("输入昵称:");
    scanf("%s", user.nickname);
    while (1){
        printf("请选择性别 1.男  2.女:\n");
        scanf("%hd", &tsex);
        switch (tsex) {
            case 1:
                strncpy(user.sex, "男", 4);
                select_sex = 1;
                break;
            case 2:
                strncpy(user.sex, "女", 4);
                select_sex = 1;
                break;
            default:
                select_sex = 0;
                break;
        }
        if (select_sex == 1) {
            break;
        }else {
            printf("选择有误，请重试...\n");
            continue;
        }
    }
    printf("输入年龄:");
    scanf("%hd", &user.age);
    system("clear");
    printf("注册成功...点击任意键返回主菜单...\n");
    _RegisterUserInfo(&user, fd);
    getchar();
    getchar();
}

/* UserRegister module -> FillPersonInfo */

void FillPersonInfo(int fd) {

}

/* UserLogin module */

void UserLogin(int fd) {
    char account[20];
    char secret[20];
    printf("请输入帐号:");
    scanf("%s", account);
    printf("请输入密码:");
    scanf("%s", secret);
}

/* UserLogin -> ViewMyInfo */

void ViewMyInfo(int fd) {
    printf("浏览个人信息:");
}

/* UserLogin -> ViewMyInfo -> ShowAllFriend */
void ShowAllFriend(int fd) {

}

/* UserLogin -> ViewMyInfo -> ShowAllFriend -> AddFriend */
void AddFriend(int fd) {

}

/* UserLogin -> ViewMyInfo -> ShowAllFriend -> DeleteFriend */
void DeleteFriend(int fd) {

}

/* UserLogin -> ViewMyInfo -> ShowLifeFriend */
void ShowLifeFriend(int fd) {

}

/* UserLogin -> ViewMyInfo -> ShowLifeFriend -> SelectOneFriendChat */
void SelectOneFriendChat(int fd) {

}

/* UserLogin -> ViewOnlineGroup */
void ViewOnlineGroup(int fd) {
    printf("浏览在线用户:");
}

/* UserLogin -> ViewOnlineGroup -> JoinGroupChat */
void JoinGroupChat(int fd) {

}

/* UserLogin -> ViewOnlineGroup -> CreateGroupChat */
void CreateGroupChat(int fd) {

}

/* UserLogin -> UserMessage */
void UserMessage(int fd) {
    printf("用户信息:");
}

/* UserLogin -> UserMessage -> HandlerMessage */
void HandlerMessage(int fd) {

}


#endif
