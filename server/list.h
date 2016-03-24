/*======================================================
    > File Name: list.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月24日 星期四 23时40分53秒
 =======================================================*/


#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

struct Node {
    char account[40];
    uint16_t sockfd;
    struct Node *next;
};

struct Node* CreateGroup();
void AddGroup(struct Node* head, struct Node* node);
void QuitGroup(struct Node* head, char *account);
void PrintGroup(struct Node* head);

struct Node* CreateGroup() {
    struct Node* head = (struct Node*)malloc(sizeof(struct Node));
    if(head == NULL) {
        perror("create head error");
        exit(1);
    }
    return head;
} 

void AddGroup(struct Node* head, struct Node* node) {
    struct Node* phead = head;
    if(head == NULL) {
        return;
    }   
    while(phead->next != NULL) {
        phead = phead->next;
    }
    phead->next = node;
    node->next = NULL;
}

void QuitGroup(struct Node* head, char *account) {
    struct Node * pre, *phead;
    if(head == NULL) {
        return;
    }   
    phead = head->next;
    pre = head;
    while(phead != NULL) {
        if(strcmp(phead->account, account) == 0) {
            pre->next = phead->next;
            free(phead);
            return;   
        }
        pre = phead;
        phead = phead->next;
    }   
}

void PrintGroup(struct Node* head) {
    struct Node* phead = head->next;
    if(head == NULL) {
        return;
    }
    while(phead != NULL) {
        printf("%s\n", phead->account);
        printf("%d\n", phead->sockfd);
        phead = phead->next;
    }
}

#endif
