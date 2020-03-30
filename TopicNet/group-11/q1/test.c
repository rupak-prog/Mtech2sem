#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/syscall.h>
#include <string.h>
int main(void){
    char st[256];
    gets(st);
    long sta = syscall(333,st);
	system("dmesg|tail -1");	
    return 0;
}
