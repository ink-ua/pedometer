#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
//#include <QDebug>
#include <QDate>
#include <QAccelerometer>
#include <QSettings>
#include <QDate>
#include <math.h>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>

#include "historyentry.h"
//#include "historymodel.h"

QTM_USE_NAMESPACE

#define LAST_STEPS_TIME 10
#define SPEED_WEIGHT 2.0 / (1 + LAST_STEPS_TIME);
#define EPS 0.001

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
    Q_PROPERTY(QString avgSpeed READ getAvgSpeed NOTIFY avgSpeedChanged)
    Q_PROPERTY(QString speed READ getSpeed NOTIFY speedChanged)
    Q_PROPERTY(double todayDistance READ getTodayDistance NOTIFY todayDistanceChanged)
    Q_PROPERTY(double cal READ getCal NOTIFY calChanged)
    Q_PROPERTY(double sensitivity READ getSensitivity WRITE setSensitivity NOTIFY sensitivityChanged)
    Q_PROPERTY(bool inverted READ getInverted WRITE setInverted NOTIFY invertedChanged)
    Q_PROPERTY(double calPerStep READ getCalPerStep NOTIFY calPerStepChanged)
    Q_PROPERTY(bool freeze READ getFreeze WRITE setFreeze NOTIFY freezeChanged)

