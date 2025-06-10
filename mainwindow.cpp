#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), currentUser(username)
{
    ui->setupUi(this);
    ui->mainimage->setPixmap(QPixmap(":/signup/main.png"));
    ui->mainimage->setScaledContents(true);

    contactFile = "contacts_" + username + ".txt";

    // Cài đặt tiêu đề cột
    ui->tableWidget->setColumnCount(8);
    QStringList headers = {"", "Name", "Phone", "Email", "Age", "Gender", "Address", "Group"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Bắt buộc: hiển thị header ngang
    ui->tableWidget->horizontalHeader()->setVisible(true);

    // Co giãn đều các cột
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Hiển thị số thứ tự hàng (1, 2, 3...)
    ui->tableWidget->verticalHeader()->setVisible(true); // Đảm bảo hiển thị
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed); // Kích thước cố định
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(25); // Chiều cao dòng (nếu cần)
    ui->tableWidget->verticalHeader()->setMinimumWidth(30);       // Chiều rộng cột đánh số
    ui->tableWidget->setColumnWidth(0, 50);

    loadContacts();

    // Cài đặt comboBox tìm kiếm
    ui->comboSearchBy->addItems({"Name", "Phone", "Email", "Group", "Address"});
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadContacts()
{
    ui->tableWidget->setRowCount(0); // Clear table

    QFile file(contactFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(";");

        // Debug: kiểm tra giá trị của các trường
        qDebug() << "Fields: " << fields;

        if (fields.size() == 7) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            // Thêm checkbox vào cột đầu tiên
            QTableWidgetItem *checkItem = new QTableWidgetItem();
            checkItem->setCheckState(Qt::Unchecked); // Chưa chọn
            ui->tableWidget->setItem(row, 0, checkItem);

            // Thêm các cột còn lại, bao gồm cột Name
            for (int col = 1; col < 8; ++col) {
                ui->tableWidget->setItem(row, col, new QTableWidgetItem(fields[col - 1]));
            }
        }
    }

    file.close();

    // Đặt kích thước cột đầu tiên (chứa checkbox) nhỏ lại
    ui->tableWidget->setColumnWidth(0, 30);  // Cột đầu tiên có kích thước là 30 pixels
}

void MainWindow::saveContacts()
{
    QFile file(contactFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    int rowCount = ui->tableWidget->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        QStringList fields;
        // Bỏ qua cột 0 (checkbox), lấy từ cột 1 đến 7
        for (int col = 1; col < 8; ++col) {
            fields << ui->tableWidget->item(row, col)->text();
        }
        out << fields.join(";") << "\n";
    }

    file.close();
}



void MainWindow::on_btnAdd_clicked()
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    // Thêm checkbox vào cột đầu tiên
    QTableWidgetItem *checkItem = new QTableWidgetItem();
    checkItem->setCheckState(Qt::Unchecked); // Chưa chọn
    ui->tableWidget->setItem(row, 0, checkItem);

    // Thêm các cột còn lại, bao gồm cột Name
    for (int col = 1; col < 8; ++col) {
        QTableWidgetItem *item = new QTableWidgetItem("");
        ui->tableWidget->setItem(row, col, item);
    }

    ui->tableWidget->setCurrentCell(row, 0);
    ui->tableWidget->editItem(ui->tableWidget->item(row, 0));

    ui->btnSave->show(); // <-- hiện nút lưu
}

void MainWindow::on_btnDelete_clicked()
{
    QList<int> rowsToDelete;

    // Thu thập các dòng mà người dùng đã tích chọn
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem *checkItem = ui->tableWidget->item(row, 0);
        if (checkItem && checkItem->checkState() == Qt::Checked) {
            rowsToDelete.append(row);
        }
    }

    if (rowsToDelete.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select at least one contact to delete.");
        return;
    }

    // Xác nhận xóa
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete the selected contacts?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // Dù người dùng chọn gì, ta vẫn xóa từ dưới lên để không bị lệch dòng
        std::sort(rowsToDelete.begin(), rowsToDelete.end(), std::greater<int>());
        for (int row : rowsToDelete) {
            ui->tableWidget->removeRow(row);
        }

        ui->btnSave->show(); // Hiện nút lưu để người dùng lưu thay đổi nếu muốn
    }
}

void MainWindow::on_btnEdit_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a contact to edit.");
        return;
    }

    ui->tableWidget->editItem(ui->tableWidget->item(row, 0));
    ui->btnSave->show(); // <-- hiện nút lưu
}

void MainWindow::on_btnSearch_clicked()
{
    QString keyword = ui->lineSearch->text().trimmed();
    if (keyword.isEmpty()) {
        QMessageBox::warning(this, "Empty", "Please enter a keyword to search.");
        return;
    }

    QMap<int, int> searchColumnMap = {
        {0, 1},  // Name
        {1, 2},  // Phone
        {2, 3},  // Email
        {3, 7},  // Group
        {4, 6}   // Address
    };

    int comboIndex = ui->comboSearchBy->currentIndex();
    int searchColumn = searchColumnMap.value(comboIndex, -1);

    if (searchColumn == -1) {
        QMessageBox::warning(this, "Error", "Invalid search column.");
        return;
    }

    bool found = false;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem *item = ui->tableWidget->item(row, searchColumn);
        if (!item) {
            ui->tableWidget->setRowHidden(row, true);
            continue;
        }

        bool match = item->text().contains(keyword, Qt::CaseInsensitive);
        ui->tableWidget->setRowHidden(row, !match);
        if (match) {
            found = true;
        }
    }

    if (!found) {
        QMessageBox::information(this, "No Results", "No contacts found.");
    }
}

void MainWindow::on_btnDisplayAll_clicked()
{
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        ui->tableWidget->setRowHidden(row, false); // Hiện tất cả dòng
    }
}

void MainWindow::on_btnExit_clicked()
{
   emit logOut();  // Phát tín hiệu
}

void MainWindow::on_btnSave_clicked()
{
    saveContacts();
    ui->btnSave->hide();  // <-- ẩn sau khi lưu
    QMessageBox::information(this, "Saved", "Contacts saved successfully.");
}

