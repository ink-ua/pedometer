#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include "historyentry.h"

#include <QAbstractListModel>
#include <QList>

class HistoryModel : public QAbstractListModel {
    Q_OBJECT

public:
    HistoryModel(QObject *parent =0) : QAbstractListModel(parent) {
        QHash<int, QByteArray> roles;
        roles[HistoryEntry::STEPS] = "steps";
        roles[HistoryEntry::TIME] = "time";
        roles[HistoryEntry::DAY] = "day";
        roles[HistoryEntry::MONTH] = "month";
        roles[HistoryEntry::SECONDS] = "seconds";
        roles[HistoryEntry::DISTANCE] = "distance";
        roles[HistoryEntry::CALORIES] = "calories";
        setRoleNames(roles);
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const {
        return m_entries.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
        if (!index.isValid() || (index.row() >= m_entries.size() || index.row() < 0)) {
            return QVariant();
        }
        if (role == Qt::DisplayRole) {
            return QVariant::fromValue(*m_entries.at(index.row()));
        }
        return m_entries.at(index.row())->data(role);
    }

    void add(HistoryEntry* e) {
        m_entries.push_back(e);
    }

    void clear() {
        m_entries.clear();
    }

private:
    QList<HistoryEntry*> m_entries;
};

#endif // HISTORYMODEL_H
