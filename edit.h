#ifndef EDIT_H
#define EDIT_H

#include "mp3.h"


typedef struct _editDetails
{
    /* mp3 details */
    char *mp3name;
    FILE *fptr_mp3;

    /* New file */
    char *newfile;
    FILE *fptr_new;

    /* data buff */
    char data_buff[255];


} EditData;

/* -- view function prototypes -- */

//validate arguments
Status validate_edit_args(char *argv[], EditData *editData);

//Open files
Status open_editfiles(EditData *editData);

//editing
Status go_edit_data(char *argv[], EditData *editData);

//check tag
Status check_edit_tag(char *argv[], EditData *editData);

//copy header
Status copy_header(EditData *editData);

//copy initial bytes
Status copy_first_bytes(int curr_pos, EditData *editData);

//copy size
Status copy_size(int size, EditData *editData);

//copy new name
Status copy_new(char *argv[], int size, EditData *editData);

//copy remaining data
Status copy_remaining_data(EditData *editData);

//check size
unsigned int check_size_ed(EditData *editData);

//edit data
Status edit_data(char *argv[], EditData *editData);
#endif