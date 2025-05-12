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
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool loginUser(const char* username, const char* password);
void registerUser(const char* username, const char* password);
void showUsers();

void route() {
    ROUTE_START()

    ROUTE_POST("/login")
    {
        char *user_start = strstr(payload, "sername=");
        char *pass_start = strstr(payload, "password=");

        if (user_start && pass_start) {
            char username[64] = {0}, password[64] = {0};
            sscanf(user_start, "sername=%63[^&]", username);
            sscanf(pass_start, "password=%63[^&]", password);

            if (loginUser(username, password)) {
                printf("\033[0;32m\nAccess granted\n\033[0m");
            } else {
                printf("\033[0;31m\nAccess denied\n\033[0m");
            }
        }
    }

    ROUTE_POST("/register")
    {
        char *user_start = strstr(payload, "sername=");
        char *pass_start = strstr(payload, "password=");

        if (user_start && pass_start) {
            char username[64] = {0}, password[64] = {0};
            sscanf(user_start, "sername=%63[^&]", username);
            sscanf(pass_start, "password=%63[^&]", password);

            registerUser(username, password);
            printf("User registered successfully.\n");
        }
    }

    ROUTE_GET("/show")
    {
        showUsers();
    }

    ROUTE_END()
}
