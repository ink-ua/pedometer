#ifndef HISTORYPROVIDER_H
#define HISTORYPROVIDER_H

#include <QObject>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include "historyentry.h"
#include "appcontroller.h"

extern AppController* appController;

class HistoryProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int totalSteps READ getTotalSteps NOTIFY totalStepsChanged)
    Q_PROPERTY(int totalTime READ getTotalTime NOTIFY totalTimeChanged)
    Q_PROPERTY(double calTotal READ getCalTotal NOTIFY calTotalChanged)

public:
    HistoryProvider(QObject* parent =0) : QObject(parent), m_totalTime(0), m_totalSteps(0) {}

    Q_INVOKABLE void loadHistory() {
        qDebug() << "* they want history";
        m_totalTime = 0;
        m_totalSteps = 0;
        q = appController->m_db.exec("SELECT SUM(seconds), SUM(steps), date FROM history GROUP BY date ORDER BY date DESC");
    }

    Q_INVOKABLE QObject* getNextEntry() {
        int t = q.value(0).toInt();
        int s = q.value(1).toInt();
        QString date = q.value(2).toString();

        addEntry(s, t);

        HistoryEntry* h = new HistoryEntry(t, s, date);
        return h;
    }

    Q_INVOKABLE bool nextEntry() {
        return q.next();
    }

    int getTotalSteps() {
        return m_totalSteps;
    }
    int getTotalTime() const {
        return m_totalTime;
    }
    double getCalTotal() {
        return m_totalSteps * appController->getCalPerStep();
    }

private:
    QSqlQuery q;
    int m_totalTime;
    int m_totalSteps;

signals:
    void calTotalChanged();
    void totalStepsChanged();
    void totalTimeChanged();

public slots:
    void addEntry(int s, int t) {
        m_totalTime += t;
        m_totalSteps += s;
        emit calTotalChanged();
        emit totalStepsChanged();
        emit totalTimeChanged();
    }
};

#endif // HISTORYPROVIDER_H
