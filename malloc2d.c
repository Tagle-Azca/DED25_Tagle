#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Lets create a 2D INT matrix of 5 columns and 3 rows
    [1, 2, 3, 4,  5]
    [6, 7, 8, 9, 10]
    [11,12,13,14,15]

    Don't forget to free it!
   */

int main()
{
    // step # 1. create the double pointer that will be used to reference to
    // the 2D matrix

    int rows = 3, cols = 5;
    int **matrix = (int **)malloc(rows * sizeof(int *));

    // step #2 . allocate enough memory for the rows (what is the data type?)

    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }

    // step #3 . for each one of the rows, allocate enough memory for the columns

    // step #4 . verify that the 2D matrix works as usual, just as you saw in the
    // previous semesters, you can use brackets [ ] fill matrix and print to test

    int counter = 1;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = counter++;
        }
    }

    // fill it
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    /* Can you do the same to store the following :

    ["WELCOME"]                len = 8
    ["TO"]                     len = 3
    ["DATA STRUCTURE CLASS"]   len = 21

    */

    char *messages[3];
    messages[0] = (char *)malloc(8 * sizeof(char));
    messages[1] = (char *)malloc(3 * sizeof(char));
    messages[2] = (char *)malloc(21 * sizeof(char));

    strcpy(messages[0], "WELCOME");
    strcpy(messages[1], "TO");
    strcpy(messages[2], "DATA STRUCTURE CLASS");

    /* can you print it using pointer math :) ? */
    for (int i = 0; i < 3; i++)
    {
        char *ptr = messages[i];
        while (*ptr)
        {
            printf("%c", *ptr);
            ptr++;
        }
        printf("\n");
    }

    /* can you print it with brackers [] ? */
    for (int i = 0; i < 3; i++)
    {
        printf("%s\n", messages[i]);
    }

    /* FREE everything we just created */
    for (int i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }
    free(matrix);

    for (int i = 0; i < 3; i++)
    {
        free(messages[i]);
    }

    return 0;
}