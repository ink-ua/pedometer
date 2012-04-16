#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QString>
#include <QVariant>
//#include <QDebug>
#include <QDate>
#include <QAccelerometer>
#include <QSettings>
#include <QDate>
#include <math.h>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>

#include "formatter.h"

QTM_USE_NAMESPACE

#define LAST_STEPS_TIME 10
#define SPEED_WEIGHT 2.0 / (1 + LAST_STEPS_TIME);
#define EPS 0.001

#define CALORIES_PER_STEP_BASE 0.04
#define BASE_WEIGHT 70 // kg
#define CALORIES_WEIGHT_FACTOR 0.0005

#define CURRENT_VERSION "1.2.0"

class AppController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(double stepLength READ getStepLength WRITE setStepLength NOTIFY stepLengthChanged)
    Q_PROPERTY(double daily READ getDaily WRITE setDaily NOTIFY dailyChanged)
    Q_PROPERTY(int steps READ getSteps WRITE setSteps NOTIFY stepsChanged)
    Q_PROPERTY(int seconds READ getSeconds WRITE setSeconds NOTIFY secondsChanged)
    Q_PROPERTY(double distance READ getDistance NOTIFY distanceChanged)
    Q_PROPERTY(double avgSpeed READ getAvgSpeed NOTIFY avgSpeedChanged)
    Q_PROPERTY(double speed READ getSpeed NOTIFY speedChanged)
    Q_PROPERTY(double todayDistance READ getTodayDistance NOTIFY todayDistanceChanged)
    Q_PROPERTY(double cal READ getCal NOTIFY calChanged)
    Q_PROPERTY(double sensitivity READ getSensitivity WRITE setSensitivity NOTIFY sensitivityChanged)
    Q_PROPERTY(bool inverted READ getInverted WRITE setInverted NOTIFY invertedChanged)
    Q_PROPERTY(double calPerStep READ getCalPerStep NOTIFY calPerStepChanged)
    Q_PROPERTY(bool freeze READ getFreeze WRITE setFreeze NOTIFY freezeChanged)
    Q_PROPERTY(int units READ getUnits WRITE setUnits NOTIFY unitsChanged)
    Q_PROPERTY(double weight READ getWeight WRITE setWeight NOTIFY weightChanged)

public:
    static void init(QAccelerometer* s) {
        INSTANCE = new AppController(s);
    }
    static AppController* getInstance() {
        return INSTANCE;
    }

    double getCalPerStep() {
        return m_calPerStep;
    }

    void setTodayDistance(double td) {
        m_todayDistance = td;
    }
    double getTodayDistance() const {
        double today = m_todayDistance + m_steps * m_stepLength;
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

    double getWeight() {
        return m_weight;
    }
    void setWeight(double w) {
        if(w > 0 && fabs(m_weight - w) > EPS) {
            m_weight = w;
            settings.setValue("weight", QVariant(w));
            emit weightChanged();
        }
    }

    int getUnits() {
        return Formatter::getInstance()->getUnits();
    }
    void setUnits(int u) {
        if(u != getUnits()) {
            Formatter::getInstance()->setUnits(u);
            settings.setValue("units", QVariant(u));
            emit unitsChanged();
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
        return m_steps * getCalPerStep();
    }

    int getSeconds() {
        return m_seconds;
    }
    void setSeconds(int s) {
        if(m_seconds != s) {
            if(!m_freezeTimer || QDateTime::currentMSecsSinceEpoch() - m_lastStepTimestamp < 2000) {
                m_seconds = s;
                emit secondsChanged();
                emit avgSpeedChanged();
            }

            if(m_lastSecond == LAST_STEPS_TIME - 1) {
                emit speedChanged();
                for(int i = 0; i < LAST_STEPS_TIME - 1; i++)
                    m_lastSteps[i] = m_lastSteps[i + 1];
                m_lastSteps[LAST_STEPS_TIME - 1] = 0;
            } else {
                m_lastSecond++;
            }

            // reset after midnight
            QDate newCurrent = QDate::currentDate();
            if(m_currentDate != newCurrent) {
                reset();
                m_todayDistance = 0;
                m_currentDate = newCurrent;
                setRunning(true);
            }
        }
    }

    double getAvgSpeed() {
        double speed = 0;
        if(m_seconds > 0 && m_steps > 0) {
            speed = calculateSpeed(m_steps * m_stepLength, m_seconds);
        }
        return speed;
    }

    double getSpeed() {
        double speed = 0;

        double sum = 0;
        for(int i = 0; i < LAST_STEPS_TIME; i++)
            sum += m_lastSteps[i] * (i + 1) * SPEED_WEIGHT;

        if(isRunning()) {
            speed = calculateSpeed(sum * m_stepLength, LAST_STEPS_TIME);
        }
        return speed;
    }

    QDate getCurrentDate() {
        return m_currentDate;
    }

    Q_INVOKABLE void save() {
        if(m_steps > 0) {
            emit entryAdded(m_seconds, m_steps, getDistance(), getCal(), m_currentDate);
            m_todayDistance += m_steps * getStepLength();
        }
    }

    Q_INVOKABLE void reset() {
        save();
        setRunning(false);
        setSteps(0);
        setSeconds(0);
    }

    Q_INVOKABLE double calculateSpeed(double distance, int seconds) {
        return (distance * 3600) / seconds;
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

    QString getVersion() const {
        return m_version;
    }
    void updateVersion() {
        if(m_version != CURRENT_VERSION) {
            emit versionChanged(m_version, CURRENT_VERSION);
            m_version = CURRENT_VERSION;
            settings.setValue("version", QVariant(m_version));
        }
    }

    Q_INVOKABLE double calculateRate(double distance) {
        return distance / (m_daily / 100.0);
    }

//    void setGoalReachedNotification(QObject* n) {
//        m_goalReachedNotification = n;
//    }

public slots:

    void incStep() {
        setSteps(getSteps() + 1);
    }

    void onClose() {
        save();
    }

    void onWeightChanged() {
        m_calPerStep = CALORIES_PER_STEP_BASE + (m_weight - BASE_WEIGHT) * CALORIES_WEIGHT_FACTOR;
    }

    void onUnitsChanged() {
        emit distanceChanged();
        emit speedChanged();
        emit todayDistanceChanged();
        emit dailyChanged();
        emit avgSpeedChanged();
    }

private:
    AppController(QAccelerometer* s);

    static AppController* INSTANCE;

    bool m_running;
    int m_steps;
    int m_seconds;
    double m_stepLength;
    double m_daily;
    double m_todayDistance;
    double m_weight;
    double m_calPerStep;
    short m_lastSteps[LAST_STEPS_TIME];
    short m_lastSecond;
    QAccelerometer *sensor;
    QSettings settings;
    double m_sensitivity;
    bool m_inverted;
    quint64 m_lastStepTimestamp;
    bool m_freezeTimer;
    QString m_version;
    QDate m_currentDate;

signals:
     void runningChanged();
     void stepsChanged();
     void secondsChanged();
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
     void weightChanged();
     void freezeChanged();
     void unitsChanged();
     void entryAdded(int t, int s, double d, double c, QDate date);
     void versionChanged(QString oldVersion, QString newVersion);
};
#endif // APPCONTROLLER_H
