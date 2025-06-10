#include "signup.h"
#include "ui_signup.h"
#include "login.h"
#include "usermanager.h"
#include <QMessageBox>
#include <QString>
#include <QRegularExpression>

SignUp::SignUp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SignUp)
{
    ui->setupUi(this);
    ui->signupimage->setPixmap(QPixmap(":/signup/SignUp.png"));
    ui->signupimage->setScaledContents(true);
}


SignUp::~SignUp()
{
    delete ui;
}


void SignUp::on_btnRegister_clicked()
{
    QString username = ui->lineEditUser->text();
    QString password = ui->lineEditPass->text();
    QString email    = ui->lineEditEmail->text();
    QString phone    = ui->lineEditPhone->text();

    // Kiểm tra rỗng
    if (username.isEmpty() || password.isEmpty() || email.isEmpty() || phone.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    UserManager manager("users.txt"); // Đảm bảo có đường dẫn đúng

    // Kiểm tra điều kiện tên đăng nhập
    if (username.length() < 6) {
        QMessageBox::warning(this, "Invalid Username", "Username must be at least 6 characters.");
        return;
    }

    // Kiểm tra điều kiện mật khẩu
    QRegularExpression digit("[0-9]");
    QRegularExpression upper("[A-Z]");
    QRegularExpression special("[^a-zA-Z0-9]");

    if (password.length() < 6 ||
        !password.contains(digit) ||
        !password.contains(upper) ||
        !password.contains(special))
    {
        QMessageBox::warning(this, "Invalid Password",
                             "Password must be at least 6 characters, contain at least 1 digit, 1 uppercase letter, and 1 special character.");
        return;
    }

    // Đăng ký
    if (manager.isUserExist(username)) {
        QMessageBox::warning(this, "Error", "Username already exists.");
        return;
    }

    if (manager.registerUser(username, password, email, phone)) {
        QMessageBox::information(this, "Success", "Registration successful! Returning to login screen.");
        this->close();
        emit backToLogin();
    } else {
        QMessageBox::critical(this, "Error", "Failed to register user.");
    }
}

void SignUp::on_btnBackToLogin_clicked()
{
    emit backToLogin(); // Gửi tín hiệu về main
    this->close();      // Đóng giao diện đăng ký
}

