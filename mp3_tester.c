#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <string.h>

#include "expand_file_size.h"
#include "path.h"
#include "string_array.h"


#define CMD_LEN 500
#define FFMPEG "ffmpeg.exe -hide_banner -loglevel error"
#define ERROR_MKDIR -1


void get_wav_file_list(String_array* wav_file_list, 
                       String_array* dir_file_list);
void create_sound_folder();
void fill_new_folders(String_array* wav_file_list);
void generate_new_mp3_and_wav(char* filename);
void generate_quiz_wav(char* filename, char* real_wav, int i);
void generate_ans(char* ans, String_array* wav_file_list);
void normalize_size(char* filename);



int main(int argc, char* argv[]){
    char* self_dir_path;
    
    char path_char = get_self_path(argv[0], &self_dir_path);
    
    String_array* dir_file_list = create_new_string_array(10);
    get_dir_file_list(self_dir_path, dir_file_list, path_char);
    
    String_array* wav_file_list = create_new_string_array(10);
    get_wav_file_list(wav_file_list, dir_file_list);
    
    create_sound_folder();
    
    fill_new_folders(wav_file_list);
    
    free(self_dir_path);
    string_array_free(wav_file_list);
    string_array_free(dir_file_list);
    
    printf("\nFinished\n\n");
    
    system("pause");
    return 0;
}


void get_wav_file_list(String_array* wav_file_list, 
                       String_array* dir_file_list){
    int i;
    for (i=0; i < dir_file_list->count; i++){
        char* path = (dir_file_list->data_array)[i].string;
        if (strstr(path, ".wav") != NULL){
            string_array_append(wav_file_list, path);
        }
    }
}

void create_sound_folder(){
    if (opendir("quiz_wav") || opendir("unmodified_wav_and_mp3")){
        fprintf(stderr, "Plz delete both folders \"quiz_wav\" & "
                        "\"unmodified_wav_and_mp3\" \n");
        exit(ERROR_MKDIR);
    }
    system("mkdir quiz_wav > nul 2> nul");
    system("mkdir unmodified_wav_and_mp3 > nul 2> nul");
}

void fill_new_folders(String_array* wav_file_list){
    int i;
    char* real_wav = malloc(wav_file_list->count * sizeof(char));
    srand(time(NULL));
    for (i=0; i < wav_file_list->count; i++){
        char* filename = (wav_file_list->data_array)[i].string;
        filename[strlen(filename)-4] = '\0';
        
        generate_new_mp3_and_wav(filename);
        
        generate_quiz_wav(filename, real_wav, i);
        
        normalize_size(filename);
    }
    generate_ans(real_wav, wav_file_list);
    
    free(real_wav);
}

void generate_new_mp3_and_wav(char* filename){
    char* cmd = malloc( (CMD_LEN + strlen(filename)*2) * sizeof(char) );
    
    sprintf(cmd, "%s -i \"%s.wav\" -map_metadata -1 "
                 "\"unmodified_wav_and_mp3/%s.wav\" ", 
            FFMPEG, filename, filename);
    printf("%s\n", cmd);
    system(cmd);
    
    sprintf(cmd, "%s -i \"%s.wav\" -ab 320k "
                 "\"unmodified_wav_and_mp3/%s.mp3\" ", 
            FFMPEG, filename, filename);
    printf("%s\n", cmd);
    system(cmd);
    
    sprintf(cmd, "%s -i \"unmodified_wav_and_mp3/%s.mp3\" -map_metadata -1 "
                 "\"unmodified_wav_and_mp3/%s_mp3.wav\" ", 
            FFMPEG, filename, filename);
    printf("%s\n", cmd);
    system(cmd);
    
    free(cmd);
}

void generate_quiz_wav(char* filename, char* real_wav, int i){
    char* cmd = malloc( (CMD_LEN + strlen(filename)*2) * sizeof(char) );
    float rng = rand()/(float)RAND_MAX;
    
    real_wav[i] = rng < 0.5 ? 'A' : 'B';
    
    if (real_wav[i] == 'A'){
        sprintf(cmd, "cp \"unmodified_wav_and_mp3/%s.wav\" "
                     "\"quiz_wav/%s_A.wav\"", 
                filename, filename);
        printf("%s\n", cmd);
        system(cmd);
        
        sprintf(cmd, "cp \"unmodified_wav_and_mp3/%s_mp3.wav\" "
                     "\"quiz_wav/%s_B.wav\"", 
                filename, filename);
        printf("%s\n", cmd);
        system(cmd);
    }else if (real_wav[i] == 'B'){
        sprintf(cmd, "cp \"unmodified_wav_and_mp3/%s.wav\" "
                     "\"quiz_wav/%s_B.wav\"", 
                filename, filename);
        printf("%s\n", cmd);
        system(cmd);
        
        sprintf(cmd, "cp \"unmodified_wav_and_mp3/%s_mp3.wav\" "
                     "\"quiz_wav/%s_A.wav\"", 
                filename, filename);
        printf("%s\n", cmd);
        system(cmd);
    }
    
    free(cmd);
}

void generate_ans(char* ans, String_array* wav_file_list){
    int i;
    system("rm -f quiz_wav/ANSWER.txt");
    FILE* ans_file = fopen("quiz_wav/ANSWER.txt", "a");
    for (i=0; i < wav_file_list->count; i++){
        char* wav_name = (wav_file_list->data_array)[i].string;
        fprintf(ans_file, "%s: %c\n", wav_name, ans[i]);
    }
    fclose(ans_file);
}

void normalize_size(char* filename){
    char real_filename[CMD_LEN];
    sprintf(real_filename, "quiz_wav/%s_A.wav", filename);
    FILE* f_a = fopen(real_filename, "rb+");
    sprintf(real_filename, "quiz_wav/%s_B.wav", filename);
    FILE* f_b = fopen(real_filename, "rb+");
    
    long f_a_size = get_file_size(f_a);
    long f_b_size = get_file_size(f_b);
    
    long new_size = f_a_size > f_b_size ? f_a_size + 1 : f_b_size + 1;
    
    set_file_size(f_a, f_a_size, new_size);
    set_file_size(f_b, f_b_size, new_size);
    
    fclose(f_a);
    fclose(f_b);
}











