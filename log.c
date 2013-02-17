#include "log.h"

#include <sys/time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

char* logfile = NULL;

void write_to_log(const char* format, ...)
{
  if (logfile) {
    FILE* f = fopen(logfile, "a");
    struct timeval tv;
    time_t nowtime;
    struct tm *nowtm;
    char tmbuf[64], buf[64];
    gettimeofday(&tv, NULL);
    nowtime = tv.tv_sec;
    nowtm = localtime(&nowtime);
    strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", nowtm);
    snprintf(buf, sizeof buf, "%s.%06ld", tmbuf, tv.tv_usec);
    fprintf(f, "[%s] ", buf);
    va_list arg;
    va_start(arg, format);
    vfprintf(f, format, arg);
    va_end(arg);
    fprintf(f, "\n");
    fflush(f);
    fclose(f);
  }
}