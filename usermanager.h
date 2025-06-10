#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QString>

class UserManager
{
public:
    UserManager(const QString &filePath);  // Constructor
    UserManager();

    bool registerUser(const QString &username, const QString &password, const QString &email, const QString &phone);
    bool loginUser(const QString &username, const QString &password);
    bool isUserExist(const QString &username);

private:
    QString userFilePath;
    bool isValidUsername(const QString &username);
    bool isValidPassword(const QString &password);
};

#endif // USERMANAGER_H

