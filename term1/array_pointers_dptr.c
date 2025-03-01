#include <stdio.h>

/* In this file we will learn how we can make arrays
   made of arrays.

  a = [1,5,9]  b = [0,2,4]

      [  a   ,   b   ]

  To do this, we need to understand the use of double pointers,
  or pointers to a pointer type:

*/

void array_pointers_basics();

int main()
{
    array_pointers_basics();
    return 0;
}

void array_pointers_basics()
{
    printf(" = = = = = = = = = = = = = = = = = = = \n");
    int arr0[] = {0, 1, 2};
    int arr1[] = {3, 4, 5};
    int arr2[] = {6, 7, 8};

    /* super basics: let's remember how we traverse over a simple array
    using pointer arithmetic and remember basic rules:

      1) The array name represents the address of the first element
      2) The array name cannot do pointer math   arr0++ will not work
      3) To move a pointer, we need a pointer of the stored type (int *) here
    */

    int *iptr = arr0;

    for (int i = 0; i < 3; i++)
    {
        printf("value arr0[%d] = %d at addr %p\n", i, *iptr, iptr);
        iptr++;
    }

    getchar();

    /* We can also traverse an array using a for loop */

    iptr = arr2;
    for (int i = 0; i < 3; i++, iptr++)
    {
        printf("value arr2[%d] = %d at addr %p\n", i, *iptr, iptr);
    }

    /* Creating an array of pointers */
    int *ptr_array[] = {arr0, arr1, arr2};

    printf("\n\nAddress of the original arrays: \n");
    for (int i = 0; i < 3; i++)
    {
        printf("%p, %p, %d\n", &ptr_array[i], ptr_array[i], *ptr_array[i]);
    }
    printf("\n");

    getchar();

    // -------------  NEW concepts!  ----------
    // If we have an array of int *, we need an int ** to traverse it.

    int **dptr = ptr_array; // Pointer to pointer
    printf("Address stored on pointer dptr:  \n");
    for (int i = 0; i < 3; i++)
    {
        printf("%p - %d \n", *dptr, **dptr);
        dptr++;
    }
    printf("\n");
    getchar();

    /* Using double pointers to simulate a matrix */

    dptr = ptr_array;
    int *int_ptr = NULL;

    printf("Using double pointers and pointers ");
    for (int i = 0; i < 3; i++)
    {
        int_ptr = *dptr;
        for (int x = 0; x < 3; x++)
        {
            printf(" %d ", *int_ptr);
            int_ptr++;
        }
        dptr++;
    }
    printf("\n");
    getchar();

    //  ---- The same, but using brackets ----
    printf("Using brackets: ");
    for (int i = 0; i < 3; i++)
    {
        iptr = ptr_array[i];
        for (int x = 0; x < 3; x++)
        {
            printf(" %d ", iptr[x]);
        }
    }
    printf("\n");
    getchar();

    printf("\n============================\n");
    getchar();

    /* now with characters */
    char letters[] = {'s', 'e', 'b', 'a', 's', '\0'};
    char name[] = "ale";

    char *str[] = {"edgar", name, "T", "nacho", letters};
    char **str_ptr = str;

    printf("%s\n", *str_ptr);
    str_ptr++;
    for (int i = 1; i < 5; i++)
    {
        printf("%s\n", *str_ptr);
        str_ptr++;
    }

    // Using a statically allocated 2D array of characters

    char classroom[3][10] = {"Mateo", "Iker", "Walls"};
    char *student = classroom[0];

    printf("student 1 = %s\n", student);

    char *c = student;
    for (int x = 0; x < 21; x++)
    {
        if (*c != '\0')
            printf("[%c]", *c);
        c++;
    }
    printf("\n");
}