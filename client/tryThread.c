/*======================================================
    > File Name: tryThread.c
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年05月22日 星期日 21时20分27秒
 =======================================================*/

#include <stdio.h>
#include <pthread.h>

void* fun2(void *arg) {
    printf("fun2...\n");
    sleep(2);
    printf("子子线程退出\n");
}

void* fun1(void *arg) {
    pthread_t tid2;
    pthread_create(&tid2, NULL, fun2, NULL);
    printf("fun1...\n");
    //kill(tid2, 0);
    
    pthread_cancel(tid2);
    printf("子线程杀死子子线程\n");
}

int main() {
    pthread_t tid1;
    pthread_create(&tid1, NULL, fun1, NULL); 

    sleep(4);   
}
