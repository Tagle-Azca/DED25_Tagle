#include <stdio.h>
#include <string.h>

char letter = 'A';

// how to create a pointer
//  the asteric (*) is the key to create a pointer
int *Ipointer;
char *Cpointer;
float *Fpointer;

// using the & operrator we can get the adrees of a variable
char *pc1 = &pc1;

int main()
{
    printf("%c\n", letter); // save the variable
    printf("%d\n", letter); // saves the adress of the variable
    printf("%p\n", pc1);
    printf("%n\n", pc1);
}