#pragma once
#include <QString>
#include <QVector>
#include "PhoneNumber.h"

struct Person {
    int id;                  // уникальный идентификатор
    QString firstName;
    QString lastName;
    QVector<PhoneNumber> phoneNumbers;
};
