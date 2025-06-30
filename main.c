#include <stdio.h>
#include "mp3.h"
#include "edit.h"

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        if(check_operation_type(argv) == e_view)
        {
            
            Mp3TagInfo mp3Info ;
            if(read_and_validate_for_view(argv ,&mp3Info) == e_success)
            {
                printf("\n--------------------SELECTED VIEW DETAILS----------------------\n");
                printf("\n---------------------------------------------------------------\n");
                printf("\t\tMP3 TAG READER AND EDITOR FOR ID3v2\t");
                printf("\n---------------------------------------------------------------\n\n");
                view(&mp3Info);
                printf("\n----------------------------------------------------------------\n");
                printf("\n-----------------DETAILS DISPLAYED SUCCESSFULLY-----------------\n");
            }
            else
            {
                printf("Error\n");
            }
        }
        else if(check_operation_type(argv) == e_edit)
        {
            EditData editData ;
            if(validate_edit_args(argv ,&editData) == e_success)
            {
                printf("------------------------------ SELECTED EDIT DETAILS -------------------------------\n");
                printf("\n");
                printf("------------------------------ SELECTED EDIT OPTION --------------------------------\n");
                printf("\n");

                if(go_edit_data(argv, &editData) == e_success)
                {
                    remove(editData.mp3name);
                    rename(editData.newfile, editData.mp3name);
                }
                printf("\n");
                printf("------------------------------ EDITING IS SUCCESSFULL ------------------------------\n");
            }
            
            
        }
        else if(check_operation_type(argv) == e_help)
        {
            printf("\n---------------HELP MENU---------------\n");
            printf("\n1. -v -> To view MP3 file contents\n2. -e -> To edit MP3 file contents\n");
            printf("\t2.1 -t -> To edit song title\n\t2.2 -a -> To edit artist name\n\t2.3 -A -> To edit album name\n\t2.4 -y -> To edit year\n\t2.5 -m -> To edit content\n\t2.6 -c -> To edit comment\n");
            printf("\n---------------------------------------\n");
        }
    }
    else
    {
        printf("Pass more arguments\n");
    }
}