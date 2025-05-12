#include "httpd.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUF_SIZE 1024

typedef struct {
    char username[64];
    char password[64];
} User;

User users[2] = {
    {"adan", "1234"},
    {"ayah", "5678"}
};
int user_count = 2;

void show_users() {
    printf("USERNAME | PASSWORD\n");
    for (int i = 0; i < user_count; i++) {
        printf("%s | %s\n", users[i].username, users[i].password);
    }
}

void route()
{
    ROUTE_START()

    ROUTE_POST("/login")
    {
        char *user_start = strstr(payload, "sername=");
        char *pass_start = strstr(payload, "password=");

        if (user_start && pass_start) {
            char username[64] = {0}, password[64] = {0};
            sscanf(user_start, "sername=%63[^&]", username);
            sscanf(pass_start, "password=%63[^&]", password);

            int found = 0;
            for (int i = 0; i < user_count; i++) {
                if (strcmp(users[i].username, username) == 0 &&
                    strcmp(users[i].password, password) == 0) {
                    found = 1;
                    break;
                }
            }

            const char* page = found ? "success.html" : "fail.html";
            int fd = open(page, O_RDONLY);
            printf("HTTP/1.1 200 OK\r\n\r\n");
            char buffer[BUF_SIZE];
            int n;
            while ((n = read(fd, buffer, BUF_SIZE)) > 0)
                write(STDOUT_FILENO, buffer, n);
            close(fd);
        } else {
            printf("HTTP/1.1 400 Bad Request\r\n\r\nMissing credentials\n");
        }
    }

    ROUTE_GET("/")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        int fd = open("page1.html", O_RDONLY);
        int n;
        char buffer[BUF_SIZE];
        while ((n = read(fd, buffer, BUF_SIZE)) > 0)
            write(STDOUT_FILENO, buffer, n);
        close(fd);
    }
ROUTE_GET("/lion_sleeping.jpg")
{
    int fd = open("lion_sleeping.jpg", O_RDONLY);
    if (fd < 0) {
        printf("HTTP/1.1 404 Not Found\r\n\r\nImage not found.");
        return;
    }

    printf("HTTP/1.1 200 OK\r\n");
    printf("Content-Type: image/jpeg\r\n\r\n");

    char buffer[1024];
    int n;
    while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, n);
    }

    close(fd);
}
    ROUTE_END()
}
