#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int create_socket();
void bind_socket(int socket_fd, char *ip_address, int port);
void listen_for_connections(int socket_fd, int backlog);
int accept_connection(int socket_fd);
char *read_request(int client_fd);
void parse_request(const char *request, char *method, char *path);
void build_response(const char *html, char *response_header, size_t header_size);
void send_response(int client_fd, const char *response_header, const char *html);
void close_client_socket(int client_fd);
void close_server_socket(int socket_fd);

#endif // SOCKET_H