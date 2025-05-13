#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

bool loginUser(const char* username, const char* password) {
    FILE *file = fopen("password.txt", "r");
    if (!file) return false;

    char storedUser[64], storedPass[64];
    while (fscanf(file, "%s %s", storedUser, storedPass) != EOF) {
        if (strcmp(username, storedUser) == 0) {
            for (int i = 0; storedPass[i] && password[i]; i++) {
                if (storedPass[i] != password[i]) {
                    fclose(file);
                    return false;
                }
                sleep(1);  // ⏱️ Delay for timing attack
            }

            if (strlen(storedPass) == strlen(password)) {
                fclose(file);
                return true;
            }
        }
    }

    fclose(file);
    return false;
}

void registerUser(const char* username, const char* password) {
    FILE *file = fopen("password.txt", "a");
    if (!file) return;

    fprintf(file, "%s %s\n", username, password);
    fclose(file);
}

void showUsers() {
    FILE *file = fopen("password.txt", "r");
    if (!file) return;

    char username[64], password[64];
    printf("USERNAME | PASSWORD\n");
    while (fscanf(file, "%s %s", username, password) != EOF) {
        printf("%s | %s\n", username, password);
    }
    fclose(file);
}

int main() {
    int choice;
    char user[64], pass[64];

    while (1) {
        printf("\n1. Register");
printf("\n2. Login (variable overflow test)");
printf("\n3. Login (timing attack test)");
printf("\n4. Show Users");
printf("\n5. Exit\nEnter choice: ");

        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1:
                printf("Enter username: ");
                scanf("%s", user);
                printf("Enter password: ");
                scanf("%s", pass);
                registerUser(user, pass);
                break;
                case 2:
{
    struct __attribute__((packed)) {
        char password[8];
        int access;
    } loginStruct = { .access = 0 };

    char username[64];

    printf("Enter username: ");
    scanf("%s", username);
    getchar();  
    printf("Enter password: ");
    gets(loginStruct.password); 

    FILE *file = fopen("password.txt", "r");
    if (file != NULL) {
        char storedUser[64], storedPass[64];
        while (fscanf(file, "%s %s", storedUser, storedPass) != EOF) {
            if (strcmp(username, storedUser) == 0 &&
                strcmp(loginStruct.password, storedPass) == 0) {
                loginStruct.access = 1;
                break;
            }
        }
        fclose(file);
    }

    if (loginStruct.access) {
        printf(" Access granted (overflowed or correct)\n");
    } else {
        printf(" Access denied\n");
    }
}
break;
       case 3:{
   
break;




            case 4:
                showUsers();
                break;

            case 5:
                return 0;

            default:
                printf("Invalid option\n");
        }
    }
}
