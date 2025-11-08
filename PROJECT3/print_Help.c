#include<stdio.h>
#include "types.h"
void print_help()
{
    printf("\n ----MP3 TAG READER & EDITOR----\n");
    printf("Usage:\n");
    printf("  ./a.out -v <mp3_file>                         : View all tag details\n");
    printf("  ./a.out -e <mp3_file>                         : Edit tags interactively\n");
    printf("  ./a.out -e <mp3_file> -t <tag> -n <new_value> : Edit a specific tag directly\n");
    printf("  ./a.out -h                                    : Show help information\n");


    printf("\n==============================================\n");
    printf("               SUPPORTED TAGS                  \n");
    printf("==============================================\n");
    printf("  Tag   | Description\n");
    printf("----------------------------------------------\n");
    printf("  TIT2  | Title / Song Name\n");
    printf("  TPE1  | Artist Name\n");
    printf("  TALB  | Album Name\n");
    printf("  TYER  | Year of Release\n");
    printf("  TCON  | Genre / Content Type\n");
    printf("  TCOM  | Composer / Music Creator\n");
    printf("==============================================\n\n");


    printf("\n================= EXAMPLES =================\n");
    printf("  View all tags:\n");
    printf("      ./a.out -v song.mp3\n\n");
    printf("  Edit a specific tag (Example: Change Title):\n"); 
    printf("      ./a.out -e song.mp3 -t TIT2 -n \"New Song Title\"\n");
    printf("============================================\n\n");
}