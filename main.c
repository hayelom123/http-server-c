#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

//create a simple HTTP server that listens on port 8080 and serves a static HTML file (index.html) 
//in response to incoming HTTP requests. The server should handle multiple client connections sequentially
// and respond with the contents of the index.html file for any valid HTTP GET request.

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

char *read_file(const char *filename);
int main() {
      // Disable output buffering
  setbuf(stdout, NULL);
    int socket_fd = create_socket();
    bind_socket(socket_fd, "0.0.0.0", 8080);
    listen_for_connections(socket_fd, 5);
   printf("Server is listening on port http://localhost:8080 ...\n");
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
        printf("Method: %s, Path: %s\n", method, path);
        free(request);
        char *html = read_file("index.html");
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

int main1() {


 // Create a socket
    int socket_fd,client_addr_len;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket");
        printf("Socket creation failed: %s...\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
 // Define server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

      // Since the tester restarts your program quite often, setting REUSE_PORT
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) <
      0) {
    printf("SO_REUSEPORT failed: %s \n", strerror(errno));
    return 1;
  }

    // Bind the socket to the address
    if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections 
    int connection_backlog = 5; // 5 is backlog size which is the maximum number of pending connections that can be queued up before the server starts rejecting new connections.
    if (listen(socket_fd, connection_backlog) == -1) {
        perror("listen");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
        printf("Server is listening on port 8080...\n");
    
        // Accept a client connection
    int client_fd;
    while(1) {
    
    client_fd = accept(socket_fd, NULL, NULL);

    if (client_fd == -1) {
        perror("accept");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    char buffer[4096];

int bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

if (bytes > 0) {
    buffer[bytes] = '\0';

    char method[10], path[1024];
    sscanf(buffer, "%s %s", method, path);

    printf("Method: %s, Path: %s\n", method, path);
} else if (bytes == 0) {
    printf("Client disconnected.\n");
} else {
    perror("recv");
}
  printf("Client connected!\n");
 
// Send a welcome message to the client
char *html = read_file("index.html");
if (html) {

    // 1. Build HTTP header first
    char header[256];

    sprintf(header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %lu\r\n"
        "Connection: close\r\n"
        "\r\n",
        strlen(html)
    );

    // 2. Send header
    send(client_fd, header, strlen(header), 0);

    // 3. Send HTML body separately
    send(client_fd, html, strlen(html), 0);

    free(html);
}

 
    // Close the client socket
    close(client_fd);
}
    // Close the server socket
    close(socket_fd);   




    return 0;
}

 

char *read_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buffer = malloc(size + 1);
    if (!buffer) return NULL;

    fread(buffer, 1, size, fp);
    buffer[size] = '\0';

    fclose(fp);
    return buffer;
}
 

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
  
    // Print the client's IP address
    // change the code to print the client's IP address in a human-readable format
inet_ntop(AF_INET, &client_addr.sin_addr, (char[INET_ADDRSTRLEN]){0}, INET_ADDRSTRLEN);
printf("Client connected from %s:%d\n", (char[INET_ADDRSTRLEN]){0}, ntohs(client_addr.sin_port));
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