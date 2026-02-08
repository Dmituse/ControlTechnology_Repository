#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(nullptr) {
    ui->setupUi(this);
    db.open();
}

MainWindow::~MainWindow() {
    delete ui;
    delete model;
}

void MainWindow::on_addButton_clicked() {
    bool ok;
    QString firstName = QInputDialog::getText(this, "First Name", "Enter first name:", QLineEdit::Normal, "", &ok);
    if (!ok || firstName.isEmpty()) return;
    QString lastName = QInputDialog::getText(this, "Last Name", "Enter last name:", QLineEdit::Normal, "", &ok);
    if (!ok || lastName.isEmpty()) return;

    QVector<PhoneNumber> phones;
    while (true) {
        QString number = QInputDialog::getText(this, "Phone Number", "Enter phone number (cancel to stop):", QLineEdit::Normal, "", &ok);
        if (!ok || number.isEmpty()) break;
        phones.append({0, number});
    }

    Person p{0, firstName, lastName, phones};
    db.addPerson(p);
}

void MainWindow::on_sortByPerson_clicked() {
    auto persons = db.getAllPersons();
    delete model;
    model = new PersonModel(persons, this);
    ui->tableView->setModel(model);
}

void MainWindow::on_sortByPhone_clicked() {
    auto list = db.getAllByPhone();
    delete model;
    model = new PersonModel(list, this);
    ui->tableView->setModel(model);
}
