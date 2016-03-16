/*======================================================
    > File Name: run.c
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月15日 星期二 23时36分57秒
 =======================================================*/

#include "client.h"
#include <stdlib.h>

int main(int argc, char **argv) {
    if(argc < 3) {
        perror("argument error");
        exit(1);
    }
    Start(argv[1], atoi(argv[2]));

    return 0;
}

