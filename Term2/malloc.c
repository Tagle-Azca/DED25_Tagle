#include <stdio.h>
#include <stdlib.h>

/*  Malloc / Free

  Use only dynamic memory for this exercise.
  Remember, for EVERY malloc, there must exist a free */

int main()
{
  /* 1) create an int, and assign the value 10 to it. Print it to be sure */
  int *iptr = (int *)malloc(sizeof(int));
  int *i = (int *)malloc(sizeof(int));

  if (iptr == NULL || i == NULL)
  {
    printf("Memory allocation failed\n");
    return 1;
  }

  *iptr = 10;
  *i = 10;

  printf("The value of iptr and i is both %d %d\n", *i, *iptr);

  /* 2) create an array of 10 integers */
  int *arptr = (int *)malloc(10 * sizeof(int));

  if (arptr == NULL)
  {
    printf("Memory allocation failed\n");
    free(iptr);
    free(i);
    return 1;
  }

  /* 2.1) fill every space with random numbers and print them  */
  for (int j = 0; j < 10; j++)
  {
    arptr[j] = rand();
    printf("arr[%d] = %d\n", j, arptr[j]);
  }

  /* 3) Create an array with 20 chars, and put your name in it, (request it to the user)
        and print it  */
  char *name = (char *)malloc(sizeof(char) * 20);

  if (name == NULL)
  {
    printf("Memory allocation failed\n");
    free(iptr);
    free(i);
    free(arptr);
    return 1;
  }

  printf("Enter your name: ");
  fgets(name, 20, stdin);

  printf("My name is: %s\n", name);

  /* before this simple program complete. What do we need to do?
    Remember the golden rule! */

  free(iptr);
  free(i);
  free(arptr);
  free(name);

  /* and who will release the memory for the pointers? We don't! because all
  of our pointers, were created with STATIC memory and the program will free that
  memory for us as soon as the scope of the variable ends. */

  /* ==================================================================== */

  return 0;
}