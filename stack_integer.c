#include <stdio.h>
#include <stdlib.h>

/* NOTE: This entire stack only works with integers since the stack node stores
   an integer value. In the next exam, we will see how to improve this and
   generalize it using an ADT (Abstract Data Type) later. */

// Structure to create a stack_node
typedef struct stack_node
{
    int info;
    struct stack_node *prev;
} stack_node;

/* GLOBAL STACK */
stack_node *stack_top = NULL;
int count = 0;

// Push() operation to insert elements into the stack
void push(int data)
{
    stack_node *nptr = stack_top;
    if (nptr == NULL)
    {
        nptr = (stack_node *)malloc(sizeof(stack_node));
        nptr->info = data;
        nptr->prev = NULL;
        stack_top = nptr;
        count++;
    }
    else
    {
        nptr = (stack_node *)malloc(sizeof(stack_node));
        nptr->info = data;
        nptr->prev = stack_top;
        stack_top = nptr;
        count++;
    }
}

// For this version of the integer stack, it returns -1 if there is nothing to pop
int pop()
{
    stack_node *nptr = stack_top;
    if (stack_top == NULL)
    {

        return -1;
    }
    else
    {
        stack_top = nptr->prev;
        count--;
        return nptr->info;
    }
}

// Displays the current elements in the stack
void display()
{
    stack_node *nptr = stack_top;

    if (nptr == NULL)
    {
        printf("\nStack Underflow\n");
        return;
    }

    printf("The stack is \n");
    while (nptr != NULL)
    {
        printf("%d--->", nptr->info);
        nptr = nptr->prev;
    }
    printf("NULL\n\n");
}

int main()
{
    int choice, value, ret;
    printf("\nSTACK:\n");
    while (1)
    {
        printf("\n1. Push\n2. Pop\n3. Display\n4. Exit\n");
        printf("\n\tChoice : ");
        ret = scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("\tEnter the value to insert: ");
            ret = scanf("%d", &value);
            push(value);
            break;
        case 2:
            if (stack_top == NULL)
            {
                printf("There is literally nothing to pop\n");
            }
            else
            {
                printf("Popped element is :%d\n", pop());
            }
            break;
        case 3:
            display();
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("\nWrong Choice\n");
        }
    }
}
