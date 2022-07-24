#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "log.h"


#ifdef __linux__ 
const char* home_dir = "HOME";
const char* file_name = "/.cache/Nessi.log";
#elif _WIN32 || _WIN64
const char* home_dir = "%APPDATA%";  
const char* file_name = "Local\\Nessi.log";
#endif

char* log_path = NULL;

void log_init(void) {
    char* get_home_dir = getenv(home_dir);
    
    if(get_home_dir == NULL) {
        fprintf(stderr,"Couldnt get Home Dir!");
        exit(-1);
    }

    size_t path_name_size = strlen(get_home_dir) + strlen(file_name) + 1;

    log_path = malloc(path_name_size * sizeof(char));

    if(!log_path) {
        fprintf(stderr,"Allocating Memory for Log Path failed!");
        exit(-1);
    }

    strcat(log_path,get_home_dir);
    strcat(log_path,file_name);
   
    FILE* log_file = fopen(log_path,"w");

    if(!log_file) {
        fprintf(stderr,"Could not create Log File!");
        exit(-1);
    }

    fclose(log_file);

}

int log_write(const char* file,int line,const char* format,...) {
    time_t current_time = time(NULL);
    struct tm* your_time = localtime(&current_time);

    va_list args;
    int done;
    FILE* log_file = fopen(log_path,"a");

    if(!log_file) {
        fprintf(stderr,"Can not write to Log File!");
        exit(-1);
    }
    
    fprintf(log_file,"%d:%d:%d:%s:%d:\t",your_time->tm_hour,your_time->tm_min,your_time->tm_sec,file,line);

    va_start(args,format);
    done = vfprintf(log_file,format,args);
    va_end(args);

    fclose(log_file);
    return done;
}
