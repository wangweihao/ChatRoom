/*======================================================
    > File Name: interface.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月15日 星期二 23时37分34秒
 =======================================================*/

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void MainInterface(); 
void UserRegister();
void UserLogin();
void ViewMyInfo();
void ShowAllFriend();
void AddFriend();
void DeleteFriend();
void ShowLifeFriend();
void SelectOneFriendChat();
void ViewOnlineGroup();
void JoinGroupChat();
void CreateGroupChat();
void UserMessage();
void HandlerMessage();

    
void MainInterface() {
    int choose;

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
                UserLogin();
                break;
            case 2:
                UserRegister();
                break;
            default:
                break;
        }
    }

}

/* UserRegister module */

void UserRegister() {
    printf("输入帐号:");
    printf("输入密码:");
    printf("输入昵称:");
    printf("输入性别:");
    printf("输入年龄:");
}

/* UserRegister module -> FillPersonInfo */

void FillPersonInfo() {

}

/* UserLogin module */

void UserLogin() {

}

/* UserLogin -> ViewMyInfo */

void ViewMyInfo() {
    printf("浏览个人信息:");
}

/* UserLogin -> ViewMyInfo -> ShowAllFriend */
void ShowAllFriend() {

}

/* UserLogin -> ViewMyInfo -> ShowAllFriend -> AddFriend */
void AddFriend() {

}

/* UserLogin -> ViewMyInfo -> ShowAllFriend -> DeleteFriend */
void DeleteFriend() {

}

/* UserLogin -> ViewMyInfo -> ShowLifeFriend */
void ShowLifeFriend() {

}

/* UserLogin -> ViewMyInfo -> ShowLifeFriend -> SelectOneFriendChat */
void SelectOneFriendChat() {

}

/* UserLogin -> ViewOnlineGroup */
void ViewOnlineGroup() {
    printf("浏览在线用户:");
}

/* UserLogin -> ViewOnlineGroup -> JoinGroupChat */
void JoinGroupChat() {

}

/* UserLogin -> ViewOnlineGroup -> CreateGroupChat */
void CreateGroupChat() {

}

/* UserLogin -> UserMessage */
void UserMessage() {
    printf("用户信息:");
}

/* UserLogin -> UserMessage -> HandlerMessage */
void HandlerMessage() {

}

#endif
