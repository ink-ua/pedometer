#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include "historyentry.h"

class HistoryModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)

public:
    enum HISTORY_ROLES { STEPS = Qt::UserRole + 1, TIME, DAY, MONTH };

    HistoryModel(QObject *parent =0) : QAbstractListModel(parent) {
        QHash<int, QByteArray> roles;
        roles[STEPS] = "steps";
        roles[TIME] = "time";
        roles[DAY] = "day";
        roles[MONTH] = "month";
        setRoleNames(roles);
    }

    Q_INVOKABLE QObject* get(int index) {
        return historyList.at(index);
    }

    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const {
        return historyList.count();
    }

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole ) const {
        QVariant v = QVariant();
        if (index.isValid() && index.row() < historyList.count() /*&& role == Qt::DisplayRole*/) {
            HistoryEntry* h = (HistoryEntry*)historyList.at(index.row());
            switch(role) {
            case STEPS:
                v = QVariant::fromValue(h->getSteps());
                break;
            case TIME:
                v = QVariant::fromValue(h->getTime());
                break;
            case DAY:
                v = QVariant::fromValue(h->getDay());
                break;
            case MONTH:
                v = QVariant::fromValue(h->getMonth());
                break;
            }
        }
        return v;
    }

    void insert(QObject* h) {
        historyList.push_front(h);
        insertRows(0, 1, index(0));
    }

private:
    QList<QObject*> historyList;
};

#endif // HISTORYMODEL_H
