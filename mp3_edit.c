#include "edit.h"

/* Function Definitions*/
Status validate_edit_args(char *argv[], EditData *editData)
{

    //validate argv[4] file name
    if(argv[4] == NULL || strstr(argv[4], ".mp3") == NULL)
    {

        return e_failure;
    }

    editData -> mp3name = argv[4];

    //validate options
    if (argv[2] == NULL || 
        (strcmp(argv[2], "-t") != 0 && strcmp(argv[2], "-a") != 0 &&
         strcmp(argv[2], "-A") != 0 && strcmp(argv[2], "-y") != 0 &&
         strcmp(argv[2], "-m") != 0 && strcmp(argv[2], "-c") != 0))
    {

        return e_failure;
    }

    return e_success;
}

Status open_editfiles(EditData *editData)
{
    // open mp3 file
    editData -> fptr_mp3 = fopen(editData -> mp3name, "rb");

    // Do Error handling
    if (editData -> fptr_mp3 == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", editData -> mp3name);

        return e_failure;
    }

    //open one new file to add the edited version
    editData -> newfile = "new.mp3";
    editData -> fptr_new = fopen(editData -> newfile, "wb");

    // Do Error handling
    if (editData -> fptr_new == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", editData -> newfile);

        return e_failure;
    }


    // No failure return e_success
    return e_success;
}

Status go_edit_data(char *argv[], EditData *editData)
{
    //open files
    if(open_editfiles(editData) != e_success)
    {
        printf("File not opened\n");
        return e_failure;
    }

    //edit

    //check tags
    if(check_edit_tag(argv,editData) != e_success)
    {
        return e_failure;
    }
    fclose(editData->fptr_mp3);
    fclose(editData->fptr_new);
    return e_success;
}

Status check_edit_tag(char *argv[], EditData *editData)
{
    //store argv[2] to a character
    char op = argv[2][1];

    //skip 10 bytes
    fseek(editData -> fptr_mp3, 10, SEEK_SET);

    //take 4 bytes and check whether the tag is found or not
    while(fread(editData -> data_buff, sizeof(char), 4, editData -> fptr_mp3) == 4)
    {
        switch(op)
        {
            case 't':
            {
                if(strncmp(editData -> data_buff, "TIT2", 4) == 0)
                {
                    if(edit_data(argv, editData) == e_success)
                    {
                        printf("---------------- CHANGE THE TITLE ----------------\n");
                        printf("\n");
                        printf("TITLE   :   %s\n",argv[3]);
                        printf("\n");
                        printf("---------------- TITLE CHANGED SUCCESSFULLY ----------------\n");
                        return e_success;
                    }
                    else
                    {
                        printf("---------------- TITLE NOT FOUND ----------------\n");
                        return e_failure;
                    }

                }
            }
            break;
            case 'a':
            {
                if(strncmp(editData -> data_buff, "TPE1", 4) == 0)
                {
                    if(edit_data(argv, editData) == e_success)
                    {
                        printf("---------------- CHANGE THE ARTIST NAME ----------------\n");
                        printf("\n");
                        printf("ARTIST   :   %s\n",argv[3]);
                        printf("\n");
                        printf("----------- ARTIST NAME CHANGED SUCCESSFULLY -----------\n");
                        return e_success;
                    }
                    else
                    {
                        printf("---------------- ARTIST NOT FOUND ----------------");
                        return e_failure;
                    }

                }
            }
            break;
            case 'A':
            {
                if(strncmp(editData -> data_buff, "TALB", 4) == 0)
                {
                    if(edit_data(argv, editData) == e_success)
                    {
                        printf("---------------- CHANGE THE ALBUM NAME ----------------\n");
                        printf("\n");
                        printf("ALBUM   :   %s\n",argv[3]);
                        printf("\n");
                        printf("----------- ALBUM NAME CHANGED SUCCESSFULLY -----------\n");
                        return e_success;
                    }
                    else
                    {
                        printf("---------------- ALBUM NOT FOUND ----------------");
                        return e_failure;
                    }

                }
            }
            break;
            case 'y':
            {
                if(strncmp(editData -> data_buff, "TYER", 4) == 0)
                {
                    if(edit_data(argv, editData) == e_success)
                    {
                        printf("---------------- CHANGE THE YEAR ----------------\n");
                        printf("\n");
                        printf("YEAR   :   %s\n",argv[3]);
                        printf("\n");
                        printf("----------- YEAR CHANGED SUCCESSFULLY -----------\n");
                        return e_success;
                    }
                    else
                    {
                        printf("---------------- YEAR NOT FOUND ----------------");
                        return e_failure;
                    }

                }
            }
            break;
            case 'm':
            {
                if(strncmp(editData -> data_buff, "TCON", 4) == 0)
                {
                    if(edit_data(argv, editData) == e_success)
                    {
                        printf("---------------- CHANGE THE CONTENT ----------------\n");
                        printf("\n");
                        printf("CONTENT   :   %s\n",argv[3]);
                        printf("\n");
                        printf("----------- CONTENT CHANGED SUCCESSFULLY -----------\n");
                        return e_success;
                    }
                    else
                    {
                        printf("---------------- CONTENT NOT FOUND ----------------");
                        return e_failure;
                    }

                }
            
            }
            break;
            case 'c':
            {
                if(strncmp(editData -> data_buff, "TCOM", 4) == 0)
                {
                    if(edit_data(argv, editData) == e_success)
                    {
                        printf("---------------- CHANGE THE COMPOSER ----------------\n");
                        printf("\n");
                        printf("COMPOSER   :   %s\n",argv[3]);
                        printf("\n");
                        printf("----------- COMPOSER CHANGED SUCCESSFULLY -----------\n");
                        return e_success;
                    }
                    else
                    {
                        printf("---------------- COMPOSER NOT FOUND ----------------");
                        return e_failure;
                    }
                }
            }
            break;
        }
        // Seek back by 3 bytes 
        fseek(editData->fptr_mp3, -3, SEEK_CUR);
    }
    printf("----------- TAG NOT FOUND IN FILE -----------\n");
    return e_failure;
}

