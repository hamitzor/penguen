#include "log.h"
#include <stdio.h>
#include <time.h>

void pgn_log_now()
{
    time_t timer;
    char buffer[26];
    struct tm *tm_info;
    timer = time(NULL);
    tm_info = localtime(&timer);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(stdout, "[%s]", buffer);
}