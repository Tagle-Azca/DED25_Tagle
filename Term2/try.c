#include <stdio.h>

int main()
{
    int numero = 10;
    int *ptr;

    int arr[] = {100, 50, 102, 340, 33, 123};
    int *arrptr = arr;
    int *end = arr + (sizeof(arr) / sizeof(arr[0]));

    int x = 150;
    int *z = &x;

    int in = 5034;
    int *i = &in;

    printf("Valor del numero: %d\n", numero);
    printf("direccion del numero: %p\n", &numero);
    printf("Valor del numero %d\n", *ptr);
    ptr = &numero;
    printf("Valor apuntado por ptr: %d \n", *ptr);

    printf("Hola mi nombre es: %d\n", x);
    printf("tengo la direccion de %p\n ", &x);

    if (x >= 0)
    {
        printf("Numero registrado: %d\n", x);
    }
    else
    {
        printf("direccion del numero: %p\n", &z);
    };

    printf(" elemento de la lista: %d\n", *(arrptr + 4));

    while (arrptr < end)
    {
        printf("elemento: %d\n", *arrptr);
        arrptr++;
    }

    printf("puntero con int: %d\n ", *i);

    return 0;
}