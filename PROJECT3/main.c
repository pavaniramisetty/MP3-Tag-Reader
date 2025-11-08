/*
Name : Pavani
Date: 28/10/25 - 8/11/25
Description:An MP3 Tag Reader in C reads and displays song metadata stored in MP3 files. 
It accesses ID3v1 tags, extracts details like title, artist, album, and year, and prints them using file handling functions like fseek() and fread().
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

int main(int argc, char *argv[])
{
     if (argc >= 2)
     {
          if (check_operation(argc, argv) == e_help) // to check if it is help
          {
               printf("You chose the help menu.\n");
               printf("Displaying help information...\n");
               print_help();
          }
          else if (check_operation(argc, argv) == e_view) // to check for view
          {
               printf(">>>>>>>>>     DISPLAYING MP3 TAGS     <<<<<<<<<\n");
     


               Tag_view tag;

               if (open_files(argc, argv, &tag) == e_failure) // to open files
               {
                    return e_failure;
               }
               printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

               if (view_mp3(&tag) == e_failure) // to view the data
               {
                    printf("VIEW failed!!\n");
                    return e_failure;
               }
               printf(">>>>>>>>>  DETAILS DISPLAYED SUCCESSFULLY  <<<<<<<<\n");
          }
          else if (check_operation(argc, argv) == e_edit) // tocheck for edit
          {
               printf(">>>>>>>>>>>     EDITING MP3 TAG DATA     <<<<<<<<<<<\n");
               Tag_edit tag;
               if (open_files_edit(argc, argv, &tag) == e_failure) // to open files
               {
                    printf("Opening files failed\n");
                    return e_failure;
               }
               if (read_and_validate_edit(argc, argv, &tag) == e_failure) // to validate the edit
               {
                    printf("read an validate failed\n");
                    return e_failure;
               }
               if (Edit_mp3(&tag) == e_failure) // to change the ediit
               {
                    printf("Editing failed\n");
                    return e_failure;
               }

               printf(">>>>>>>>>   DETAILS EDITED SUCCESSFULLY   <<<<<<<<<\n");          }
          else if (check_operation(argc, argv) == e_unsupported) // to check for other values
          {
               printf("Invalid input. Please Enter -help, -v, or -e\n");
               return e_failure;
          }
     }

     else
     {
          printf("Error: Incorrect argument format.\n");
          printf("Check your arguments and retry.\n");
          return e_failure;
     }
}
