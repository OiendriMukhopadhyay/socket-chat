#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUF_SIZE 4096

// Helper to URL-decode (replace + with space, handle %xx)
void url_decode(char *str) {
    char *p = str, *q = str;
    while (*p) {
        if (*p == '+') {
            *q++ = ' ';
        } else if (*p == '%' && p[1] && p[2]) {
            char hex[3] = {p[1], p[2], '\0'};
            *q++ = (char) strtol(hex, NULL, 16);
            p += 2;
        } else {
            *q++ = *p;
        }
        p++;
    }
    *q = '\0';
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[BUF_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    // Listen
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server running on http://localhost:%d\n", PORT);

    while (1) {
        addr_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, BUF_SIZE);

        // Extract POST data
        char *data = strstr(buffer, "\r\n\r\n");
        if (!data) {
            close(client_fd);
            continue;
        }
        data += 4;

        // Parse parameters
        char type[20] = "", message[1024] = "";
        sscanf(data, "type=%19[^&]&message=%1023[^\r\n]", type, message);

        url_decode(type);
        url_decode(message);

        printf("Received type: %s | message: %s\n", type, message);

        char response[BUF_SIZE];
        FILE *chatFile;

        if (strcmp(type, "Reader") == 0) {
            // --- Reader: Show file content ---
            chatFile = fopen("chat.txt", "r");
            char chatContent[2048] = "";
            if (chatFile) {
                char line[256];
                while (fgets(line, sizeof(line), chatFile))
                    strcat(chatContent, line);
                fclose(chatFile);
            } else {
                strcpy(chatContent, "No messages yet!");
            }

            sprintf(response,
                "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                "<html><body style='font-family:Arial;text-align:center;'>"
                "<h2>Reader Connected</h2><h3>Chat Messages:</h3>"
                "<pre>%s</pre><br><a href='/'>Go Back</a></body></html>",
                chatContent);
        } 
        else if (strcmp(type, "Writer") == 0) {
            // --- Writer: Append user message ---
            chatFile = fopen("chat.txt", "a");
            if (chatFile) {
                fprintf(chatFile, "Writer: %s\n", message[0] ? message : "(no message)");
                fclose(chatFile);
            }

            sprintf(response,
                "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                "<html><body style='font-family:Arial;text-align:center;'>"
                "<h2>Writer Connected</h2>"
                "<p>Your message has been added to chat.txt!</p>"
                "<a href='/'>Go Back</a></body></html>");
        } 
        else {
            sprintf(response,
                "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n"
                "<html><body><h3>Invalid client type!</h3></body></html>");
        }

        write(client_fd, response, strlen(response));
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
