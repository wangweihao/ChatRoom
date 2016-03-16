/*======================================================
    > File Name: server.c
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月16日 星期三 09时52分42秒
 =======================================================*/

#include <stdlib.h>
#include "server.h"

int main(int argc, char **argv) {
    InitServer(argv[1], atoi(argv[2]));
}
