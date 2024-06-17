#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userFunc.h"

USER* users = NULL;
int userCount = 0;
int nextId = 1;

void loadUsers() {
    FILE* file = fopen("users.txt", "r");
    if (file) {
        fscanf(file, "%d\n%d\n", &userCount, &nextId);
        users = (USER*)malloc(userCount * sizeof(USER));
        if (!users) {
            perror("Neuspjela alokacija memorije");
            fclose(file);
            return;
        }
        rewind(file);

        for (int i = 0; i < userCount; i++) {
            fscanf(file, "%d %s %s %s %f\n", &users[i].id, users[i].name, users[i].lastName, users[i].PIN, &users[i].balance);
        }
        fclose(file);
    }
    else {
        users = NULL;
        userCount = 0;
        nextId = 1;
    }
}

void saveUsers() {
    FILE* file = fopen("users.txt", "w");
    if (!file) {
        perror("Nismo uspjeli otvoriti users.txt");
        return;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        perror("Failed to seek to start of file");
        fclose(file);
        return;
    }

    fprintf(file, "%d\n%d\n", userCount, nextId);
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%d %s %s %s %f\n", users[i].id, users[i].name, users[i].lastName, users[i].PIN, users[i].balance);
    }
    fclose(file);
}

void addUser(USER* user) {
    users = (USER*)realloc(users, (userCount + 1) * sizeof(USER));
    if (!users) {
        perror("Neuspjela alokacija memorije");
        return;
    }
    users[userCount++] = *user;
    saveUsers();
}

void deleteUserById(int id) {
    for (int i = 0; i < userCount; i++) {
        if (users[i].id == id) {
            for (int j = i; j < userCount - 1; j++) {
                users[j] = users[j + 1];
            }
            userCount--;
            users = (USER*)realloc(users, userCount * sizeof(USER));
            saveUsers();
            printf("Korisnik uspjesno obrisan.\n");
            return;
        }
    }
    printf("Korisnik nije pronadjen.\n");
}

USER* authenticateUser() {
    char pin[9];
    printf("Unesi svoj PIN: ");
    scanf("%s", pin);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].PIN, pin) == 0) {
            return &users[i];
        }
    }

    printf("Korisnik nije pronadjen.\n");
    return NULL;
}

/////////////////////////////////////////////////////////

void depositMoney(USER* user) {
    float amount;
    printf("Unesi iznos za uplatu: ");
    scanf("%f", &amount);

    if (amount > 0) {
        user->balance += amount;
        printf("Uplata uspjesna. Novi iznos: %.2f\n", user->balance);
        saveUsers();
    }
    else {
        printf("Netocan iznos.\n");
    }
}

void withdrawMoney(USER* user) {
    float amount;
    printf("Unesi iznos za podici: ");
    scanf("%f", &amount);

    if (amount > 0 && amount <= user->balance) {
        user->balance -= amount;
        printf("Podizanje uspjesno. Novi iznos: %.2f\n", user->balance);
        saveUsers();
    }
    else {
        printf("Netocan iznos ili nedovoljno sredstava.\n");
    }
}

void showUserInfo(USER* user) {
    printf("ID: %d, Ime: %s %s, PIN: %s, Iznos: %.2f\n",
        getUserId(user), getUserName(user), getUserLastName(user), getUserPIN(user), getUserBalance(user));
}

int transferMoney(USER* user) {
    int recipientId;
    float amount;
    USER* recipient = NULL;

    printf("Unesi ID primatelja: ");
    scanf("%d", &recipientId);

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == recipientId) {
            recipient = &users[i];
            break;
        }
    }

    if (!recipient) {
        printf("Korisnik nije pronadjen.\n");
        return -1;
    }

    printf("Unesi iznos za poslati: ");
    scanf("%f", &amount);

    if (amount <= 0 || amount > user->balance) {
        printf("Netocan iznos ili nedovoljno sredstava.\n");
        return -1;
    }

    user->balance -= amount;
    recipient->balance += amount;
    printf("Prijenos uspjesan. Vasa novi iznos: %.2f\n", user->balance);
    saveUsers();
    return recipientId;
}

/// trazenje korisnika po PIN

int compareUserPINs(const void* a, const void* b) {
    return strcmp(((USER*)a)->PIN, ((USER*)b)->PIN);
}


USER* findUserByPIN(const char* PIN) {
    USER key;
    strcpy(key.PIN, PIN);
    return (USER*)bsearch(&key, users, userCount, sizeof(USER), compareUserPINs);
}