Status copy_first_bytes(int curr_pos, EditData *editData)
{
    //allocate memory for buffer
    char *buff = malloc(curr_pos);
    if (!buff) {
        fprintf(stderr, "Memory allocation failed\n");
        return e_failure;
    }


    // Go to the beginning of the file
    fseek(editData->fptr_mp3, 0, SEEK_SET);

    //read curr_pos data and copy
    fread(buff , sizeof(char), curr_pos - 4, editData -> fptr_mp3);
    
    fwrite(buff, sizeof(char),curr_pos - 4, editData -> fptr_new);

    free(buff);
    return e_success;

}

Status copy_size(int size, EditData *editData)
{
    //size array
    char size_arr[4];

    size_arr[0] = (size >> 21) & 0x7F;
    size_arr[1] = (size >> 14) & 0x7F;
    size_arr[2] = (size >> 7) & 0x7F;
    size_arr[3] = size & 0x7F;


    //copy size
    fwrite(size_arr, sizeof(char), 4, editData -> fptr_new);

    //copy flag
    char flag[2] = {0};
    fwrite(flag, sizeof(char), 2, editData -> fptr_new);

    return e_success;
}

Status copy_new(char *argv[], int size, EditData *editData)
{
    fputc(0x00, editData->fptr_new);

    //copy new 
    fwrite(argv[3], sizeof(char), size - 1, editData->fptr_new);

    return e_success;
}

unsigned int check_size_ed(EditData *editData)
{
    unsigned char size[4];

    // read 4 bytes data and store it in buff
    fread(size, sizeof(char), 4, editData -> fptr_mp3);


    //it will be stored in little endian therefore read from backwards
    unsigned int size_int = (size[0] << 21) | (size[1] << 14) | (size[2] << 7) | size[3];

    //printf("Size = %u\n",size);

    return size_int;
}

Status edit_data(char *argv[], EditData *editData)
{
    //take curr position and copy upto that
    long curr_pos = ftell(editData -> fptr_mp3);
    //printf("%ld",curr_pos);

    //Copy all bytes before the tag we're editing
    if (copy_first_bytes(curr_pos, editData) == e_failure)
    {
        printf("ERROR : Failed to copy initial bytes\n");
        return e_failure;
    }

    //skip tag name (4 bytes)
    fseek(editData->fptr_mp3, 4, SEEK_CUR);

    //read original tag size
    unsigned int old_size = check_size_ed(editData);

    //skip flag bytes (2 bytes)
    fseek(editData->fptr_mp3, 2, SEEK_CUR);

    //write new tag name again to new file
    fwrite(editData->data_buff, sizeof(char), 4, editData->fptr_new);

    //calculate the size of the new content
    int size = strlen(argv[3]) + 1;

    //copy size and flags
    if (copy_size(size, editData) == e_failure)
    {
        printf("ERROR : Failed to copy size and flag\n");
        return e_failure;
    }


    //copy the new tag content
    if (copy_new(argv, size, editData) == e_failure)
    {
        printf("ERROR : Failed to copy new data\n");
        return e_failure;
    }

    //skip the rest of the original frame
    fseek(editData->fptr_mp3, old_size, SEEK_CUR);


    //copy the remaining MP3 data
    int bytes;
    while (!feof(editData -> fptr_mp3))
    {
        bytes = fread(editData->data_buff, sizeof(char), sizeof(editData->data_buff), editData->fptr_mp3);
        fwrite(editData->data_buff, sizeof(char), bytes, editData->fptr_new);
    }

    return e_success;
}