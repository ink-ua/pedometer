#ifndef HISTORYPROVIDER_H
#define HISTORYPROVIDER_H

#include <QObject>
#include <QtSql/QSqlQuery>
//#include <QDebug>

#include "appcontroller.h"
#include "historyentry.h"

class HistoryProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint64 totalSteps READ getTotalSteps NOTIFY totalStepsChanged)
    Q_PROPERTY(quint64 totalTime READ getTotalTime NOTIFY totalTimeChanged)
    Q_PROPERTY(double calTotal READ getCalTotal NOTIFY calTotalChanged)

public:
    HistoryProvider(QObject* parent =0) : QObject(parent), m_totalTime(0),
        m_totalSteps(0), m_currentDate(QDate::currentDate()), m_today(0) {}

    Q_INVOKABLE void loadHistory() {
        //qDebug() << "* they want history";
        m_totalTime = 0;
        m_totalSteps = 0;
        q = AppController::getInstance()->m_db.exec("SELECT SUM(seconds), SUM(steps), date FROM history GROUP BY date ORDER BY date DESC");
    }

    Q_INVOKABLE QObject* getNextEntry() {
        int t = q.value(0).toInt();
        int s = q.value(1).toInt();
        QString date = q.value(2).toString();

        addTotal(t, s);

        HistoryEntry* h = new HistoryEntry(t, s, date);
        if(h->getDate() == m_currentDate)
            m_today = h;
        return h;
    }

    Q_INVOKABLE bool nextEntry() {
        return q.next();
    }

    quint64 getTotalSteps() {
        return m_totalSteps;
    }
    quint64 getTotalTime() const {
        return m_totalTime;
    }
    double getCalTotal() {
        return m_totalSteps * AppController::getInstance()->getCalPerStep();
    }

    void addTotal(int t, int s) {
        m_totalTime += t;
        m_totalSteps += s;
        emit calTotalChanged();
        emit totalStepsChanged();
        emit totalTimeChanged();
    }

private:
    QSqlQuery q;
    quint64 m_totalTime;
    quint64 m_totalSteps;
    HistoryEntry* m_today;
    QDate m_currentDate;

signals:
    void calTotalChanged();
    void totalStepsChanged();
    void totalTimeChanged();
    void entryAdded(QObject* h);

public slots:
    void addEntry(int t, int s) {
        QSqlQuery q(AppController::getInstance()->m_db);
        q.prepare("INSERT INTO history(seconds, steps) VALUES(:seconds, :steps)");
        q.bindValue(":seconds", t);
        q.bindValue(":steps", s);
        q.exec();

        QDate newCurrent = QDate::currentDate();
        if(m_today && m_currentDate == newCurrent) {
            m_today->plusSteps(s);
            m_today->plusTime(t);
        }
        else {
            m_currentDate = newCurrent;
            m_today = new HistoryEntry(t, s, m_currentDate);
            emit entryAdded(m_today);
        }

        addTotal(t, s);
    }
};

#endif // HISTORYPROVIDER_H
