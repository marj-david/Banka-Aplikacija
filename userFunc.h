#ifndef USERFUNC_H
#define USERFUNC_H

#define USERNAMELENGTH 50
#define PINLENGTH 9

typedef struct {
    int id;
    char name[USERNAMELENGTH];
    char lastName[USERNAMELENGTH];
    char PIN[PINLENGTH];
    float balance;
} USER;

extern USER* users;
extern int userCount;
extern int nextId;

void loadUsers();
void saveUsers();
void addUser(USER* user);
void deleteUserById(int id);
USER* authenticateUser();
void depositMoney(USER* user);
void withdrawMoney(USER* user);
void showUserInfo(USER* user);
int transferMoney(USER* user);

int compareUserPINs(const void* a, const void* b);
USER* findUserByPIN(const char* PIN);

inline int getUserId(USER* user) { return user->id; }
inline const char* getUserName(USER* user) { return user->name; }
inline const char* getUserLastName(USER* user) { return user->lastName; }
inline const char* getUserPIN(USER* user) { return user->PIN; }
inline float getUserBalance(USER* user) { return user->balance; }

#endif // USER_H
