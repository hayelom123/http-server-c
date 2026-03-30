#include "../include/log.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <errno.h>

#define COLOR_GREEN  "\x1b[32m"
#define COLOR_RED    "\x1b[31m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RESET  "\x1b[0m"

static void log_time(char *buf, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", t);
}

/* =========================
   CONSOLE LOGS
   ========================= */

void log_info(const char *format, ...) {
    char time_str[20];
    log_time(time_str, sizeof(time_str));

    printf("%s[%s] [INFO] ", COLOR_GREEN, time_str);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("%s\n", COLOR_RESET);
}

void log_warn(const char *format, ...) {
    char time_str[20];
    log_time(time_str, sizeof(time_str));

    printf("%s[%s] [WARN] ", COLOR_YELLOW, time_str);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("%s\n", COLOR_RESET);
}

void log_error(const char *format, ...) {
    char time_str[20];
    log_time(time_str, sizeof(time_str));

    printf("%s[%s] [ERROR] ", COLOR_RED, time_str);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("%s\n", COLOR_RESET);
}

/* =========================
   FILE LOGGING CORE
   ========================= */

static void log_to_file(const char *filename,
                        const char *level,
                        const char *format,
                        va_list args) {
    char path[256];
    snprintf(path, sizeof(path), "%s%s", LOG_DIR, filename);
    FILE *f = fopen(path, "a");
    if (!f) {
        perror("fopen log file");
        return;
    }

    char time_str[20];
    log_time(time_str, sizeof(time_str));

    fprintf(f, "[%s] [%s] ", time_str, level);

    va_list copy;
    va_copy(copy, args);
    vfprintf(f, format, copy);
    va_end(copy);

    fprintf(f, "\n");

    fclose(f);
}

/* =========================
   FILE LOG FUNCTIONS
   ========================= */

void log_info_file(const char *filename, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_to_file(filename, "INFO", format, args);
    va_end(args);
}

void log_warn_file(const char *filename, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_to_file(filename, "WARN", format, args);
    va_end(args);
}

void log_error_file(const char *filename, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_to_file(filename, "ERROR", format, args);
    va_end(args);
}

/* =========================
   OPTIONAL: errno helper
   ========================= */

void log_errno(const char *msg) {
    fprintf(stderr, "[ERROR] %s: %s\n", msg, strerror(errno));
}




static void ensure_logs_dir(const char *path) {
    struct stat st = {0};

    if (stat(path, &st) == -1) {
        if (mkdir(path, 0755) == -1) {
            perror("mkdir logs");
        }
    }
}
void log_init() {
    ensure_logs_dir( LOG_DIR);
}