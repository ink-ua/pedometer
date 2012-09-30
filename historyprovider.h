#ifndef HISTORYPROVIDER_H
#define HISTORYPROVIDER_H

#include <QObject>
#include <QtSql/QSqlQuery>
#include <QDebug>

#include "appcontroller.h"
#include "historymodel.h"
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
    Q_PROPERTY(double avgSteps READ getAvgSteps NOTIFY avgStepsChanged)
    Q_PROPERTY(double avgTime READ getAvgTime NOTIFY avgTimeChanged)
    Q_PROPERTY(double avgCalories READ getAvgCalories NOTIFY avgCaloriesChanged)
    Q_PROPERTY(double avgDistance READ getAvgDistance NOTIFY avgDistanceChanged)
    Q_PROPERTY(double avgRate READ getAvgRate NOTIFY avgRateChanged)

public:
    HistoryProvider(HistoryModel& historyModel, QObject* parent =0) : QObject(parent), m_totalTime(0),
        m_totalSteps(0), m_totalDistance(0), m_totalCalories(0),
        m_countDays(0), m_totalRate(0), m_historyModel(historyModel)
    {
        QObject::connect(AppController::getInstance(), SIGNAL(unitsChanged()), this, SLOT(onUnitsChanged()));
        m_today = new HistoryEntry(0, 0, 0, 0, AppController::getInstance()->getCurrentDate(), this);
    }

    // returns true if history reloaded
    // false in case when only today changed since last load
    Q_INVOKABLE void reloadHistory() {
        QDate current = AppController::getInstance()->getCurrentDate();
        if(m_reloadDate < current) {
            m_totalTime = 0;
            m_totalSteps = 0;
            m_totalDistance = 0;
            m_totalCalories = 0;
            m_countDays = 0;
            m_reloadDate = current;
            queryTotal = DBUtils::getInstance()->getTotalsByDayAsc();

            while(hasNextEntry()) {
                m_historyModel.add(getNextEntry());
            }
        }
    }

    Q_INVOKABLE HistoryEntry* getNextEntry() {
        int t = queryTotal.value(0).toInt();
        int s = queryTotal.value(1).toInt();
        double d = queryTotal.value(2).toDouble();
        double c = queryTotal.value(3).toDouble();
        QString dateStr = queryTotal.value(4).toString();
        QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");

        HistoryEntry* result;
        // set today data
        if(date == m_reloadDate) {
            m_today->populate(t, s, d, c);
            result = m_today;
        } else {
            result = new HistoryEntry(t, s, d, c, date, this);
        }

        // modify totals and averages
        m_countDays++;
        addTotal(t, s, d, c);

        return result;
    }

    Q_INVOKABLE bool hasNextEntry() {
        return queryTotal.next();
    }

    Q_INVOKABLE QObject* getTodayEntry() {
        return m_today;
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

    // Averages
    int getAvgSteps() {
        return calcAvg(m_totalSteps);
    }
    int getAvgTime() {
        return calcAvg(m_totalTime);
    }
    double getAvgDistance() {
        return calcAvg(m_totalDistance);
    }
    double getAvgCalories() {
        return calcAvg(m_totalCalories);
    }
    double getAvgRate() {
        return calcAvg(m_totalRate);
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
        emit avgStepsChanged();
        emit avgTimeChanged();
        emit avgCaloriesChanged();
        emit avgDistanceChanged();
    }

private:
    QSqlQuery queryTotal;
    quint64 m_totalTime;
    quint64 m_totalSteps;
    double m_totalDistance;
    double m_totalCalories;
    double m_totalRate;
    int m_countDays;
    HistoryModel& m_historyModel;
    HistoryEntry* m_today;
    QDate m_reloadDate;

    template <typename T>
    T calcAvg(T total) {
        T result = 0;
        if(total > 0 && m_countDays > 0) {
            result = total / m_countDays;
        }
        return result;
    }

signals:
    void totalDistanceChanged();
    void totalCaloriesChanged();
    void totalStepsChanged();
    void totalTimeChanged();
    void avgSpeedChanged();
    void avgRateChanged();
    void avgStepsChanged();
    void avgTimeChanged();
    void avgCaloriesChanged();
    void avgDistanceChanged();

public slots:
    void addEntry(int t, int s, double d, double c, QDate date) {
        if(m_today->getDate() == date) {
            m_today->plusSteps(s);
            m_today->plusTime(t);
            m_today->plusDistance(d);
            m_today->plusCalories(c);
            addTotal(t, s, d, c);
        }
        else {
            // assume that on the next request all entries will be reloaded
//            delete m_today;
            m_today = new HistoryEntry(t, s, d, c, date, this);
        }
    }

    void onUnitsChanged() {
        emit totalDistanceChanged();
        emit avgSpeedChanged();
    }
};

#endif // HISTORYPROVIDER_H
