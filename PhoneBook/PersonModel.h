#pragma once
#include <QAbstractTableModel>
#include <QVector>
#include <QList>
#include <QPair>
#include "Person.h"

class PersonModel : public QAbstractTableModel {
    Q_OBJECT
public:
    // Конструктор для сортировки по людям
    explicit PersonModel(const QVector<Person>& persons, QObject* parent = nullptr)
        : QAbstractTableModel(parent), m_persons(persons), m_usePhones(false) {}

    // Конструктор для сортировки по номерам
    explicit PersonModel(const QList<QPair<QString, QString>>& list, QObject* parent = nullptr)
        : QAbstractTableModel(parent), m_list(list), m_usePhones(true) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return m_usePhones ? m_list.size() : m_persons.size();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return m_usePhones ? 2 : 3; // Name + Phone или First + Last + Phones
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

        if (m_usePhones) {
            const auto &pair = m_list[index.row()];
            return index.column() == 0 ? pair.first : pair.second;
        } else {
            const Person &p = m_persons[index.row()];
            switch (index.column()) {
            case 0: return p.firstName;
            case 1: return p.lastName;
            case 2: {
                QStringList phones;
                for (const auto &ph : p.phoneNumbers)
                    phones << ph.number;
                return phones.join(", ");
            }
            }
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role != Qt::DisplayRole || orientation != Qt::Horizontal) return QVariant();

        if (m_usePhones) {
            return section == 0 ? "Name" : "Phone";
        } else {
            switch(section) {
            case 0: return "First Name";
            case 1: return "Last Name";
            case 2: return "Phone Numbers";
            }
        }
        return QVariant();
    }

private:
    QVector<Person> m_persons;
    QList<QPair<QString, QString>> m_list;
    bool m_usePhones; // true если используем список (имя, номер)
};
