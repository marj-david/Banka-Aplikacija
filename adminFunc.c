#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "userFunc.h"
#include "adminFunc.h"
#define USERNAMELENGTH 50
#define PINLENGTH 9

int authenticateAdmin() {
    char username[USERNAMELENGTH], password[USERNAMELENGTH];
    printf("Unesi korisnicko ime admina: ");
    scanf("%s", username);
    printf("Unesi lozinku admina: ");
    scanf("%s", password);

    if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
        return 1;
    }
    else {
        printf("Netocni podatci o adminu.\n");
        return 0;
    }
}

void adminMenu() {
    AdminMenuOptions choice;

    while (1) {
        printf("\nAdmin Izbornik\n");
        printf("1. Ispisi sve korisnike  (abecedno po prezimenima)\n");
        printf("2. Ispisi sve korisnike (sortirano po ID)\n");
        printf("3. Registriraj novog korisnika\n");
        printf("4. Izbrisi postojeceg korisnika\n");
        printf("5. Pronadji postojeceg korisnika po PIN-u\n");
        printf("6. Izbrisi sve korisnike\n");
        printf("7. Izlaz u glavni izbornik\n");
        printf("Unesi svoj odabir: ");
        scanf("%d", (int*)&choice);

        switch (choice) {
        case ADMIN_PRINT_ALL_USERS:
            sortAndPrintUsersByLastName();
            break;
        case ADMIN_PRINT_ALL_USERS_BY_ID:
            sortAndPrintUsersByID();
            break;
        case ADMIN_REGISTER_USER:
            registerUser();
            break;
        case ADMIN_DELETE_USER:
            deleteUser();
            break;
        case ADMIN_FIND_USER:
            findUserByPINAdmin();
            break;
        case ADMIN_DELETE_ALL:
            deleteUserFile();
            break;
        case ADMIN_EXIT:
            return;
        default:
            printf("Krivi odabir, pokusajte ponovo.\n");
        }
    }
}

int isValidPIN(const char* pin) {
    if (strlen(pin) != 8) {
        return 0;
    }
    for (int i = 0; i < 8; i++) {
        if (!isdigit(pin[i])) {
            return 0;
        }
    }
    return 1;
}

void registerUser() {
    USER newUser;
    int pinExists = 0;

    newUser.id = nextId++;
    printf("Unesi ime: ");
    scanf("%s", newUser.name);
    printf("Unesi prezime: ");
    scanf("%s", newUser.lastName);

    do {
        pinExists = 0;
        printf("Unesi PIN od 8 brojeva: ");
        scanf("%s", newUser.PIN);

        if (!isValidPIN(newUser.PIN)) {
            printf("Greska. PIN mora imati tocno 8 brojeva.\n");
            continue;
        }

        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].PIN, newUser.PIN) == 0) {
                printf("Greska. PIN vec postoji, pokusaj ponovo.\n");
                pinExists = 1;
                break;
            }
        }
    } while (pinExists || !isValidPIN(newUser.PIN));

    newUser.balance = 0.0;
    addUser(&newUser);
}

void deleteUser() {
    int id;
    printf("Unesi ID korisnika za brisanje: ");
    scanf("%d", &id);
    deleteUserById(id);
}

void findUserByPINAdmin() {
    char PIN[PINLENGTH];
    printf("Unesite PIN koji zelite pronaci: ");
    scanf("%s", PIN);
    USER* user = findUserByPIN(PIN);
    if (user) {
        printf("ID: %d, Ime: %s %s, PIN: %s, Saldo: %.2f\n", user->id, user->name, user->lastName, user->PIN, user->balance);
    }
    else {
        printf("Nema pronadjenog korisnika s PIN-om %s.\n", PIN);
    }
}



/// ispisi imena abecedno

void sortAndPrintUsersByLastName() {
    quicksortUsersByLastName(users, 0, userCount - 1);
    printAllUsers();
}

int compareUserLastNames(const void* a, const void* b) {
    return strcmp(((USER*)a)->lastName, ((USER*)b)->lastName);
}

// rekurzivna
void quicksortUsersByLastName(USER* users, int low, int high) {
    if (low < high) {
        int pi = partition(users, low, high);

        quicksortUsersByLastName(users, low, pi - 1);
        quicksortUsersByLastName(users, pi + 1, high);
    }
}


int partition(USER* users, int low, int high) {
    USER pivot = users[high];
    int i = (low - 1); 

    for (int j = low; j <= high - 1; j++) {
        
        if (compareUserLastNames(&users[j], &pivot) < 0) {
            i++; 
            USER temp = users[i];
            users[i] = users[j];
            users[j] = temp;
        }
    }
    USER temp = users[i + 1];
    users[i + 1] = users[high];
    users[high] = temp;
    return (i + 1);
}

//ispis imena po ID


void sortAndPrintUsersByID() {
    qsort(users, userCount, sizeof(USER), compareUserIDs);
    printAllUsers();
}

int compareUserIDs(const void* a, const void* b) {
    USER* userA = (USER*)a;
    USER* userB = (USER*)b;
    return userA->id - userB->id;
}

void printAllUsers() {
    for (int i = 0; i < userCount; i++) {
        printf("ID: %d, Ime: %s %s, PIN: %s, Saldo: %.2f\n", users[i].id, users[i].name, users[i].lastName, users[i].PIN, users[i].balance);
    }
}

void deleteUserFile() {
    const char* fileName = "users.txt";

    if (remove(fileName) != 0) {
        perror("Neusjpelo brisanje datoteke");
    }
    else {
        printf("Korisnicka datoteka uspjesno obrisana.\n");
    }
}