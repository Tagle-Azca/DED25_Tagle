#include <stdlib.h>
#include <stdio.h>

int main()
{
   FILE *salesFile, *averageFile;
   char line[100];
   int day, items;
   float sales, average;

   salesFile = fopen("sales.txt", "r");
   if (salesFile == NULL)
   {
      printf("cant oppen sales.txt\n");
      return 1;
   }

   averageFile = fopen("average_sales.txt", "w");
   if (averageFile == NULL)
   {
      printf("Ncant create average_sales.txt\n");
      fclose(salesFile);
      return 1;
   }

   fprintf(averageFile, "Month average\n");

   while (fgets(line, sizeof(line), salesFile))
   {
      if (sscanf(line, "day %d sales $%f items %d", &day, &sales, &items) == 3)
      {
         if (items == 0)
         {
            fprintf(averageFile, "day %d ERROR: division by zero!\n", day);
         }
         else
         {
            average = sales / items;
            fprintf(averageFile, "day %d average $%.2f\n", day, average);
         }
      }
   }

   fclose(averageFile);
   fclose(salesFile);

   salesFile = fopen("sales.txt", "a");
   if (salesFile == NULL)
   {
      printf("Cant oppen sales.txt to write\n");
      return 1;
   }

   fprintf(salesFile, "Average: average_sales.txt\n");

   fclose(salesFile);

   return 0;
}