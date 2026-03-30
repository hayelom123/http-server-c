#include "../include/socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>      
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>


// create socket and return the file descriptor
int create_socket(){
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket");
        printf("Socket creation failed: %s...\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return socket_fd;
}
// bind the socket to the specified port and address
void bind_socket(int socket_fd,char *ip_address, int port){
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    server_addr.sin_port = htons(port);

    if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
}
// listen for incoming connections
void listen_for_connections(int socket_fd, int backlog){
    if (listen(socket_fd, backlog) == -1) {
        perror("listen");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
}
void print_client_address(struct sockaddr_in client_addr) {
    char client_address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_address, INET_ADDRSTRLEN);
    printf("Client connected from %s:%d\n", client_address, ntohs(client_addr.sin_port));
}
// accept a client connection and return the client socket file descriptor
int accept_connection(int socket_fd){
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd == -1) {
        perror("accept");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    print_client_address(client_addr);
//     char client_address[1024];
//         // Print the client's IP address
//     // change the code to print the client's IP address in a human-readable format
// inet_ntop(AF_INET, &client_addr, client_address, 1024);
// printf("Client connected from %s:%d\n", client_address, ntohs(client_addr.sin_port));
 
// printf("Client connected from %s:%d\n", client_address, ntohs(client_addr.sin_port));
    return client_fd;
}

// read the HTTP request from the client
char *read_request(int client_fd){
    char *buffer = malloc(1024);
    if (!buffer) {
        perror("malloc");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    ssize_t bytes_received = recv(client_fd, buffer, 1024, 0);
    if (bytes_received == -1) {
        perror("recv");
        free(buffer);
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';
    return buffer;
}
// parse the HTTP request and extract the requested path
void parse_request(const char *request, char *method, char *path){
    sscanf(request, "%s %s", method, path);
}
// build the HTTP response header and body
void build_response(const char *html, char *response_header, size_t header_size){
    snprintf(response_header, header_size, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %lu\r\nConnection: close\r\n\r\n", strlen(html));
}
// send the HTTP response header and body to the client
void send_response(int client_fd, const char *response_header, const char *html){
    send(client_fd, response_header, strlen(response_header), 0);
    send(client_fd, html, strlen(html), 0);
}
// close the client socket
void close_client_socket(int client_fd){
    close(client_fd);
}
// close the server socket when the program is terminated
void close_server_socket(int socket_fd){
    close(socket_fd);
}