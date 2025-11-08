#include <stdio.h>

#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

#define MAX_DATA 1000

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_help,
    e_view,
    e_edit,
    e_unsupported
} OperationType;

typedef struct _mp3_tagreader
{
    /* data */
    FILE *tag_ptr;

} mp3;
typedef struct _mp3_view
{
    FILE *File_ptr;
    char *mp3_fname;

    char header_tag[3];
    char header_tag_version[2];

    char tag[4];
    char size_tag[4];
    char info[MAX_DATA];
} Tag_view;

typedef struct _mp3_edit
{
    FILE *File_ptr;
    char *mp3_fname;

    FILE *output_File_ptr;
    char *output_name;

    char in_tag[5];
    char in_tag_info[MAX_DATA];

    char tag[5];
    char size_tag[4];
    char flag[3];
    char info[MAX_DATA];
} Tag_edit;

void print_help();
// to check the operation
OperationType check_operation(int argc, char *argv[]);

// to view

// to open fiiles
Status open_files(int argc, char *argv[], Tag_view *tag);

Status view_mp3(Tag_view *tag);

// to edit
Status open_files_edit(int argc, char *argv[], Tag_edit *tag);

Status read_and_validate_edit(int argc, char *argv[], Tag_edit *tag);

Status Edit_mp3(Tag_edit *tag);

#endif
