#include "Database.h"
#include <QDebug>

Database::Database() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../phonebook.db");
}

Database::~Database() {
    close();
}

bool Database::open() {
    if (!db.open()) {
        qDebug() << "Cannot open database:" << db.lastError().text();
        return false;
    }
    initTables();
    return true;
}

void Database::close() {
    if (db.isOpen()) db.close();
}

void Database::initTables() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS persons (id INTEGER PRIMARY KEY AUTOINCREMENT, firstName TEXT, lastName TEXT)");
    query.exec("CREATE TABLE IF NOT EXISTS phones (id INTEGER PRIMARY KEY AUTOINCREMENT, personId INTEGER, number TEXT, FOREIGN KEY(personId) REFERENCES persons(id))");
}

bool Database::addPerson(const Person& person) {
    QSqlQuery query;
    query.prepare("INSERT INTO persons (firstName, lastName) VALUES (?, ?)");
    query.addBindValue(person.firstName);
    query.addBindValue(person.lastName);

    if (!query.exec()) {
        qDebug() << "Failed to add person:" << query.lastError().text();
        return false;
    }

    int personId = query.lastInsertId().toInt();
    for (const auto& phone : person.phoneNumbers) {
        QSqlQuery phoneQuery;
        phoneQuery.prepare("INSERT INTO phones (personId, number) VALUES (?, ?)");
        phoneQuery.addBindValue(personId);
        phoneQuery.addBindValue(phone.number);
        if (!phoneQuery.exec()) {
            qDebug() << "Failed to add phone:" << phoneQuery.lastError().text();
        }
    }
    return true;
}

QVector<Person> Database::getAllPersons() {
    QVector<Person> result;
    QSqlQuery query("SELECT id, firstName, lastName FROM persons");
    while (query.next()) {
        Person p;
        p.id = query.value(0).toInt();
        p.firstName = query.value(1).toString();
        p.lastName = query.value(2).toString();

        QSqlQuery phoneQuery;
        phoneQuery.prepare("SELECT id, number FROM phones WHERE personId = ?");
        phoneQuery.addBindValue(p.id);
        if (phoneQuery.exec()) {
            while (phoneQuery.next()) {
                PhoneNumber ph;
                ph.id = phoneQuery.value(0).toInt();
                ph.number = phoneQuery.value(1).toString();
                p.phoneNumbers.append(ph);
            }
        }
        result.append(p);
    }
    return result;
}

QList<QPair<QString, QString>> Database::getAllByPhone() {
    QList<QPair<QString, QString>> result;
    QSqlQuery query("SELECT p.firstName || ' ' || p.lastName, ph.number FROM phones ph JOIN persons p ON ph.personId = p.id ORDER BY ph.number ASC");
    while (query.next()) {
        result.append({query.value(0).toString(), query.value(1).toString()});
    }
    return result;
}
