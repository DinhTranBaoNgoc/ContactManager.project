#include "signup.h"
#include "login.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogIn login;
    MainWindow *mainWin = nullptr;

    QObject::connect(&login, &LogIn::loginSuccess, [&](const QString &username) {
        mainWin = new MainWindow(username);
        mainWin->show();
        login.hide();  // << Ẩn login khi vào main

        QObject::connect(mainWin, &MainWindow::logOut, [&]() {
            mainWin->close();

            login.clearFields();   // Xóa nội dung đăng nhập
            login.show();          // Quay về login
        });
    });

    login.show();
    return a.exec();
}
