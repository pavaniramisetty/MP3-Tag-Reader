#include <stdio.h>
#include <string.h>
#include "types.h"

OperationType check_operation(int argc, char *argv[])
{
    if (strcmp(argv[1], "-help") == 0)// returs e_help if it is help
    {
        return e_help;
    }

    else if (strcmp(argv[1], "-v") == 0)// returns e_view if it is view
    {
        return e_view;
    }

    else if (strcmp(argv[1], "-e") == 0)// returns e_edit if it is edit
    {
        return e_edit;
    }

   
        return e_unsupported;// returns e_unsupported is any other value is entered
    

    
}

Status open_files(int argc, char *argv[], Tag_view *tag)
{
    if (argv[2] == NULL)// to check the argv[2] is null then the value is not entered 
    {
        printf("Error: MP3 file name not specified.\n");
        printf("Please provide the MP3 file name.\n");
        return e_failure;
    }
    tag->File_ptr = fopen(argv[2], "rb");//  to open the file 
    if (tag->File_ptr == NULL)// maybe file pointer is not opened
    {
        printf("Error: File could not be opened.\n");
        printf("File name appears to be different.\n");
        printf("Please verify the file name.\n");
        return e_failure;
    }

    printf("            File Opened Sussessfully\n");
    return e_success;
}

Status view_mp3(Tag_view *tag)
{

    fseek(tag->File_ptr, 10, SEEK_SET);// to skip the header of mp3
    int i = 0;
    while (i != 6)// this has to run only for 6 times as out tag will be present in only top 6 
    {
        if (fread(tag->tag, 1, 4, tag->File_ptr) != 4)// to read header
        {
            printf("Error: Could not read the header.\n");
            return e_failure;
        }
        if (fread(tag->size_tag, 1, 4, tag->File_ptr) != 4)// to read size
        {
            printf("Error: Tag size could not be read\n");
            return e_failure;
        }

        // to convert size from big to little endian
        int frame_size = (tag->size_tag[0] << 24) | (tag->size_tag[1] << 16) | (tag->size_tag[2] << 8) | (tag->size_tag[3]);
        // printf("frame size : %d\n", frame_size);
        fseek(tag->File_ptr, 2, SEEK_CUR);// to skip for flag

        if (fread(tag->info, 1, frame_size, tag->File_ptr) != frame_size) // to read the content of the tags
        {
            printf("Error: Content could not be read\n");
            return e_failure;
        }
        tag->info[frame_size] = '\0';
        printf("%s    :     ", tag->tag); // to print the tag// in tag info first character is NULL
        for (int i = 1; i <= frame_size; i++)
        {
            printf("%c", tag->info[i]);// printing character by character
        }
        printf("\n");
        i++;
    }

    fclose(tag->File_ptr);// to close the file payment
    return e_success;
}
