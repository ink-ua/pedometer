#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include "historyentry.h"

#include <QAbstractListModel>
#include <QList>

class HistoryModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum HISTORY_ROLES { STEPS = Qt::UserRole + 1, TIME, DAY, MONTH, SECONDS, DISTANCE, CALORIES };

    HistoryModel(QObject *parent =0) : QAbstractListModel(parent) {
        QHash<int, QByteArray> roles;
        roles[STEPS] = "steps";
        roles[TIME] = "time";
        roles[DAY] = "day";
        roles[MONTH] = "month";
        roles[SECONDS] = "seconds";
        roles[DISTANCE] = "distance";
        roles[CALORIES] = "calories";
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
            //return m_entries.at(index.row());
            return QVariant::fromValue(*m_entries.at(index.row()));
//            return *((QVariant*)m_entries.at(index.row()));
        }
        return m_entries.at(index.row())->data(role);
    }

    void add(HistoryEntry* e) {
        m_entries.push_back(e);
    }

private:
    QList<HistoryEntry*> m_entries;
};

#endif // HISTORYMODEL_H
