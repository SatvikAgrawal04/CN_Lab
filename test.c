#include <stdio.h>

int main(int argc, char const *argv[])
{
    char send_buf[100];
    printf("Enter message: \n");
    scanf("\n%s", send_buf);

    for (int i = 0; i != '\0'; i++)
    {
        printf("%d", send_buf[i]);
    }

    return 0;
}
