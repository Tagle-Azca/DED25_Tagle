#include <stdio.h>
#include <time.h>

int main()
{
    // its only done once
    int num;
    for (int i = 0; i < 10; i++)
    {
        num = rand();
        printf("NUM = %D\n", num);
    }
    getchar();
}