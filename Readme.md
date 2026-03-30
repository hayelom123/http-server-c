<h1> This is how you start the server </h1>

```bash
mkdir build
cd build
cmake ..
make
./server
```

Note: The above code is a simple HTTP server that listens on port 8080.
It accepts incoming connections, reads the HTTP request, and responds with the contents of "index.html".
The server continues to run indefinitely, accepting new client connections until it is manually stopped.

step 1: Create a socket using the socket() system call.
This socket will be used to listen for incoming connections.

step 2: Define the server address using a sockaddr_in structure.
This includes setting the address family (AF_INET for IPv4), the IP address
(INADDR_ANY to accept connections on all interfaces), and the port number (8080 in this case).

step 3: Bind the socket to the defined address using the bind() system call.
This associates the socket with the specified IP address and port number.

step 4: Listen for incoming connections using the listen() system call.
This allows the server to accept connection requests from clients.
The second argument specifies the maximum number of pending connections that can be queued up before
the server starts rejecting new connections.

step 5: Accept incoming client connections in a loop using the accept() system call.
This creates a new socket for each accepted connection, which can be used to communicate with the client.

step 6: Read the HTTP request from the client using the recv() system call.
This allows the server to process the client's request and determine the appropriate response.

step 7: Build the HTTP response header, including the status line, content type, content length,
and connection status. This header is sent to the client before the actual HTML content.

step 8: Send the HTTP response header and the HTML content separately using the send() system call.
This allows the client to receive the response in a structured manner.

step 9: Close the client socket after sending the response to free up resources.
The server continues to accept new client connections until it is manually stopped.
Finally, the server socket is closed when the program is terminated.
