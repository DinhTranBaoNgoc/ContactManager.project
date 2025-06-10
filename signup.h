#ifndef SIGNUP_H
#define SIGNUP_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SignUp; }
QT_END_NAMESPACE

class SignUp : public QMainWindow
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);
    ~SignUp();
signals:
    void backToLogin();
private slots:
    void on_btnRegister_clicked();
    void on_btnBackToLogin_clicked();

private:
    Ui::SignUp *ui;
};
#endif // SIGNUP_H
