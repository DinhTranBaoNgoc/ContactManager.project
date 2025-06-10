#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &username, QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void logOut();  // Thêm dòng này
private slots:
    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

    void on_btnEdit_clicked();

    void on_btnSearch_clicked();

    void on_btnDisplayAll_clicked();

    void on_btnExit_clicked();

    void on_btnSave_clicked();

private:
    Ui::MainWindow *ui;
    QString currentUser;
    QString contactFile;

    void loadContacts();
    void saveContacts();

};

#endif // MAINWINDOW_H
