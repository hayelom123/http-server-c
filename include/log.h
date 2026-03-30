#ifndef LOG_H
#define LOG_H

#define LOG_DIR "../logs/"

void log_info(const char *format, ...);
void log_warn(const char *format, ...);
void log_error(const char *format, ...);

void log_info_file(const char *filename, const char *format, ...);
void log_warn_file(const char *filename, const char *format, ...);
void log_error_file(const char *filename, const char *format, ...);

void log_errno(const char *msg);

void log_init();

/**
 * Usage examples:
 * log_info("Server started on http://localhost:%d", 8080);
 * log_warn("High memory usage: %d MB", 512);
 * log_error("Failed to bind socket");
 * log_info_file("server.log", "Server started on http://localhost:%d", 8080);
 * log_warn_file("server.log", "High memory usage: %d MB", 512);
 * log_error_file("server.log", "Failed to bind socket");
 */

#endif // LOG_H