#include <stdio.h>
#include <string.h>
#include "types.h"

Status open_files_edit(int argc, char *argv[], Tag_edit *tag)
{
    if (argc == 5)
    {
        if (argv[4] == NULL) // to check operation
        {
            printf("Error: MP3 file name not entered.\n");
            return e_failure;
        }
        tag->File_ptr = fopen(argv[4], "rb"); // top open mp3 file in read
        if (tag->File_ptr == NULL)
        {
            printf("Error: File could not be opened.\n");
            printf("Check if the file name is correct.\n");
            return e_failure;
        }
        else
        {
            tag->mp3_fname = argv[4]; // to store
           // printf("tag->mp3_fname %s\n", tag->mp3_fname);
            printf("MP3 File Name: %s\n", tag->mp3_fname);
            // printf("argv[4] File opened successfully\n");
        }
    }
    else
    {
        printf("Error: Arguments not provided correctly.\n");
        return e_failure;
    }

    tag->output_name = "output.mp3";
    tag->output_File_ptr = fopen(tag->output_name, "wb"); // to open output file in write mode
    if (tag->output_File_ptr == NULL)
    {
        printf("Error: Cannot write to the output file.\n");
        return e_failure;
    }
    else
    {
        // printf("output File opened in write mode successfully\n");
    }
    fseek(tag->File_ptr, 0, SEEK_END);
    long fsize = ftell(tag->File_ptr); // to store
    rewind(tag->File_ptr);
    // printf("Input file size: %ld bytes\n", fsize);

    if (fsize < 10)
    {
        printf("Error: File size insufficient for ID3 tag.\n");
        return e_failure;
    }
    return e_success;
}

Status read_and_validate_edit(int argc, char *argv[], Tag_edit *tag)
{

    strcpy(tag->in_tag_info, argv[3]); // to copy data in info

    strcpy(tag->in_tag, argv[2]); // to copy tag

    // to copy the tag to temporary structuure

    if (strcmp(tag->in_tag, "-t") == 0)
    {
        printf("You have selected to edit the song title.\n");
        strcpy(tag->in_tag, "TIT2");
    }
    else if (strcmp(tag->in_tag, "-a") == 0)
    {
        printf("You have selected to edit the artist's name.\n");
        strcpy(tag->in_tag, "TPE1");
    }
    else if (strcmp(tag->in_tag, "-A") == 0)
    {
        printf("You have selected to edit the album name.\n");
        strcpy(tag->in_tag, "TALB");
    }
    else if (strcmp(tag->in_tag, "-y") == 0)
    {
        printf("You have selected to edit the year.\n");
        strcpy(tag->in_tag, "TYER");
    }
    else if (strcmp(tag->in_tag, "-g") == 0)
    {
        printf("You have selected to edit the genre.\n");

        strcpy(tag->in_tag, "TCON");
    }
    else if (strcmp(tag->in_tag, "-c") == 0)
    {
        printf("You have selected to edit the comment.\n");

        strcpy(tag->in_tag, "COMM");
    }
    else
    {
        printf("Enter -t to change the  Song Title\n");
        printf("Enter -a to change the  Artise Name\n");
        printf("Enter -A to change the  Album Name\n");
        printf("Enter -y to change the  Year\n");
        printf("Enter -g to change the  Genre\n");
        printf("Enter -c to change the  Commnet\n");
        return e_failure;
    }
    printf("Tag chosen: %s\n", tag->in_tag);

    return e_success;
}

