#ifndef HISTORYENTRY_H
#define HISTORYENTRY_H

#include <QObject>
#include <QVariant>
#include <QDate>

#include "formatter.h"
#include "appcontroller.h"

class HistoryEntry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString time READ getTime NOTIFY timeChanged)
    Q_PROPERTY(int seconds READ getSeconds NOTIFY timeChanged)
    Q_PROPERTY(int steps READ getSteps NOTIFY stepsChanged)
    Q_PROPERTY(double distance READ getDistance NOTIFY distanceChanged)
    Q_PROPERTY(double calories READ getCalories NOTIFY caloriesChanged)
    Q_PROPERTY(int day READ getDay NOTIFY dateChanged)
    Q_PROPERTY(QString month READ getMonth NOTIFY dateChanged)
    //Q_PROPERTY(int intMonth READ getIntMonth NOTIFY dateChanged)

public:
    enum HISTORY_ROLES { STEPS = Qt::UserRole + 1, TIME, DAY, MONTH, SECONDS, DISTANCE, CALORIES };

//    //HistoryEntry(QObject *parent=0) : QObject(parent) {}
//    HistoryEntry(int time, int steps, double distance, double calories, QString date, QObject *parent=0)
//        : QObject(parent), m_time(time), m_steps(steps), m_distance(distance), m_calories(calories),
//          m_date(QDate::fromString(date, "yyyy-MM-dd"))
//    {
//        init();
//    }
    HistoryEntry(int time, int steps, double distance, double calories, QDate date, QObject *parent=0)
        : QObject(parent), m_time(time), m_steps(steps), m_distance(distance), m_calories(calories), m_date(date)
    {
        HistoryEntry();
    }

    HistoryEntry(const HistoryEntry& rhs) {
        HistoryEntry(rhs.m_time, rhs.m_steps, rhs.m_distance, rhs.m_calories, rhs.m_date, rhs.parent());
    }

    HistoryEntry() {
        QObject::connect(AppController::getInstance(), SIGNAL(unitsChanged()), this, SLOT(onUnitsChanged()));
    }

    void populate(int time, int steps, double distance, double calories) {
        m_time = time;
        m_steps = steps;
        m_distance = distance;
        m_calories = calories;

        emit timeChanged();
        emit stepsChanged();
        emit distanceChanged();
        emit caloriesChanged();
    }

    QVariant data(int role) const {
        QVariant v;
        switch(role) {
        case STEPS:
            v = QVariant::fromValue(getSteps());
            break;
        case TIME:
            v = QVariant::fromValue(getTime());
            break;
        case DAY:
            v = QVariant::fromValue(getDay());
            break;
        case MONTH:
            v = QVariant::fromValue(getMonth());
            break;
        case SECONDS:
            v = QVariant::fromValue(getSeconds());
            break;
        case DISTANCE:
            v = QVariant::fromValue(getDistance());
            break;
        case CALORIES:
            v = QVariant::fromValue(getCalories());
            break;
        default:
            v = QVariant();
        }
        return v;
    }

//    // hash based on Date
//    int hash() {
//        return hash(m_date);
//    }
//    static int hash(QDate d) {
//        return d.dayOfYear() + ((int)(d.year() - 2011) * 365.25);
//    }

    QDate getDate() const {
        return m_date;
    }

    QString getTime() const {
        return Formatter::getInstance()->formatTime(m_time);
    }

    int getSeconds() const {
        return m_time;
    }

    void plusTime(int s) {
        m_time += s;
        emit timeChanged();
    }

    void plusSteps(int s) {
        m_steps += s;
        emit stepsChanged();
    }

    int getSteps() const {
        return m_steps;
    }

    double getDistance() const {
        return m_distance;
    }
    void plusDistance(double d) {
        m_distance += d;
        emit distanceChanged();
    }

    double getCalories() const {
        return m_calories;
    }
    void plusCalories(double c) {
        m_calories += c;
        emit caloriesChanged();
    }

    int getDay() const {
        return m_date.day();
    }

    QString getMonth() const {
        return m_date.toString("MMM yyyy");
    }

//    int getIntMonth() {
//        return m_date.month() + m_date.year() * 12;
//    }

public slots:
    void onUnitsChanged() {
        emit distanceChanged();
    }

signals:
    void timeChanged();
    void stepsChanged();
    void distanceChanged();
    void caloriesChanged();
    void dateChanged();

private:
    int m_time;
    int m_steps;
    double m_distance;
    double m_calories;
    QDate m_date;
};

Q_DECLARE_METATYPE(HistoryEntry)

#endif // HISTORYENTRY_H
