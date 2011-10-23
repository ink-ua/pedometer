#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QDate>
#include <QAccelerometer>
#include <QSettings>
#include <QDate>
#include <math.h>

#include "historyentry.h"

QTM_USE_NAMESPACE

#define LAST_STEPS_TIME 10
#define EPS 0.0001

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
    Q_PROPERTY(int totalTime READ getTotalTime NOTIFY totalTimeChanged)
    Q_PROPERTY(QString avgSpeed READ getAvgSpeed NOTIFY avgSpeedChanged)
    Q_PROPERTY(QString speed READ getSpeed NOTIFY speedChanged)
    Q_PROPERTY(double todayDistance READ getTodayDistance NOTIFY todayDistanceChanged)
    Q_PROPERTY(double cal READ getCal NOTIFY calChanged)
    Q_PROPERTY(double calTotal READ getCalTotal NOTIFY calTotalChanged)
    Q_PROPERTY(double sensitivity READ getSensitivity WRITE setSensitivity NOTIFY sensitivityChanged)
    //Q_PROPERTY(QList history READ getHistory NOTIFY historyChanged)

public:
    AppController(QSqlDatabase db, QAccelerometer* s) :
        m_running(true), m_db(db), m_seconds(0), m_steps(0), m_totalSteps(0), m_totalTime(0),
        m_lastSteps(0), sensor(s), settings("ink", "Pedometer") {

        m_stepLength = settings.value("step_length", QVariant(0.7)).toDouble();
        m_daily = settings.value("daily", QVariant(10000)).toDouble();
        m_calPerStep = settings.value("cal_per_step", QVariant(0.03)).toDouble();
        m_sensitivity = settings.value("sensitivity", QVariant(0.75)).toDouble();

        QSqlQuery queryTodaySteps = m_db.exec("SELECT SUM(steps) FROM history WHERE date = (date('now'))");
        m_todaySteps = queryTodaySteps.next() ? queryTodaySteps.value(0).toInt() : 0;

        QString date;
        QSqlQuery q = m_db.exec("SELECT sum(seconds), sum(steps), date FROM history GROUP BY date ORDER BY date DESC");
        while(q.next()) {
            int t = q.value(0).toInt();
            int s = q.value(1).toInt();
            date = q.value(2).toString();
            m_totalTime += t;
            m_totalSteps += s;
            qDebug() << t << s << date;
            HistoryEntry* h = new HistoryEntry(t, s, date);
            historyList.insert(h->hash(), h);
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

    double getSensitivity() {
        return m_sensitivity;
    }
    void setSensitivity(double s) {
        if(s > 0) {
            m_sensitivity = s;
            settings.setValue("sensitivity", QVariant(s));
            emit sensitivityChanged();
        }
    }

    double getStepLength() {
        return m_stepLength;
    }
    void setStepLength(double sl) {
        if(sl > 0 && fabs(sl - m_stepLength) > EPS) {
            m_stepLength = sl;
            settings.setValue("step_length", QVariant(sl));
            stepLengthChanged();
        }
    }

    double getDaily() {
        return m_daily;
    }
    void setDaily(double d) {
        if(d > 0 && abs(d - m_daily) > EPS) {
            m_daily = d;
            settings.setValue("daily", QVariant(m_daily));
            dailyChanged();
        }
    }

    bool isRunning() {
        return m_running;
    }
    void setRunning(bool r) {
        m_running = r;
        runningChanged();
        if(r)
            sensor->start();
        else
            sensor->stop();
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

    double getCalTotal() {
        return m_totalSteps * m_calPerStep;
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
        double speed = 0;
        if(m_seconds != 0)
            speed = (m_steps * m_stepLength * 3600) / m_seconds;
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

            QDate currentDate = QDate::currentDate();
            int hash = HistoryEntry::hash(currentDate);
            if(historyList.contains(hash))  {
                HistoryEntry* h = (HistoryEntry*)historyList.value(hash);
                h->plusTime(m_seconds);
                h->plusSteps(m_steps);
            }
            else
                historyList.insert(hash, new HistoryEntry(m_seconds, m_steps, currentDate));

            m_totalSteps += m_steps;
            m_todaySteps += m_steps;
            m_totalTime += m_seconds;
            totalStepsChanged();
            totalTimeChanged();
            calTotalChanged();
            //historyChanged();
        }
    }

    Q_INVOKABLE void reset() {
        save();
        setRunning(false);
        setSteps(0);
        setSeconds(0);
    }

    Q_INVOKABLE static QString formatTime(int seconds) {
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

    int getTotalTime() const {
        return m_totalTime;
    }

    Q_INVOKABLE QString formatDistance(double distance) {
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

//    QList<QObject*> getHistory() {
//        return historyList;
//    }

    QHash<int, QObject*> historyList;

public slots:
    void incStep() {
        setSteps(getSteps() + 1);
    }
    void onClose() {
        save();
    }

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
    QAccelerometer *sensor;
    QSettings settings;
    double m_sensitivity;

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
     void calTotalChanged();
     void sensitivityChanged();
     //void historyChanged();
};
#endif // APPCONTROLLER_H
