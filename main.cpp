#include <QtGui/QApplication>
#include <QtDeclarative>
//#include <QDebug>
#include <QAccelerometer>
#include <QDesktopServices>
//#include <qmdisplaystate.h>

#include "appcontroller.h"
#include "stephandler.h"
#include "historyprovider.h"
#include "historymodel.h"
#include "dbutils.h"

int main(int argc, char *argv[])
{
    QString baseDataPath(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "Pedometer");
    QDir().mkpath(baseDataPath);

    DBUtils::init(baseDataPath);
    DBUtils::getInstance()->setUp();

    QAccelerometer sensor;
    sensor.setDataRate(20);
    sensor.setProperty("alwaysOn", true);

    AppController::init(&sensor);
    QObject::connect(AppController::getInstance(), SIGNAL(versionChanged(QString, QString)), DBUtils::getInstance(), SLOT(onVersionChanged(QString, QString)));
    AppController::getInstance()->updateVersion();
    AppController::getInstance()->setTodayDistance(DBUtils::getInstance()->getTodayDistance());

    StepHandler filter;
    sensor.addFilter(&filter);
    QObject::connect(&filter, SIGNAL(onStep()), AppController::getInstance(), SLOT(incStep()));

    HistoryModel historyModel;
    HistoryProvider historyProvider(historyModel);
    QObject::connect(AppController::getInstance(), SIGNAL(entryAdded(int, int, double, double, QDate)), &historyProvider, SLOT(addEntry(int, int, double, double, QDate)));
    QObject::connect(AppController::getInstance(), SIGNAL(entryAdded(int, int, double, double, QDate)), DBUtils::getInstance(), SLOT(addEntry(int, int, double, double, QDate)));

    QApplication app(argc, argv);
    QDeclarativeView view;
    view.connect(view.engine(), SIGNAL(quit()), SLOT(close()));
    view.rootContext()->setContextProperty("appcontroller", AppController::getInstance());
    view.rootContext()->setContextProperty("historyProvider", &historyProvider);
    view.rootContext()->setContextProperty("historyModel", &historyModel);
    view.rootContext()->setContextProperty("formatter", Formatter::getInstance());

//    Object* notif = view.rootObject()->findChild<QObject *>("goalReachedNotification");
//    appController->setGoalReachedNotificationObject(notif);

    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.showFullScreen();
    sensor.start();

    QObject::connect(&app, SIGNAL(lastWindowClosed()), AppController::getInstance(), SLOT(onClose()));
    QObject::connect(&app, SIGNAL(lastWindowClosed()), DBUtils::getInstance(), SLOT(onClose()));

    return app.exec();
}
