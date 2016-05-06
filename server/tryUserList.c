/*======================================================
    > File Name: tryUserList.c
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年05月06日 星期五 14时35分25秒
 =======================================================*/


#include "UserList.h"

int main(int argc, char **argv) {
    User *head = CreateUserList();
    AddUserList(head, "wang", 1);
    AddUserList(head, "wei", 2);
    AddUserList(head, "hao", 3);
    PrintUserList(head);
    QuitUserList(head, "wei");
    QuitUserList(head, "wei");
    QuitUserList(head, "wei");
    PrintUserList(head);
    User *p = SearchUser(head, "hao");
    printf("name:%s, socket:%d\n", p->name, p->socket);
}
