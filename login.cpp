#include "login.h"
#include "ui_login.h"
#include "signup.h"
#include "usermanager.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QRegularExpression>

LogIn::LogIn(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogIn)
{
    ui->setupUi(this);
    ui->loginimage->setPixmap(QPixmap(":/signup/SignUp.png"));
    ui->loginimage->setScaledContents(true);
}

LogIn::~LogIn()
{
    delete ui;
}

void LogIn::on_btnLogin_clicked()
{
    QString username = ui->lineEditUser->text();
    QString password = ui->lineEditPass->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both username and password.");
        return;
    }

    UserManager userManager("users.txt");
    if (userManager.loginUser(username, password)) {
        emit loginSuccess(username);  // <-- Phát tín hiệu
        this->close();                // <-- Đóng login window
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password!");
    }
}

void LogIn::on_btnSignUp_clicked()
{
    SignUp *signup = new SignUp();

    // Kết nối tín hiệu quay lại đăng nhập
    connect(signup, &SignUp::backToLogin, this, &LogIn::show);

    signup->show();
    this->close();
}

void LogIn::clearFields() {
    // Ví dụ: xóa nội dung các QLineEdit
    ui->lineEditUser->clear();
    ui->lineEditPass->clear();
}

