#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Create a programs that reads from a text file the following format

8
3216579
$989.90
3255872
$1000.50
3262498
$950.00
3262500
$1950.30
3162501
$40950.99
3005905
$140950.20
1000590
$99105.20
3150199
0.00
ITESO SA DE CV
...

The number represents
The very first row, contains the number of students  (8)
Then the following 8 pairs
  ITESO ID
  $Ammount of debt
The Name of the School is the last line (always below 50 chars)

Create a program that using that information, creates as many student
structures as needed. Use Dynamic memory for everything

student = {int ID, float debt}

TASK #1
a "school" contains X ammount of students, and its an array of student
pointers just like we did in the Farm excercise.
school = {*,    *,    *}
          \           \
           student     student


TASK #2
Use the provided Function (print_school_debt() to verify that everything is
working as we expected. Don't for get to close the text file. We don't need it
after this point.


TASK #3
Create a function that "dumps" the content of our program into binary file.
Call this file school_debt.dump
Make sure to include all the information needed to recreate the program.

Close the binary. And done!

QUESTION:
How much space are the text file vs the binary file using? check this in
your operating system file properties.

Can we tell at this point if the binary file is correct? or not corrupt? how
can we verify that it contains all the right info? .... WE CAN'T!

Until we read it back. So:

TASK #4
Create another program to read the binary file you just created.
The program should work like this.

  WELCOME TO THIS PROGRAM:
  Please enter the binary file to open : (read file name from keyboard)
  You have chosen a file for school : ITESO SA DE CV

  Press ENTER TO CONTINUE LOADING

  Loading ...
  Data load complete.
  Student 3216579 debt is $989.90
  Student 3255872 debt is $1000.50
  ...

As you see, we need to read the SCHOOL NAME first, to make sure we are
loading the right file. Find a way (tip, use FSEEK) to move to the last
part of the file to load the info we need and then rewind the file as
needed to read the rest.

Re use the function to print the student that we use before to make
sure it works.

In this big excercise we are putting in practice:

  -Reading TEXT FILES
  -Writing into Binary files
  -Reading from Binary files
  -Using FSEEK to move inside a binary file
  -building structures with dynamic memory
  -Allocating arrays of arrays on demand with the exact memory we need
  -Compare the size of text file vs a binary file for this use
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  int id;
  float debt;
} student;

void print_school_debt(student **school, int num_students, char *name)
{

  printf("School: %s\n", name);
  for (int i = 0; i < num_students; i++)
  {
    printf("Student %d debt is $%.2f\n",
           school[i]->id, school[i]->debt);
  }
}

student *create_student(int id, float debt)
{
  student *s = (student *)malloc(sizeof(student));
  s->id = id;
  s->debt = debt;
  return s;
}

int main()
{
  char text_filename[50] = "iteso_data.txt";
  char binary_filename[100] = "iteso_debt.dump";
  char school_name[50];
  student **school;
  int num_students;

  FILE *iteso_data = fopen(text_filename, "r");

  if (iteso_data != NULL)
  {
    printf("File opened successfully.\n");
  }
  else
  {
    printf("File Cant oppen ");
  };

  fscanf(iteso_data, "%d", &num_students);
  school = (student **)malloc(sizeof(student *) * num_students);

  for (int i = 0; i < num_students; i++)
  {
    int id;
    float debt;
    char debt_str[20];
    fscanf(iteso_data, "%d\n", &id);
    fscanf(iteso_data, "%s\n", debt_str);
    sscanf(debt_str, "$%f", &debt);
    school[i] = create_student(id, debt);
  }

  fgets(school_name, 50, iteso_data);
  school_name[strcspn(school_name, "\n")] = 0;

  fclose(iteso_data);

  print_school_debt(school, num_students, school_name);

  return 0;
}