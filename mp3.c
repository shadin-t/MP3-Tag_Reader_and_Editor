#include "mp3.h"

int count = 0 ;
OperationType check_operation_type(char *argv[])
{
    
    if(strcmp(argv[1], "-v") == 0)
    {
        return e_view ;
    }
    else if (strcmp(argv[1], "-e") == 0)
    {
        return e_edit ;
    }
    else if (strcmp(argv[1], "--help") == 0)
    {
        return e_help ;
    }
    else
    {
        return e_unsupported ;
    }
}
Status read_and_validate_for_view(char *argv[] , Mp3TagInfo *mp3Info)
{
    if (argv[2] == NULL)
    {
        printf("Invalid Arguments\n");
        return e_failure ;
    }
    else if( strstr(argv[2] , ".mp3") != NULL)
    {
        mp3Info->mp3_file_name = argv[2] ;
        // printf("Mp3 filename = %s\n", mp3Info->mp3_file_name);
        return e_success;
    }
    else
    {
        return e_failure ;
    }
}
Status open_files(Mp3TagInfo *mp3Info)
{
    mp3Info->fptr_mp3_file = fopen(mp3Info->mp3_file_name , "r");
    if (mp3Info->fptr_mp3_file == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", mp3Info->mp3_file_name);

    	return e_failure;
    }
    return e_success ;
}

Status check_id3(Mp3TagInfo *mp3Info)
{
    char buffer[3];
    fread(buffer , 3 , 1 , mp3Info->fptr_mp3_file);
    
    // printf("File ID3 or not = %s\n",buffer);
    if(strcmp(buffer , "ID3") == 0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
    
}
Status check_version(Mp3TagInfo *mp3Info)
{
    char buffer[2];
    fread(buffer , 2 , 1 , mp3Info->fptr_mp3_file);
    //printf("Version = %s\n",buffer);
    
    return e_success;
    /*if(strcmp(buffer , "") == 0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }*/

}
int convert_big_to_little(int size)
{
    int i;
    char temp ;
    char *ptr = (char *)&size ;
    for(i = 0 ; i < 2 ; i++)
    {
        temp = ptr[i];
        ptr[i] = ptr[4 - 1 - i] ;
        ptr[4 - 1 - i]  = temp ;
    }
    return size ;

}
Status check_tags(Mp3TagInfo *mp3Info)
{
    char buffer[5];
    if(fread(buffer , 4 , 1, mp3Info->fptr_mp3_file) != 1)
    {
        return e_failure;
    }
    buffer[4] = '\0';
    // printf("tag = %s\n",buffer);
    mp3Info->Tag = malloc(5);
    strcpy(mp3Info->Tag ,buffer);
    int size , res;
    if(fread(&size , 4 , 1, mp3Info->fptr_mp3_file) != 1)
    {
        return e_failure;
    }
    res = convert_big_to_little(size);
    if(res <= 0 || res >= 1024)
    {
        return e_failure;
    }
    mp3Info->size = res ;

    return e_success;
}
Status print_content(Mp3TagInfo *mp3Info)
{
    fseek(mp3Info->fptr_mp3_file , 3, SEEK_CUR);
    mp3Info->content = malloc(mp3Info->size);
    fread(mp3Info->content , mp3Info->size - 1 , 1 , mp3Info->fptr_mp3_file);
    mp3Info->content[mp3Info->size - 1] = '\0';
    return e_success;


}
Status view(Mp3TagInfo *mp3Info)
{
    // printf("Open files\n");
    if(open_files(mp3Info) == e_success)
    {
        // printf("files Opened\n");
        if(check_id3(mp3Info) == e_success)
        {
            // printf("ID3 checked = %ld\n",ftell(mp3Info->fptr_mp3_file));
            if (check_version (mp3Info) == e_success)
            {
                // printf("Version checked = %ld\n",ftell(mp3Info->fptr_mp3_file));
                fseek(mp3Info->fptr_mp3_file ,10 , SEEK_SET);
                while(count != 6 && !feof(mp3Info->fptr_mp3_file))
                {
                    if(check_tags(mp3Info) == e_success)
                    {
                        print_content(mp3Info);
                        //printf("TAG = %s\n",mp3Info ->Tag);
                        // printf("CONTENT = %s\n",mp3Info->content);
                        

                        if(strcmp(mp3Info->Tag , "TIT2") == 0)
                        {
                            count++ ;
                            printf("TITLE\t:\t%s\n",mp3Info->content);
                        }
                        else if(strcmp(mp3Info->Tag , "TPE1") == 0)
                        {
                            count++ ;
                            printf("ARTIST\t:\t%s\n",mp3Info->content);
                        }
                        else if(strcmp(mp3Info->Tag , "TALB") == 0)
                        {
                            count++ ;
                            printf("ALBUM\t:\t%s\n",mp3Info->content);
                        }
                        else if(strcmp(mp3Info->Tag , "TYER") == 0)
                        {
                            count++ ;
                            printf("YEAR\t:\t%s\n",mp3Info->content);
                        }
                        else if(strcmp(mp3Info->Tag , "TCON") == 0)
                        {
                            count++ ;
                            printf("MUSIC\t:\t%s\n",mp3Info->content);
                        }
                        else if(strcmp(mp3Info->Tag , "COMM") == 0)
                        {
                            count++ ;
                            printf("COMMENT\t:\t%s\n",mp3Info->content);
                        }
                    }
                }
            }
        }
    }
}