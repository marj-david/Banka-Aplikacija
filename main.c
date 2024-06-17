#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "userFunc.h"
#include "adminFunc.h"

typedef enum {
    MAIN_LOGIN_ADMIN = 1,
    MAIN_LOGIN_USER,
    MAIN_REGISTER_USER,
    MAIN_EXIT
} MainMenuOptions;

void userMenu(USER* user);

int main() {
    MainMenuOptions choice;
    loadUsers();

    while (1) {
        printf("\t\t\t\t\t --Banka KOKUZ-- \n");
        printf("1. Prijava kao admin\n");
        printf("2. Prijava kao korisnik\n");
        printf("3. Registriraj se kao novi korisnik\n");
        printf("4. Izlaz\n");
        printf("Unesi svoj izbor: ");
        scanf("%d", (int*)&choice);

        switch (choice) {
        case MAIN_LOGIN_ADMIN:
            if (authenticateAdmin()) {
                adminMenu();
            }
            break;
        case MAIN_LOGIN_USER: {
            USER* user = authenticateUser();
            if (user != NULL) {
                userMenu(user);
            }
            break;
        }
        case MAIN_REGISTER_USER:
            registerUser();
            break;
        case MAIN_EXIT:
            saveUsers();
            free(users);
            return 0;
        default:
            printf("Krivi odabir, pokusaj ponovo.\n");
        }
    }
}

void userMenu(USER* user) {
    typedef enum {
        USER_DEPOSIT = 1,
        USER_WITHDRAW,
        USER_SHOW_INFO,
        USER_TRANSFER_MONEY,
        USER_LOGOUT
    } UserMenuOptions;

    UserMenuOptions choice;
    int recipientId;

    while (1) {
        printf("\nKorisnicki izbornik\n");
        printf("1. Uplati novac\n");
        printf("2. Podigni\n");
        printf("3. Pogledaj informacije o racunu\n");
        printf("4. Posalji novac\n");
        printf("5. Log out\n");
        printf("Unesi svoj izbor: ");
        scanf("%d", (int*)&choice);

        switch (choice) {
        case USER_DEPOSIT:
            depositMoney(user);
            break;
        case USER_WITHDRAW:
            withdrawMoney(user);
            break;
        case USER_SHOW_INFO:
            showUserInfo(user);
            break;
        case USER_TRANSFER_MONEY:
            recipientId = transferMoney(user);
            break;
        case USER_LOGOUT:
            return;
        default:
            printf("Krivi odabir, pokusajte ponovo.\n");
        }
    }
}