Status Edit_mp3(Tag_edit *tag)
{
    char header[10], ch;
    if (tag->File_ptr == NULL || tag->output_File_ptr == NULL)
    {
        printf("Error: Unable to open input or output file.\n");
        return e_failure;
    }
    // to ensure both file pointers are at start
    rewind(tag->File_ptr);
    rewind(tag->output_File_ptr);

    int bytes = fread(header, 1, 10, tag->File_ptr); // to copy header
    if (bytes != 10)
    {
        printf("Error: Could not read header. Read %d bytes.\n", bytes);
        return e_failure;
    }
    bytes = fwrite(header, 1, 10, tag->output_File_ptr); // to paste header
    if (bytes != 10)
    {
        printf("Error: Could not write header. Wrote %d bytes.\n", bytes);
        return e_failure;
    }
    // printf("tag->in_tag_info --> %s\n",tag->in_tag_info);

    for (int i = 0; i < 6; i++) // as tags are present in top 6 only
    {
        if (fread(tag->tag, 1, 4, tag->File_ptr) != 4) // to read tag
        {
            printf("Error: Unable to read while editing.\n");
            return e_failure;
        }
        if (fread(tag->size_tag, 1, 4, tag->File_ptr) != 4) // to read size
        {
            printf("Error: Unable to read the tag size.\n");
            return e_failure;
        }
        // to convert lttle to big
        int frame_size = (tag->size_tag[0] << 24) | (tag->size_tag[1] << 16) | (tag->size_tag[2] << 8) | (tag->size_tag[3]);
        // printf("%d\n", frame_size);
        if (fread(tag->flag, 1, 2, tag->File_ptr) != 2) // to read flag
        {
            printf("Error: Unable to read flag.\n");
            return e_failure;
        }

        if (fread(tag->info, 1, frame_size, tag->File_ptr) != frame_size) // to read the content of the tags
        {
            printf("Error: Unable to read the content.\n");
            return e_failure;
        }
        tag->info[frame_size] = '\0'; // to add null chaacter

        // printf("tag->in_tag_info outside write --> %s\n", tag->in_tag_info);

        if (strcmp(tag->tag, tag->in_tag) == 0) // if that tag is compared
        {
            if (fwrite(tag->tag, 1, 4, tag->output_File_ptr) != 4) // write the tag
            {
                printf("Error: Unable to write tag in edit mode.\n");
                return e_failure;
            }

            int len = strlen(tag->in_tag_info) + 1; // to add null we are taking +1

            // liittle to big endian
            unsigned char size_bytes[4] = {0};

            size_bytes[0] = (len >> 24) & 0xFF;
            size_bytes[1] = (len >> 16) & 0xFF;
            size_bytes[2] = (len >> 8) & 0xFF;
            size_bytes[3] = len & 0xFF;

            if (fwrite(size_bytes, 1, 4, tag->output_File_ptr) != 4) // writing tag
            {
                printf("Error: Unable to write size in edit mode.\n");
                return e_failure;
            }
            if (fwrite(tag->flag, 1, 2, tag->output_File_ptr) != 2) // writing flag
            {
                printf("Error: Unable to write flag in edit mode.\n");
                return e_failure;
            }

            unsigned char encoding = 0x00;                          // 0x00 is null character
            if (fwrite(&encoding, 1, 1, tag->output_File_ptr) != 1) // writing null character
                return e_failure;

            // Then write the actual string (without null terminator)
            /// printf("tag->in_tag_info inside write function --> %s\n", tag->in_tag_info);
            if (fwrite(tag->in_tag_info, 1, len - 1, tag->output_File_ptr) != len - 1) // to write our data in iit
            {
                printf("Error: Unable to write information in edit mode.\n");
                return e_failure;
            }
        }
        else // copying the old data to it
        {
            if (fwrite(tag->tag, 1, 4, tag->output_File_ptr) != 4)
            {
                printf("Error: Tag could not be written in edit mode.\n");
                return e_failure;
            }

            if (fwrite(tag->size_tag, 1, 4, tag->output_File_ptr) != 4)
            {
                printf("Error: Could not write size in edit mode.\n");
                return e_failure;
            }
            if (fwrite(tag->flag, 1, 2, tag->output_File_ptr) != 2)
            {
                printf("Error: Could not write flag in edit mode.\n");
                return e_failure;
            }

            if (fwrite(tag->info, 1, frame_size, tag->output_File_ptr) != frame_size)
            {
                printf("Error: Could not write info in edit mode.\n");
                return e_failure;
            }
        }
    }

    // to copy remaining data
    while (fread(&ch, 1, 1, tag->File_ptr) == 1)
    {
        if (fwrite(&ch, 1, 1, tag->output_File_ptr) != 1)
        {
            printf("Error: Could not copy data.\n");
            return e_failure;
        }
    }

    remove(tag->mp3_fname); // to remove the file
    // printf("tag->output_name %s,tag->mp3_fname %s\n",tag->output_name,tag->mp3_fname);
    rename(tag->output_name, tag->mp3_fname); // to rename the file
    printf("The file has been copied successfully.\n");

    // to close files
    fclose(tag->File_ptr);
    fclose(tag->output_File_ptr);
    return e_success;
}