#pragma once

#define LOG_WRITE(...) log_write(__FILE__, __LINE__ ,__VA_ARGS__)
// Create a Log File in a OS specific Path
void log_init(void);

int log_write(const char* file,int line,const char* format,...);
