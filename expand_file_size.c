#include "expand_file_size.h"


long get_file_size(FILE* fp){
    rewind(fp);
    fseek(fp, 0, SEEK_END);
    long file_len = ftell(fp);
    rewind(fp);
    return file_len;
}

void set_file_size(FILE* fp, long original_size, long decided_size){
    if(decided_size > original_size){
        fseek(fp, decided_size , SEEK_SET);
        fputc('\0', fp);
    }
}