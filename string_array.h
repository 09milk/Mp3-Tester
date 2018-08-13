/******************************************
    String array (header)
******************************************/

#ifndef STRING_ARRAY_H_
#define STRING_ARRAY_H_

#define INIT_STRING_LEN 5


#define REALLOC_FAIL -1
#define SUCCESS -2



typedef struct String_struct{
    char* string;
    size_t size;
    
} String;

typedef struct String_array_struct{
    String* data_array;
    size_t size;
    int count;
} String_array;


String_array* create_new_string_array(int init_size);
int string_array_append(String_array* array_p, const char* data);
int string_array_pop(String_array* array_p, char** removed_string);
void string_array_free(String_array* array_p);

#endif