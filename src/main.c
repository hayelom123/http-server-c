#include <stdio.h>
#include <stdlib.h>
#include "../include/log.h"
#include "../include/socket.h"
#include "file.h"
 
int main() {
    log_init();
    log_info("Server started on port %d", 8080);
    log_info_file( "access.log", "Client connected: fd=%d", 5);
    log_error_file( "error.log", "Failed to bind socket");
    log_warn("High memory usage detected");
// log_info("Server started on http://localhost:%d", 8080);
// log_warn("High memory usage: %d MB", 512);
// log_error("Failed to bind socket");
 
  // Disable output buffering
  setbuf(stdout, NULL);
    int socket_fd = create_socket();
    bind_socket(socket_fd, "0.0.0.0", 8080);
    listen_for_connections(socket_fd, 5);
    //log_info("Server is listening on port http://localhost:8080 ...\n");
    log_info("Server started on http://localhost:%d", 8080);

    while(1) {
        int client_fd = accept_connection(socket_fd);
        if (client_fd == -1) {
            perror("accept");
            close_server_socket(socket_fd);
            exit(EXIT_FAILURE);
        }
        char *request = read_request(client_fd);
        char method[10], path[1024];
        parse_request(request, method, path);
        log_info("Method: %s, Path: %s\n", method, path);
        free(request);
        char *html = read_file("../public/index.html");
        if (html) {
            char response_header[256];
            build_response(html, response_header, sizeof(response_header));
            send_response(client_fd, response_header, html);
            free(html);
        }

        // Handle client request
        close_client_socket(client_fd);
    }
    close_server_socket(socket_fd);
    return 0;
}