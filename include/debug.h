#pragma once

#include <stdio.h>

#define DEBUG_FILE "debug.log"

class DEBUG {
private:
    DEBUG();
    static DEBUG * _debug_instance;

    FILE * _debug_log_file;
public:
    DEBUG(const DEBUG &) = delete;
    void operator=(DEBUG &) = delete;
    
    static DEBUG * GetInstance();
    FILE * GetLogFile();

    ~DEBUG();
};

#ifdef DEBUG_MODE
    #define debug_printf(msg, ...) \
        do { \
            FILE * log_fd = DEBUG::GetInstance() != nullptr ? DEBUG::GetInstance()->GetLogFile() : nullptr; \
            if (log_fd == nullptr) log_fd = stderr; \
            fprintf(log_fd, "%s : %d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
            fflush(log_fd); \
        } while(0);
#else
    #define debug_printf(msg, ...) ((void)0)
#endif
