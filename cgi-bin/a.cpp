/*************************************************************************
    > File Name: a.cpp
    > Author:
    > Created Time: 一  4/25 16:21:29 2016
 ************************************************************************/
#include <cstdio>
#include <cstring>
char a[20]="asb\0";

int main(int argc, char *argv[]){
	strcpy(a, argv[1]);
	printf("{\"get\": \"%s\", \"put\": 2}\n", a);
	//	fprintf(stderr, "log: %s\n", a);
}
