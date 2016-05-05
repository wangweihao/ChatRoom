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
            case 3:
                ViewMyInfo(fd);
                break;
            case 4:
                ShowAllFriend(fd);
                break;
            default:
                break;
        }
        choose = 0;
    }

}

void LoginInterface(int fd) {
    int choose;

    while (1) {
        system("clear");
        printf("----------------------------------------\n");
        printf("1.个人信息\n");
        printf("2.全部好友\n");
        printf("3.在线好友\n");
        printf("4.在线群组\n");
        printf("5.我的消息\n");
        printf("0.退出（返回主页）\n");
        printf("----------------------------------------\n");
        printf("\n\n\n\n\n请选择:");
        scanf("%d", &choose);
        switch(choose) {
            case 1:
                ViewMyInfo(fd);
                break;
            case 2:
                ShowAllFriend(fd);
                break;
            case 3:
                ShowLifeFriend(fd);
                break;
            case 4:
                ViewOnlineGroup(fd);
                break;
            case 5:
                UserMessage(fd);
                break;
            case 0:
                system("clear");
                return;
                break;
            default:
                printf("输入有误...请重新输入...\n");
                sleep(1);
                system("clear");
                break;
        } 

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
    UserInfo user;
    printf("请输入帐号:");
    scanf("%s", user.account);
    strcpy(myinfo.account, user.account);
    printf("请输入密码:");
    scanf("%s", user.secret);
    if(_UserLogin(&user, fd) == 0) {
        LoginInterface(fd);
    } else {
        system("clear");
        printf("login failed,wait...\n");
        sleep(2);
        return;
    }
}

/* UserLogin -> ViewMyInfo */

void ViewMyInfo(int fd) {
    printf("浏览个人信息\n");
    _ViewMyInfo(myinfo.account, fd);
    printf("\n\n\n\n按任意键返回上一层:\n");
    getchar();
    getchar();
    LoginInterface(fd);
}

/* UserLogin -> ViewMyInfo -> ShowAllFriend */
void ShowAllFriend(int fd) {
    printf("全部好友\n");
    _ShowAllFriend(myinfo.account, fd);
    printf("\n\n\n\n按任意键返回上一层:\n");
    getchar();
    getchar();
    LoginInterface(fd);
}

/* UserLogin -> ViewMyInfo -> ShowAllFriend -> AddFriend */
void AddFriend(int fd) {
    printf("添加好友\n");
}

/* UserLogin -> ViewMyInfo -> ShowAllFriend -> DeleteFriend */
void DeleteFriend(int fd) {
    printf("删除好友\n");
}

/* UserLogin -> ViewMyInfo -> ShowLifeFriend */
void ShowLifeFriend(int fd) {
    int choose;    

    printf("显示在线好友\n");
    int num = _ShowLifeFriend(myinfo.account, fd);
    if (num == 1) {
        printf("\n\n当前没有好友在线\n");
        printf("\n按任意键返回上一层\n\n");
        getchar();
        getchar();
        LoginInterface(fd);
    }else {
        printf("\n\n1.聊天\n");
        printf("0.返回上一层\n");
        printf("\n\n\n\n请选择:\n");
        scanf("%d", &choose);
        switch(choose) {
            case 1:
                _HandlerChat(myinfo.account, fd);
                break;
            case 0:
                LoginInterface(fd);
                break;
            default:
                LoginInterface(fd);
                break;
        }
    }
}

/* UserLogin -> ViewMyInfo -> ShowLifeFriend -> SelectOneFriendChat */
void SelectOneFriendChat(int fd) {
    printf("选择好友聊天\n");
}

/* UserLogin -> ViewOnlineGroup */
void ViewOnlineGroup(int fd) {
    int choose; 

    printf("浏览在线用户\n");
    _ViewOnlineGroup(fd);
    printf("1.加入群组\n");
    printf("2.创建群组\n");
    printf("0.返回上一级\n");
    printf("\n\n\n\n请选择:\n");
    scanf("%d", &choose);
    switch(choose) {
        case 1:
            JoinGroupChat(fd);
            break;
        case 2:
            CreateGroupChat(fd);
            break;
        case 0:
            LoginInterface(fd);
            break;
        default:
            LoginInterface(fd);
            break;
    }
}

/* UserLogin -> ViewOnlineGroup -> JoinGroupChat */
void JoinGroupChat(int fd) {
    char name[128];
    cJSON *info;

    printf("加入群聊\n");
    printf("请输入群名称:");
 
    bzero(name, 128);
    scanf("%s", name); 
    printf("加入群后，您将收到该群消息，输入\"quit\"退出!\n");
    sleep(1);
    printf("加入成功...请稍后");
    sleep(2);
    _JoinGroupChat(name, myinfo.account, fd);
    ViewOnlineGroup(fd);
}

/* UserLogin -> ViewOnlineGroup -> CreateGroupChat */
void CreateGroupChat(int fd) {
    printf("创建群聊天\n");
    _CreateGroupChat(myinfo.account, fd);
    printf("创建成功...\n");
    printf("\n\n\n\n按任意键返回上一层:\n");
    getchar();
    getchar();
    LoginInterface(fd);
}

/* UserLogin -> UserMessage */
void UserMessage(int fd) {
    printf("用户信息:");
    _UserMessage(myinfo.account, fd);

    HandlerMessage(fd);
    printf("\n\n\n\n按任意键返回上一层:\n");
    getchar();
    getchar();
    LoginInterface(fd);
}

/* UserLogin -> UserMessage -> HandlerMessage */
void HandlerMessage(int fd) {
    printf("处理消息\n");
    _HandlerMessage(myinfo.account, fd);
}


#endif
