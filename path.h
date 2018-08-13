#include "string_array.h"


char get_self_path(char* exe_path, char** self_dir_path);
void get_dir_file_list(const char* target_path, String_array* s_array_p, 
                       char path_char);