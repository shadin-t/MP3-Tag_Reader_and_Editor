#ifndef MP3_H
#define MP3_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_view ,
    e_edit ,
    e_unsupported ,
    e_help
} OperationType;

typedef struct _Mp3TagInfo
{
    char *mp3_file_name ;
    FILE *fptr_mp3_file ;

    int size ;
    char *Tag ;
    char *content ;\

}Mp3TagInfo ;


/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate args from argv */
Status read_and_validate_for_view(char *argv[] , Mp3TagInfo *mp3Info);

Status view(Mp3TagInfo *mp3Info);

Status check_id3(Mp3TagInfo *mp3Info);

Status check_version(Mp3TagInfo *mp3Info);

Status check_tags(Mp3TagInfo *mp3Info);

Status print_content(Mp3TagInfo *mp3Info);

/* Get File pointers for files */
Status open_files(Mp3TagInfo *mp3Info);

int convert_big_to_little(int size);



#endif