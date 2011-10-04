#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QDate>

#include "historyentry.h"

#define LAST_STEPS_TIME 10

class AppController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(double stepLength READ getStepLength WRITE setStepLength NOTIFY stepLengthChanged)
    Q_PROPERTY(double daily READ getDaily WRITE setDaily NOTIFY dailyChanged)
    Q_PROPERTY(int steps READ getSteps WRITE setSteps NOTIFY stepsChanged)
    Q_PROPERTY(int seconds READ getSeconds WRITE setSeconds)
    Q_PROPERTY(QString time READ getTime NOTIFY timeChanged)
    Q_PROPERTY(double distance READ getDistance NOTIFY distanceChanged)
    Q_PROPERTY(int totalSteps READ getTotalSteps NOTIFY totalStepsChanged)
    Q_PROPERTY(QString totalTime READ getTotalTime NOTIFY totalTimeChanged)
    Q_PROPERTY(QString avgSpeed READ getAvgSpeed NOTIFY avgSpeedChanged)
    Q_PROPERTY(QString speed READ getSpeed NOTIFY speedChanged)
    Q_PROPERTY(double todayDistance READ getTodayDistance NOTIFY todayDistanceChanged)
    Q_PROPERTY(double cal READ getCal NOTIFY calChanged)

public:
    AppController(QSqlDatabase db) : m_running(true), m_db(db), m_seconds(0), m_steps(0), m_totalSteps(0), m_totalTime(0), m_lastSteps(0) {
        QString sl = getVar("step_length");
        m_stepLength = sl.isEmpty() ? 0.65 : sl.toDouble();

        QString d = getVar("daily");
        m_daily = d.isEmpty() ? 5000.0 : d.toDouble();

        QString c = getVar("cal_per_step");
        m_calPerStep = c.isEmpty() ? 0.03 : c.toDouble();

        QSqlQuery queryTodaySteps = m_db.exec("SELECT SUM(steps) FROM history WHERE date = (date('now'))");
        m_todaySteps = queryTodaySteps.next() ? queryTodaySteps.value(0).toInt() : 0;

        QSqlQuery q = m_db.exec("SELECT seconds, steps, date FROM history ORDER BY date DESC");
        while(q.next()) {
            int t = q.value(0).toInt();
            int s = q.value(1).toInt();            
            m_totalTime += t;
            m_totalSteps += s;
            historyList.append(new HistoryEntry(t, s, q.value(2).toString()));
        }
        totalStepsChanged();
        totalTimeChanged();
    }

    ~AppController(){
        m_db.close();
        QSqlDatabase::removeDatabase(m_db.connectionName());
    }

    double getTodayDistance() {
        double today = (m_todaySteps + m_steps) * m_stepLength;
        return today;
    }

    double getStepLength() {
        return m_stepLength;
    }
    void setStepLength(double sl) {
        if(sl > 0) {
            m_stepLength = sl;
            setVar("step_length", QString::number(sl));
            stepLengthChanged();
        }
    }

    double getDaily() {
        return m_daily;
    }
    void setDaily(double d) {
        if(d > 0) {
            m_daily = d;
            setVar("daily", QString::number(m_daily));
            dailyChanged();
        }
    }

    bool isRunning() {
        return m_running;
    }
    void setRunning(bool r) {
        m_running = r;
        runningChanged();
    }

    int getTotalSteps() {
        return m_totalSteps;
    }

    int getSteps() {
        return m_steps;
    }
    void setSteps(int s) {
        m_steps = s;
        m_lastSteps++;
        stepsChanged();
        distanceChanged();
        todayDistanceChanged();
        avgSpeedChanged();
        calChanged();
    }

    double getCal() {
        return m_steps * m_calPerStep;
    }

    int getSeconds() {
        return m_seconds;
    }
    void setSeconds(int s) {
        m_seconds = s;
        timeChanged();
        avgSpeedChanged();
        if(m_seconds % LAST_STEPS_TIME == 0) {
            speedChanged();
            m_lastSteps = 0;
        }
    }

    QString getAvgSpeed() {
        double speed = (m_steps * m_stepLength * 3600) / m_seconds;
        return formatDistance(speed) + "/h";
    }

    QString getSpeed() {
        double speed = m_lastSteps * m_stepLength * (3600 / LAST_STEPS_TIME);
        return formatDistance(speed) + "/h";
    }

    static QString getCurrentDate() {
        return QDate::currentDate().toString("yyyy-MM-dd");
    }

    Q_INVOKABLE void save() {
        if(m_steps > 0) {
            QSqlQuery q(m_db);
            q.prepare("INSERT INTO history(seconds, steps) VALUES(:seconds, :steps)");
            q.bindValue(":seconds", m_seconds);
            q.bindValue(":steps", m_steps);
            q.exec();
            historyList.append(new HistoryEntry(m_seconds, m_steps, getCurrentDate()));
            m_totalSteps += m_steps;
            m_todaySteps += m_steps;
            m_totalTime += m_seconds;
            totalStepsChanged();
            totalTimeChanged();
        }
    }

    Q_INVOKABLE void reset() {
        save();
        setRunning(false);
        setSteps(0);
        setSeconds(0);
    }

    static QString formatTime(int seconds) {
        int sec = seconds % 60;
        int min = (seconds / 60) % 60;
        int hr = (seconds / 3600) % 24;

        QString ret;
        ret.sprintf("%.2d:%.2d:%.2d", hr, min, sec);
        return ret;
    }

    QString getTime() const {
        return formatTime(m_seconds);
    }

    QString getTotalTime() const {
        return formatTime(m_totalTime);
    }

    Q_INVOKABLE static QString formatDistance(double distance) {
        QString ret;
        if(distance > 1000)
            ret.sprintf("%.2f km", distance / 1000.0);
        else
            ret.sprintf("%.2f m", distance);
        return ret;
    }

    double getDistance() const {
        return m_steps * m_stepLength;
    }

    QList<QObject*> historyList;

