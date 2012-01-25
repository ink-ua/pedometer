#include "appcontroller.h"

AppController* AppController::INSTANCE = 0;

AppController::AppController(QSqlDatabase db, QAccelerometer* s) :
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
