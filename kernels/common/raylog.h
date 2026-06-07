/* Ray instrumentation log */
#pragma once
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

static FILE* raylog_file = NULL;

static inline void raylog_open() {
    if (raylog_file) return;
    time_t t = time(NULL);
    struct tm* lt = localtime(&t);
    char path[256];
    strftime(path, sizeof(path), "raytrace_%Y-%m-%d_%H-%M-%S.log", lt);
    raylog_file = fopen(path, "w");
    if (raylog_file) {
        fprintf(raylog_file, "# Ray trace log started %s", ctime(&t));
        fflush(raylog_file);
    }
}

static inline void raylog(const char* file, int line, const char* func,
                          const char* fmt, ...) {
    raylog_open();
    if (!raylog_file) return;
    fprintf(raylog_file, "%s:%d %s | ", file, line, func);
    va_list args;
    va_start(args, fmt);
    vfprintf(raylog_file, fmt, args);
    va_end(args);
    fprintf(raylog_file, "\n");
    fflush(raylog_file);
}

static inline void raylog_close() {
    if (raylog_file) { fclose(raylog_file); raylog_file = NULL; }
}