private:
    bool m_running;
    int m_steps;
    int m_seconds;
    int m_totalSteps;
    int m_totalTime;
    double m_stepLength;
    QSqlDatabase m_db;
    double m_daily;
    int m_todaySteps;
    double m_calPerStep;
    int m_lastSteps;

    QString getVar(QString key) {
        QSqlQuery query(m_db);
        query.prepare("SELECT val FROM var WHERE key = :key");
        query.bindValue(":key", key);
        query.exec();

        if(query.next()) {
            QVariant v = query.value(0);
            //qDebug() << "selected " + v.toString();
            return v.toString();
        }
        else {
            //qDebug("nothing selected");
            return "";
        }
    }
    void setVar(QString key, QString val) {
        QSqlQuery query(m_db);
        query.prepare("UPDATE var SET val = :val WHERE key = :key");
        query.bindValue(":key", key);
        query.bindValue(":val", val);
        bool r = query.exec();
        //qDebug() << "updated " << (r ? "true" : "false");

        if(query.numRowsAffected() < 1) {
            //qDebug() << "not modified, key: " + key + ", val: " + val;
            QSqlQuery queryIns(m_db);
            queryIns.prepare("INSERT INTO var (key, val) VALUES (:key, :val)");
            queryIns.bindValue(":key", key);
            queryIns.bindValue(":val", val);
            bool res = queryIns.exec();
            //qDebug() << "inserted " << (res ? "true" : "false");
        }
    }

signals:
     void runningChanged();
     void stepsChanged();
     void timeChanged();
     void distanceChanged();
     void stepLengthChanged();
     void totalStepsChanged();
     void totalTimeChanged();
     void avgSpeedChanged();
     void dailyChanged();
     void todayDistanceChanged();
     void calChanged();
     void speedChanged();
};
#endif // APPCONTROLLER_H
