#include <stdio.h>

void print_start(int b)
{
    while (b--)
        printf("|*|");
    printf("\n");
}

void print_array(int *);
int main()
{
    int arrA[5] = {5, 4, 3, 2, 1};
    int arrB[5] = {2, 2, 4, 4, 5};

        int var = 5;
    int *ptr = &var;
    int **ptr = *ptr;

    ptr = arrA;
    print_array(ptr);
}

void print_array(int *a)
{

    // las dos funcionan igual la logica es la misma solo cambia la forma en la que se usa
    printf("%d\n", a[4]);
    printf("%d\n", *(a + 4));

    for (int x = 0; x < 4; x++)
    {
        printf("%d ", a[x]);
    }
}

void process(int **dptr) {
    // printf("[0] %d", dptr[0]);
    // printf("%d\n", dptr[1]);
};