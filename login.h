#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class LogIn;
}

class LogIn : public QWidget
{
    Q_OBJECT

public:
    explicit LogIn(QWidget *parent = nullptr);
     void clearFields();
    ~LogIn();
signals:
    void loginSuccess(const QString &username);
private slots:
    void on_btnLogin_clicked();

    void on_btnSignUp_clicked();

private:
    Ui::LogIn *ui;
};

#endif // LOGIN_H
