# Socket-Based Web Chat Application

A beginner-friendly client-server chat application built with **C sockets, TCP, HTTP, HTML, and JavaScript**.

The project demonstrates how a C program can act as a lightweight HTTP server and communicate with a browser-based client over a TCP socket.

## Features

- TCP socket communication using C
- Lightweight HTTP request handling without external frameworks
- Browser-based interface
- Two client modes:
  - **Writer** — sends a message
  - **Reader** — reads stored chat messages
- URL decoding for form data
- Persistent message storage in `chat.txt`
- Simple HTTP responses generated directly by the C server

## Project Structure

```text
socket-chat/
├── src/
│   ├── server.c       # TCP/HTTP server written in C
│   └── index.html     # Browser-based client interface
├── docs/
│   └── architecture.md
├── chat.txt           # Sample chat data
├── Makefile           # Build and run shortcuts
├── .gitignore
├── LICENSE
└── README.md
```

## How It Works

```text
Browser
   |
   | HTTP POST
   v
C Socket Server
   |
   +---- Writer ----> append message to chat.txt
   |
   +---- Reader ----> read chat.txt
   |
   v
HTTP Response
   |
   v
Browser
```

## Requirements

- Linux, WSL, or another POSIX-compatible environment
- GCC
- A modern web browser

The code uses POSIX socket headers such as `unistd.h` and `arpa/inet.h`, so native Windows compilation may require WSL or another compatibility environment.

## Run the Project

### 1. Clone the repository

```bash
git clone https://github.com/YOUR-USERNAME/socket-chat.git
cd socket-chat
```

### 2. Compile

```bash
make
```

or:

```bash
gcc src/server.c -o server
```

### 3. Start the server

```bash
./server
```

The server listens on:

```text
http://localhost:5000
```

### 4. Open the web client

Open `src/index.html` in your browser.

Choose:

- **Writer** to enter and save a message.
- **Reader** to display the messages stored in `chat.txt`.

## Technical Concepts Demonstrated

- TCP/IP socket programming
- `socket()`, `bind()`, `listen()`, and `accept()`
- HTTP request/response basics
- POST form data parsing
- URL decoding
- File handling in C
- Browser-to-server communication
- Basic client-server architecture

## Important Limitations

This is an educational project, not a production-ready chat server.

Current limitations include:

- Single-process sequential request handling
- No authentication or authorization
- No encryption/TLS
- Basic HTTP parsing
- No database
- Limited input validation
- Messages are stored as plain text
- Designed for local demonstration

These limitations are intentional opportunities for future improvement.

## Future Improvements

- Add multi-client/threaded support
- Add proper HTTP request parsing
- Add message timestamps and usernames
- Replace text-file storage with SQLite
- Add input validation and HTML escaping
- Add authentication
- Add HTTPS/TLS
- Add a REST API
- Improve the frontend UI
- Add automated tests
- Add Docker support

## Learning Outcome

This project demonstrates the fundamentals of low-level network programming and shows how a browser can communicate with a custom C server using TCP and HTTP.

## Author

**Banasree Maji**

B.Tech in Computer Science & Engineering

---

> Educational project created to understand socket programming, HTTP communication, and basic client-server architecture.
