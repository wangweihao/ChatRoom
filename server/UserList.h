/*======================================================
    > File Name: UserList.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年05月06日 星期五 14时13分09秒
 =======================================================*/


#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct User;

typedef struct User {
    char *name;
    int socket;
    struct User *next;
} User;

User* CreateUserList();
void AddUserList(User *head, char *name, int socket);
void QuitUserList(User *head, char *name);
void PrintUserList(User *head);
User* SearchUser(User *head, char *name);

User* SearchUser(User *head, char *name) {
    if (head == NULL) {
        return NULL;
    }
    User *p = head->next;
    while (p != NULL) {
        if (strcmp(p->name, name) == 0) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

void PrintUserList(User *head) {
    if (head == NULL) {
        return;
    }

    User *p = head->next;
    while (p != NULL) {
        printf("%s->%d\n", p->name, p->socket);
        p = p->next;
    }
}

void QuitUserList(User *head, char *name) {
    if (head == NULL) {
        return;
    }

    User *p = head;
    User *q = head->next;

    while (q != NULL) {
        if (strcmp(q->name, name) == 0) {
            p->next = q->next;
            free(q->name);
            free(q);
            break;
        }
        p = q;
        q = q->next;
    }
}

void AddUserList(User *head, char *name, int socket) {
    if (head == NULL) {
        return;
    }
    User *node = (User*)malloc(sizeof(User));
    if (node == NULL) {
        perror("malloc() error\n");
        return;
    }

    node->name = (char*)malloc(strlen(name) + 1);
    if (node->name == NULL) {
        perror("malloc() error\n");
        return;
    }
    strcpy(node->name, name);
    node->socket = socket;

    node->next = head->next;
    head->next = node;
}

User* CreateUserList() {
    User *head = (User*)malloc(sizeof(User));
    if (head == NULL) {
        perror("malloc() error\n");
        return NULL;
    }
    head->name = NULL;
    head->socket = -1;
    head->next = NULL;
    return head;
} 

