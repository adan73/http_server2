#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool loginUser(const char* username, const char* password) {
    FILE *file = fopen("password.txt", "r");
    if (!file) return false;

    char storedUser[64], storedPass[64];
    while (fscanf(file, "%s %s", storedUser, storedPass) != EOF) {
        if (strcmp(username, storedUser) == 0 && strcmp(password, storedPass) == 0) {
            fclose(file);
            return true;
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
        printf("\n1. Register\n2. Login\n3. Show Users\n4. Exit\nEnter choice: ");
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
                printf("Enter username: ");
                scanf("%s", user);
                printf("Enter password: ");
                scanf("%s", pass);
                if (loginUser(user, pass))
                    printf("✅ Access granted\n");
                else
                    printf("❌ Access denied\n");
                break;
            case 3:
                showUsers();
                break;
            case 4:
                return 0;
            default:
                printf("Invalid option\n");
        }
    }
}