public:
    AppController(QSqlDatabase db, QAccelerometer* s) :
        m_running(true), m_db(db), m_seconds(0), m_steps(0), //m_totalSteps(0), m_totalTime(0),
        sensor(s), settings("ink", "Pedometer"), m_lastSecond(0) {

        for(int i = 0; i < LAST_STEPS_TIME; i++)
            m_lastSteps[i] = 0;

        m_stepLength = settings.value("step_length", QVariant(0.7)).toDouble();
        m_daily = settings.value("daily", QVariant(10000)).toDouble();
        m_calPerStep = settings.value("cal_per_step", QVariant(0.03)).toDouble();
        m_sensitivity = settings.value("sensitivity", QVariant(0.75)).toDouble();
        m_inverted = settings.value("theme_inverted", QVariant(true)).toBool();
        m_freezeTimer = settings.value("freeze_timer", QVariant(true)).toBool();

        QSqlQuery queryTodaySteps = m_db.exec("SELECT SUM(steps) FROM history WHERE date = (date('now'))");
        m_todaySteps = queryTodaySteps.next() ? queryTodaySteps.value(0).toInt() : 0;

//        QString date;
//        QSqlQuery q = m_db.exec("SELECT SUM(seconds), SUM(steps), date FROM history GROUP BY date"); // ORDER BY date DESC
//        while(q.next()) {
//            int t = q.value(0).toInt();
//            int s = q.value(1).toInt();
//            date = q.value(2).toString();
//            m_totalTime += t;
//            m_totalSteps += s;
//            //qDebug() << t << s << date;
//            HistoryEntry* h = new HistoryEntry(t, s, date);
//            //history.getList()->push_front(h);
//            history.push_front(h);//insertRow(0, h);
//        }
//        totalStepsChanged();
//        totalTimeChanged();
    }

    ~AppController(){
        m_db.close();
        QSqlDatabase::removeDatabase(m_db.connectionName());
    }

    double getCalPerStep() {
        return m_calPerStep;
    }

    double getTodayDistance() {
        double today = (m_todaySteps + m_steps) * m_stepLength;
        return today;
    }

    double getSensitivity() {
        return m_sensitivity;
    }
    void setSensitivity(double s) {
        if(s > 0 && fabs(m_sensitivity - s) > EPS) {
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
        if(d > 0 && fabs(d - m_daily) > EPS) {
            m_daily = d;
            settings.setValue("daily", QVariant(m_daily));
            dailyChanged();
        }
    }

    bool isRunning() {
        return m_running;
    }
    void setRunning(bool r) {
        if(r != m_running) {
            m_running = r;
            runningChanged();
            if(r)
                sensor->start();
            else
                sensor->stop();
        }
    }

//    int getTotalSteps() {
//        return m_totalSteps;
//    }

    int getSteps() {
        return m_steps;
    }
    void setSteps(int s) {
        if(m_steps != s) {
            m_steps = s;
            m_lastSteps[m_lastSecond]++;
            stepsChanged();
            distanceChanged();
            todayDistanceChanged();
            avgSpeedChanged();
            calChanged();
            m_lastStepTimestamp = QDateTime::currentMSecsSinceEpoch();
        }

//        // view notification when daily goal reached
//        if(m_steps * m_stepLength >= m_daily && (m_steps - 1) * m_stepLength < m_daily) {
////            QDeclarativeEngine engine;
////            QDeclarativeComponent component(&engine, QUrl::fromLocalFile("Notification.qml"));
//            //QObject *object = ;//component.create();
//            QMetaObject::invokeMethod(m_goalReachedNotification, "open");
//            //delete object;
//        }
    }

    double getCal() {
        return m_steps * m_calPerStep;
    }

//    double getCalTotal() {
//        return m_totalSteps * m_calPerStep;
//    }

    int getSeconds() {
        return m_seconds;
    }
    void setSeconds(int s) {
        if(m_seconds != s) {
            if(!m_freezeTimer || QDateTime::currentMSecsSinceEpoch() - m_lastStepTimestamp < 2000) {
                m_seconds = s;
                timeChanged();
                avgSpeedChanged();
            }

            if(m_lastSecond == LAST_STEPS_TIME - 1) {
                speedChanged();
                for(int i = 0; i < LAST_STEPS_TIME - 1; i++)
                    m_lastSteps[i] = m_lastSteps[i + 1];
                m_lastSteps[LAST_STEPS_TIME - 1] = 0;
            } else {
                m_lastSecond++;
            }
        }
    }

//    Q_INVOKABLE void loadHistory() {
//        QDeclarativeEngine engine;
//        QDeclarativeComponent component(&engine, QUrl::fromLocalFile("History.qml"));
//        QObject* model = component.findChild<QObject*>("historyModel");
//        //QObject *object = component.create();
//        QMetaObject::invokeMethod(model, "append", (QObject*)new HistoryEntry(60, 10, "2011-10-10"));
//                    //delete object;

//    }

    QString getAvgSpeed() {
        double speed = 0;
        if(m_seconds != 0)
            speed = (m_steps * m_stepLength * 3600) / m_seconds;
        return formatDistance(speed) + "/h";
    }

    QString getSpeed() {
        double speed = 0;

        double sum = 0;
        for(int i = 0; i < LAST_STEPS_TIME; i++)
            sum += m_lastSteps[i] * (i + 1) * SPEED_WEIGHT;

        if(isRunning())
            speed = sum * m_stepLength * (3600 / LAST_STEPS_TIME);
        return formatDistance(speed) + "/h";
    }

    static QString getCurrentDate() {
        return QDate::currentDate().toString("yyyy-MM-dd");
    }

    Q_INVOKABLE void save() {
        if(m_steps > 0) {
//            QSqlQuery q(m_db);
//            q.prepare("INSERT INTO history(seconds, steps) VALUES(:seconds, :steps)");
//            q.bindValue(":seconds", m_seconds);
//            q.bindValue(":steps", m_steps);
//            q.exec();

            emit entryAdded(m_seconds, m_steps);

//            QDate currentDate = QDate::currentDate();
//            HistoryEntry* first;
//            if(history.rowCount() > 0 && (first = (HistoryEntry*)history.get(0))->getDate() == currentDate) {
//                first->plusSteps(m_steps);
//                first->plusTime(m_seconds);
//            }
//            else
//                history.insertRow(0, new HistoryEntry(m_seconds, m_steps, currentDate));

            //m_totalSteps += m_steps;
            m_todaySteps += m_steps;
//            m_totalTime += m_seconds;
//            totalStepsChanged();
//            totalTimeChanged();
//            calTotalChanged();
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

//    int getTotalTime() const {
//        return m_totalTime;
//    }

    Q_INVOKABLE QString formatDistance(double distance) {
        QString ret;
        if(distance > 1000)
            ret.sprintf("%.2f km", distance / 1000.0);
        else
            ret.sprintf("%.2f m", distance);
        return ret;
    }

    Q_INVOKABLE QString formatPercent(double v) {
        QString ret;
        ret.sprintf("%.2f %", v);
        return ret;
    }

    double getDistance() const {
        return m_steps * m_stepLength;
    }

    bool getInverted() {
        return m_inverted;
    }
    void setInverted(bool i) {
        if(i != m_inverted) {
            m_inverted = i;
            settings.setValue("theme_inverted", QVariant(m_inverted));
            emit invertedChanged();
        }
    }

    bool getFreeze() {
        return m_freezeTimer;
    }
    void setFreeze(bool f) {
        if(f != m_freezeTimer) {
            m_freezeTimer = f;
            settings.setValue("freeze_timer", QVariant(m_freezeTimer));
            emit freezeChanged();
        }
    }

//    void setGoalReachedNotification(QObject* n) {
//        m_goalReachedNotification = n;
//    }

    //HistoryModel history;
    QSqlDatabase m_db;

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
    double m_stepLength;
    double m_daily;
    int m_todaySteps;
    double m_calPerStep;
    short m_lastSteps[LAST_STEPS_TIME];
    short m_lastSecond;
    QAccelerometer *sensor;
    QSettings settings;
    double m_sensitivity;
    bool m_inverted;
    quint64 m_lastStepTimestamp;
    bool m_freezeTimer;

signals:
     void runningChanged();
     void stepsChanged();
     void timeChanged();
     void distanceChanged();
     void stepLengthChanged();
     void avgSpeedChanged();
     void dailyChanged();
     void todayDistanceChanged();
     void calChanged();
     void speedChanged();
     void sensitivityChanged();
     void invertedChanged();
     void calPerStepChanged();
     void freezeChanged();
     void entryAdded(int t, int s);
};
#endif // APPCONTROLLER_H
