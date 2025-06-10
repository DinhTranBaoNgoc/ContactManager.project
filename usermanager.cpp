#include "usermanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

UserManager::UserManager(const QString &filePath)
{
    userFilePath = filePath;
}

UserManager::UserManager()
{
    userFilePath = "users.txt";
}

bool UserManager::isUserExist(const QString &username)
{
    QFile file(userFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(";");
        if (parts.size() >= 1 && parts[0] == username) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

bool UserManager::registerUser(const QString &username, const QString &password, const QString &email, const QString &phone)
{
    if (!isValidUsername(username) || !isValidPassword(password)) {
        return false;
    }

    if (isUserExist(username)) {
        return false;
    }

    QFile file(userFilePath);
    if (!file.open(QIODevice::Append | QIODevice::Text)) return false;

    QTextStream out(&file);
    out << username << ";" << password << ";" << email << ";" << phone << "\n";
    file.close();
    return true;
}


bool UserManager::loginUser(const QString &username, const QString &password)
{
    QFile file(userFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(";");
        if (parts.size() >= 2 && parts[0] == username && parts[1] == password) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

bool UserManager::isValidUsername(const QString &username)
{
    return username.length() >= 6;
}

bool UserManager::isValidPassword(const QString &password)
{
    if (password.length() < 6)
        return false;

    bool hasDigit = false, hasUpper = false, hasSpecial = false;
    for (const QChar &ch : password) {
        if (ch.isDigit()) hasDigit = true;
        else if (ch.isUpper()) hasUpper = true;
        else if (!ch.isLetterOrNumber()) hasSpecial = true;
    }

    return hasDigit && hasUpper && hasSpecial;
}
