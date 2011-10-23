#ifndef HISTORYENTRY_H
#define HISTORYENTRY_H

#include <QObject>
#include <QDate>

class HistoryEntry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString time READ getTime NOTIFY timeChanged)
    Q_PROPERTY(int steps READ getSteps NOTIFY stepsChanged)
    Q_PROPERTY(int day READ getDay NOTIFY dateChanged)
    Q_PROPERTY(QString month READ getMonth NOTIFY dateChanged)
    Q_PROPERTY(int intMonth READ getIntMonth NOTIFY dateChanged)

public:
    HistoryEntry(QObject *parent=0) : QObject(parent) {}
    HistoryEntry(int time, int steps, QString date, QObject *parent=0)
        : QObject(parent), m_time(time), m_steps(steps), m_date(QDate::fromString(date, "yyyy-MM-dd")) {}
    HistoryEntry(int time, int steps, QDate date, QObject *parent=0)
        : QObject(parent), m_time(time), m_steps(steps), m_date(date) {}

    static QString formatTime(int seconds) {
        int sec = seconds % 60;
        int min = (seconds / 60) % 60;
        int hr = (seconds / 3600) % 24;

        QString ret;
        ret.sprintf("%.2d:%.2d:%.2d", hr, min, sec);
        return ret;
    }

    // hash based on Date
    int hash() {
        return hash(m_date);
    }
    static int hash(QDate d) {
        return d.dayOfYear() + ((int)(d.year() - 2011) * 365.25);
    }

    QDate getDate() const {
        return m_date;
    }

    QString getTime() const {
        return formatTime(m_time);
    }

    void plusTime(int s) {
        m_time += s;
    }

    void plusSteps(int s) {
        m_steps += s;
    }

    int getSteps() {
        return m_steps;
    }

    int getDay() {
        return m_date.day();
    }

    QString getMonth() {
        return m_date.toString("MMM yyyy");
    }

    int getIntMonth() {
        return m_date.month() + m_date.year() * 12;
    }

signals:
    void timeChanged();
    void stepsChanged();
    void dateChanged();

private:
    int m_time;
    int m_steps;
    QDate m_date;
};
#endif // HISTORYENTRY_H
