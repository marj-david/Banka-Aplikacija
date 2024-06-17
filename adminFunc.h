#ifndef ADMINFUNC_H
#define ADMINFUNC_H

typedef enum {
    ADMIN_PRINT_ALL_USERS = 1,
    ADMIN_PRINT_ALL_USERS_BY_ID,
    ADMIN_REGISTER_USER,
    ADMIN_DELETE_USER,
    ADMIN_FIND_USER,
    ADMIN_DELETE_ALL,
    ADMIN_EXIT
} AdminMenuOptions;

void adminMenu();
int authenticateAdmin();
void printAllUsers();
int compareUserLastNames(const void* a, const void* b);
void quicksortUsersByLastName(USER* users, int low, int high);
int partition(USER* users, int low, int high);
void registerUser();
void deleteUser();
void findUser();
int isValidPIN(const char* pin);
void deleteUserFile();
void findUserByPINAdmin();
int compareUserIDs(const void* a, const void* b);
void sortAndPrintUsersByLastName();
void sortAndPrintUsersByID();

#define SORT_BY_ID 1
#define SORT_BY_LASTNAME 2

#endif

