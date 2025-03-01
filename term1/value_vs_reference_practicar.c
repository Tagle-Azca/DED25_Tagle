#include <stdio.h>

void swap(int *, int *);
int blackjack(int, int *, char *);

int main()
{
    /* Exercise 1: Swap num1 and num2 values */
    int num1 = 1;
    int num2 = 20;

    printf("Before Swap: num1 = %d, num2 = %d\n", num1, num2);
    swap(&num1, &num2);
    printf("After Swap: num1 = %d, num2 = %d\n", num1, num2);

    /* Exercise 2: Blackjack */
    char flag = '0'; // Initial flag value
    int result = blackjack(num1, &num2, &flag);

    if (flag == 'W')
        printf("We have a winner!\n");
    printf("Sum is %s = %d\n", result ? "POSITIVE" : "NEGATIVE", num2);

    return 0;
}

/*
  Exercise 1 : Swap
  Swaps the contents of two variables.
  The function receives two integers (by reference),
  swaps their values, and returns nothing.
*/
void swap(int *a, int *b)
{
    if (a == NULL || b == NULL)
        return;

    printf("\tInside Swap: Before -> a = %d, b = %d\n", *a, *b);
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
    printf("\tInside Swap: After  -> a = %d, b = %d\n", *a, *b);
}

/*
  Exercise 2 : Blackjack

  This function:
  - Receives two numbers and a FLAG (char).
  - Returns 1 if the sum is positive (> 0).
  - Returns 0 if the sum is negative.
  - Stores the sum of the two numbers in num2.
  - If the sum equals 21, the FLAG is set to 'W' (winner).
*/
int blackjack(int num1, int *num2, char *flag)
{
    if (num2 == NULL || flag == NULL)
        return 0;

    *num2 += num1;

    if (*num2 == 21)
        *flag = 'W';

    return (*num2 > 0) ? 1 : 0;
}