# Architecture

## Components

### 1. Browser Client

`src/index.html` provides the user interface. It allows the user to select `Reader` or `Writer` and, for Writer mode, enter a message.

### 2. C Server

`src/server.c` creates a TCP socket on port `5000`, accepts browser connections, reads the HTTP request, extracts POST parameters, and generates an HTTP response.

### 3. File Storage

`chat.txt` is used as simple persistent storage.

- Writer mode appends a message.
- Reader mode reads the stored messages.

## Request Flow

```text
User
 |
 v
index.html
 |
 | HTTP POST
 v
TCP socket on localhost:5000
 |
 v
server.c
 |
 +--> Writer --> chat.txt (append)
 |
 +--> Reader --> chat.txt (read)
 |
 v
HTTP response
 |
 v
Browser
```

## Key C Networking APIs

- `socket()` — creates the socket
- `bind()` — assigns the local port
- `listen()` — waits for incoming connections
- `accept()` — accepts a client connection
- `read()` — receives request data
- `write()` — sends the response
- `close()` — closes the connection
