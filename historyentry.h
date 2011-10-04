#ifndef HISTORYENTRY_H
#define HISTORYENTRY_H

#include <QObject>

class HistoryEntry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString time READ getTime NOTIFY timeChanged)
    Q_PROPERTY(int steps READ getSteps NOTIFY stepsChanged)
    Q_PROPERTY(QString date READ getDate NOTIFY dateChanged)

public:
    HistoryEntry(QObject *parent=0) : QObject(parent) {}
    HistoryEntry(int time, int steps, QString date, QObject *parent=0)
        : QObject(parent), m_time(time), m_steps(steps), m_date(date) {}

    static QString formatTime(int seconds) {
        int sec = seconds % 60;
        int min = (seconds / 60) % 60;
        int hr = (seconds / 3600) % 24;

        QString ret;
        ret.sprintf("%.2d:%.2d:%.2d", hr, min, sec);
        return ret;
    }

    QString getTime() const {
        return formatTime(m_time);
    }

    int getSteps() {
        return m_steps;
    }

    QString getDate() {
        return m_date;
    }

signals:
    void timeChanged();
    void stepsChanged();
    void dateChanged();

private:
    int m_time;
    int m_steps;
    QString m_date;
};
#endif // HISTORYENTRY_H
