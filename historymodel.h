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
    enum HISTORY_ROLES { STEPS = Qt::UserRole + 1, TIME, DAY, MONTH, SECONDS };

    HistoryModel(QObject *parent =0) : QAbstractListModel(parent) {
        QHash<int, QByteArray> roles;
        roles[STEPS] = "steps";
        roles[TIME] = "time";
        roles[DAY] = "day";
        roles[MONTH] = "month";
        roles[SECONDS] = "seconds";
        setRoleNames(roles);
    }

    QList<QObject*>* getList() {
        return &historyList;
    }

    Q_INVOKABLE QObject* get(int index) {
        return historyList.at(index);
    }

    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const {
        Q_UNUSED(parent);
        //qDebug() << "count" << historyList.count();
        return historyList.count();
    }

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole ) const {
        QVariant v = QVariant();
        if (index.isValid() && index.row() < historyList.count() && index.row() > 0 /*&& role == Qt::DisplayRole*/) {
            HistoryEntry* h = (HistoryEntry*)historyList.at(index.row());
            qDebug() << "row" << index.row() << "data" << h->getSteps() << h->getTime() << h->getMonth();
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
            case SECONDS:
                v = QVariant::fromValue(h->getSeconds());
                break;
            }
        }        
        return v;
    }

    void push_front(QObject *item) {
        historyList.push_front(item);
    }

    void insertRow(int row, QObject *item) {
        QModelIndex parent = QModelIndex();
        connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
        beginInsertRows(parent, row, row);
        historyList.insert(row, item);
        endInsertRows();
    }

    QModelIndex indexFromItem(const QObject *item) const {
      Q_ASSERT(item);
      for(int row = 0; row < historyList.size(); row++)
        if(historyList.at(row) == item) {
            qDebug() << "row1" << index(row);
            return index(row);
        }
      return QModelIndex();
    }

public slots:
    void handleItemChange() {
      QObject* item = static_cast<QObject*>(sender());
      QModelIndex index = indexFromItem(item);
      qDebug() << "row" << index.row();
      if(index.isValid())
        emit dataChanged(index, index);
    }

private:
    QList<QObject*> historyList;
};

#endif // HISTORYMODEL_H
