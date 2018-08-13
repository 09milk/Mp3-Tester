/******************************************
    String array
******************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "string_array.h"


#define DATA_ARRAY (array_p->data_array)


static void free_data_array(String_array* array_p, int i);



String_array* create_new_string_array(int init_size){
    if (init_size < 1){
        return NULL;
    }
    
    int i;
    
    String_array* new_array_p = malloc(sizeof(String_array));
    if (new_array_p == NULL){
        return NULL;
    }
    
    new_array_p->data_array = malloc(sizeof(String) * init_size);
    if (new_array_p->data_array == NULL){
        free(new_array_p);
        return NULL;
    }
    for (i=0; i < init_size; i++){
        (new_array_p->data_array[i]).size = INIT_STRING_LEN;
        (new_array_p->data_array[i]).string = calloc(INIT_STRING_LEN,
                                                     sizeof(char)); //malloc
        if ((new_array_p->data_array[i]).string == NULL){
            free_data_array(new_array_p, i);
            return NULL;
        }
        strcpy((new_array_p->data_array[i]).string, "");
    }
    
    new_array_p->size = init_size;
    new_array_p->count = 0;
    
    return new_array_p;
}

static void free_data_array(String_array* array_p, int i){
    int j;
    for (j=i; j >= 0; j--){
        free((DATA_ARRAY[i]).string);
    }
    free(DATA_ARRAY);
    free(array_p);
}

//return status
int string_array_append(String_array* array_p, const char* data){
    int i;
    if (array_p->count == array_p->size){
        String* new_array_p = realloc(DATA_ARRAY, 
                                      array_p->size * 2 * sizeof(String));
        if (new_array_p == NULL){
            return REALLOC_FAIL;
        }else{
            DATA_ARRAY = new_array_p;
        }
        
        for(i = array_p->size; i < (array_p->size * 2); i++){
            DATA_ARRAY[i].size = INIT_STRING_LEN;
            DATA_ARRAY[i].string = calloc(INIT_STRING_LEN,
                                          sizeof(char)); // alloc coz realloc
        }
        
        array_p->size *= 2;
    }
    
    if ((DATA_ARRAY[array_p->count]).size < (strlen(data) + 1)){ //+1 for '\0'
        char* new_string_p = 
                realloc(DATA_ARRAY[array_p->count].string, 
                        (strlen(data) + 1) * sizeof(char));
        if (new_string_p == NULL){
            return REALLOC_FAIL;
        }else{
            DATA_ARRAY[array_p->count].string = new_string_p;
        }
        DATA_ARRAY[array_p->count].size = (strlen(data) + 1);
    }
    strcpy(DATA_ARRAY[array_p->count].string, data);
    array_p->count++;
    
    return SUCCESS;
}

//need melloc for removed_string, return status
int string_array_pop(String_array* array_p, char** removed_string){
    if(array_p->count == 0){
        *removed_string = NULL;
        return 0;
    }
    array_p->count--;
    char* last_string = DATA_ARRAY[array_p->count].string;
    char* new_string_p = realloc(*removed_string, 
                                 (strlen(last_string)+1) * sizeof(char));
    if (new_string_p == NULL){
        return REALLOC_FAIL;
    }else{
        *removed_string = new_string_p;
    }
    strcpy(*removed_string, last_string);
    strcpy(DATA_ARRAY[array_p->count].string, "");
    return SUCCESS;
}

void string_array_free(String_array* array_p){
    free_data_array(array_p, (array_p->size - 1));
}














