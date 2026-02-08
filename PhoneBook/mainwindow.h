#pragma once
#include <QMainWindow>
#include "Database.h"
#include "PersonModel.h"

    QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();
    void on_sortByPerson_clicked();
    void on_sortByPhone_clicked();

private:
    Ui::MainWindow *ui;
    Database db;
    PersonModel* model;
};
