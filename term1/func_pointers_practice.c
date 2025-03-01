#include <stdio.h>
#include <unistd.h>
#include <string.h>

/* This function is called when student has a non-probatory grade */
void fail(char *name, int grade)
{
  printf("Hey %s, you need to study harder!\n"
         "Your grade is %d!\n",
         name, grade);

  printf("Contacting parents... \n");
  for (int x = 0; x < 3; x++)
  {
    printf("ring...\n");
    sleep(1); // this function is inside unistd.h!!
  }
  printf("Meeting scheduled. Go home.\n");
}

/* This is the regular function for any student with a normal grade */
void average(char *name, int grade)
{
  printf("Hey %s, Your grade is %d. Keep it up!\n",
         name, grade);
}

/* Use this function to refer to the top performer of the class */
void outstanding(char *name, int grade)
{
  printf("Congratulations, %s! Your grade is %d. Excellent work!\n",
         name, grade);
}

/* CHALLENGE:
  On the main method, create an ARRAY of 3 function pointers
  Store each one of the function pointers in the following positions :  */
#define FAIL 0
#define AVERAGE 1
#define TOP 2

/* Complete the function "evaluate_student" so it receives this array,
   the student's name, and the grade.

   The function should, depending on the student, call the right function
   following these rules:

   Grade 100, run function stored at TOP position
   Grade < 60, run FAIL.
   For everyone else, run AVERAGE.
*/

typedef void (*grading_function)(char *name, int grade);

void evaluate_student(grading_function *functions, char *student, int score)
{
  grading_function action;

  if (score == 100)
    action = functions[TOP];
  else if (score < 60)
    action = functions[FAIL];
  else
    action = functions[AVERAGE];

  /* Execute the corresponding function */
  action(student, score);
}

typedef struct student_info
{
  char name[10];
  int score;
  grading_function grade_func;
} student_info;

void process_student_info(student_info *student)
{
  student->grade_func(student->name, student->score);
}

int main()
{
  char student_name[10];
  int student_grade;

  printf("Enter student name: ");
  scanf("%9s", student_name);

  printf("Enter the grade: ");
  scanf("%d", &student_grade);

  grading_function grade_actions[3];

  grade_actions[FAIL] = fail;
  grade_actions[AVERAGE] = average;
  grade_actions[TOP] = outstanding;

  /* TODO #1 Call evaluate_student */
  evaluate_student(grade_actions, student_name, student_grade);

  /* TODO #2 Store function pointers inside a student struct and execute accordingly */

  student_info top_student;
  strcpy(top_student.name, "Alex");
  top_student.score = 100;
  top_student.grade_func = outstanding;

  process_student_info(&top_student);

  return 0;
}