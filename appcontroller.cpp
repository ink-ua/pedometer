#include "appcontroller.h"

AppController* AppController::INSTANCE = 0;

AppController::AppController(QAccelerometer* s) :
    m_running(true), m_seconds(0), m_steps(0), //m_totalSteps(0), m_totalTime(0),
    sensor(s), settings("ink", "Pedometer"), m_lastSecond(0) {

    QObject::connect(this, SIGNAL(weightChanged()), this, SLOT(onWeightChanged()));

    for(int i = 0; i < LAST_STEPS_TIME; i++)
        m_lastSteps[i] = 0;

    m_stepLength = settings.value("step_length", QVariant(0.7)).toDouble();
    m_daily = settings.value("daily", QVariant(10000)).toDouble();
    m_weight = settings.value("weight", QVariant(70)).toDouble();
    weightChanged();
    m_sensitivity = settings.value("sensitivity", QVariant(0.75)).toDouble();
    m_inverted = settings.value("theme_inverted", QVariant(true)).toBool();
    m_freezeTimer = settings.value("freeze_timer", QVariant(true)).toBool();
    m_version = settings.value("version", QVariant("1.1.0")).toString();
    Formatter::getInstance()->setUnits(settings.value("units", QVariant(0)).toInt());

    m_currentDate = QDate::currentDate();

    QObject::connect(this, SIGNAL(unitsChanged()), this, SLOT(onUnitsChanged()));

    //qDebug() << "settings filename " << settings.fileName();
    //qDebug() << "m_version " << m_version;
}
