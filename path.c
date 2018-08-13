#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


#include "path.h"

//remember to free self_dir_path
char get_self_path(char* exe_path, char** self_dir_path){
    int i;
    char path_char;
    for (i = strlen(exe_path); i > 0; i--){
        if (exe_path[i] == '/'){
            path_char='/';
            break;
        }else if (exe_path[i] == '\\'){
            path_char='\\';
            break;
        }
    }
    
    
    *self_dir_path = calloc(i + 2, sizeof(char)); //+2 for '/' & '\0' (malloc)
    strncpy(*self_dir_path, exe_path, i);
    sprintf(*self_dir_path, "%s%c", *self_dir_path, path_char);
    
    return path_char;
}

void get_dir_file_list(const char* target_path, String_array* s_array_p, 
                       char path_char){
    DIR *dir;
    struct dirent* ent;
    if ((dir = opendir(target_path)) != NULL){
        while ((ent = readdir(dir)) != NULL){
            if (ent->d_type != DT_DIR){
                int path_len = strlen(ent->d_name) + 1;
                char* target_file_path = malloc(path_len * sizeof(char));
                sprintf(target_file_path, "%s", ent->d_name);
                
                string_array_append(s_array_p, target_file_path);
                
                free(target_file_path);
            }
        }
        closedir(dir);
    }
}