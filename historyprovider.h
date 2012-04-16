#ifndef HISTORYPROVIDER_H
#define HISTORYPROVIDER_H

#include <QObject>
#include <QtSql/QSqlQuery>
//#include <QDebug>

#include "appcontroller.h"
#include "historyentry.h"
#include "dbutils.h"

class HistoryProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint64 totalSteps READ getTotalSteps NOTIFY totalStepsChanged)
    Q_PROPERTY(quint64 totalTime READ getTotalTime NOTIFY totalTimeChanged)
    Q_PROPERTY(double totalDistance READ getTotalDistance NOTIFY totalDistanceChanged)
    Q_PROPERTY(double totalCalories READ getTotalCalories NOTIFY totalCaloriesChanged)
    Q_PROPERTY(double avgSpeed READ getAvgSpeed NOTIFY avgSpeedChanged)
    Q_PROPERTY(double avgRate READ getAvgRate NOTIFY avgRateChanged)

public:
    HistoryProvider(QObject* parent =0) : QObject(parent), m_totalTime(0),
        m_totalSteps(0), m_totalDistance(0), m_totalCalories(0), m_today(0),
        m_countEntries(0), m_totalRate(0)
    {
        QObject::connect(AppController::getInstance(), SIGNAL(unitsChanged()), this, SLOT(onUnitsChanged()));
    }

    Q_INVOKABLE void loadHistory() {
        //qDebug() << "loadHistory()";
        m_totalTime = 0;
        m_totalSteps = 0;
        m_totalDistance = 0;
        m_totalCalories = 0;
        queryTotal = DBUtils::getInstance()->getTotalsByDay();
    }

    Q_INVOKABLE QObject* getNextEntry() {
        int t = queryTotal.value(0).toInt();
        int s = queryTotal.value(1).toInt();
        double d = queryTotal.value(2).toDouble();
        double c = queryTotal.value(3).toDouble();
        QString date = queryTotal.value(4).toString();

        addTotal(t, s, d, c);

        HistoryEntry* h = new HistoryEntry(t, s, d, c, date);
        if(h->getDate() == AppController::getInstance()->getCurrentDate()) {
            m_today = h;
        }
        m_countEntries++;
        return h;
    }

    Q_INVOKABLE bool nextEntry() {
        return queryTotal.next();
    }

    quint64 getTotalSteps() {
        return m_totalSteps;
    }
    quint64 getTotalTime() const {
        return m_totalTime;
    }
    double getTotalDistance() {
        return m_totalDistance;
    }
    double getTotalCalories() {
        return m_totalCalories;
    }
    double getAvgSpeed() {
        double result = 0;
        if(m_totalDistance > 0 && m_totalTime > 0) {
            result = AppController::getInstance()->calculateSpeed(m_totalDistance, m_totalTime);
        }
        return result;
    }
    double getAvgRate() {
        double result = 0;
        //qDebug() << m_totalRate << " " << m_countEntries;
        if(m_totalRate > 0 && m_countEntries > 0) {
            result = m_totalRate / m_countEntries;
        }
        return result;
    }

    void addTotal(int t, int s, double d, double c) {
        m_totalTime += t;
        m_totalSteps += s;
        m_totalDistance += d;
        m_totalCalories += c;
        m_totalRate += AppController::getInstance()->calculateRate(d);
        emit totalDistanceChanged();
        emit totalCaloriesChanged();
        emit totalStepsChanged();
        emit totalTimeChanged();
        emit avgSpeedChanged();
        emit avgRateChanged();
    }

private:
    QSqlQuery queryTotal;
    quint64 m_totalTime;
    quint64 m_totalSteps;
    double m_totalDistance;
    double m_totalCalories;
    double m_totalRate;
    int m_countEntries;
    HistoryEntry* m_today;

signals:
    void totalDistanceChanged();
    void totalCaloriesChanged();
    void totalStepsChanged();
    void totalTimeChanged();
    void avgSpeedChanged();
    void avgRateChanged();
//    void entryAddedToList(QObject* e);

public slots:
    void addEntry(int t, int s, double d, double c, QDate date) {
        if(m_today && m_today->getDate() == date) {
            m_today->plusSteps(s);
            m_today->plusTime(t);
            m_today->plusDistance(d);
            m_today->plusCalories(c);
        }
        else {
            m_today = new HistoryEntry(t, s, d, c, date);
            m_countEntries++;
//            emit entryAddedToList(m_today);
        }
        addTotal(t, s, d, c);
    }

    void onUnitsChanged() {
        emit totalDistanceChanged();
        emit avgSpeedChanged();
    }
};

#endif // HISTORYPROVIDER_H
