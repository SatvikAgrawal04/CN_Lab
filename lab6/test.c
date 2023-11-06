#include<stdio.h>
#include<stdlib.h>

int main()
{
	system("ls>res.txt");
	FILE* fd = fopen("res.txt", "r");
	char buf[100];
	fread(buf, 1, 100, fd);
	printf("%d", feof(fd));
	return 0;
}
