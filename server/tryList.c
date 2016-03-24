/*======================================================
    > File Name: tryList.c
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月25日 星期五 00时07分17秒
 =======================================================*/


#include "list.h"

int main() {
    struct Node* head = CreateGroup();
    struct Node *p = (struct Node*)malloc(sizeof(struct Node));
    p->sockfd = 1;
    strcpy(p->account, "wangweihao");
    struct Node *p2 = (struct Node*)malloc(sizeof(struct Node));
    p2->sockfd = 1;
    strcpy(p2->account, "wangwe");
    struct Node *p3 = (struct Node*)malloc(sizeof(struct Node));
    p3->sockfd = 1;
    strcpy(p3->account, "wang");
    AddGroup(head, p);
    AddGroup(head, p2);
    AddGroup(head, p3);
    PrintGroup(head);
    QuitGroup(head, "wangwe");
    printf("quit...\n");
    PrintGroup(head);
    
    
}
