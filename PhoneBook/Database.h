#pragma once
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVector>
#include "Person.h"


class Database {
public:
    Database();
    ~Database();

    bool open();
    void close();

    bool addPerson(const Person& person);
    QVector<Person> getAllPersons(); // для сортировки по человеку
    QList<QPair<QString, QString>> getAllByPhone();     // для сортировки по номерам (имя, номер)

private:
    QSqlDatabase db;
    void initTables();
};
