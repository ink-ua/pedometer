#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
//#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QAccelerometer>
#include <QDesktopServices>
//#include <qmdisplaystate.h>

#include "appcontroller.h"
#include "rotationfilter.h"
#include "stephandler.h"
#include "historyprovider.h"

#define APP_NAME "Pedometer"
#define SQLITE_V ".sqlite3"

int main(int argc, char *argv[])
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString baseDataPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + APP_NAME;
    QDir().mkpath(baseDataPath);
    //qDebug() << baseDataPath;
    db.setDatabaseName(baseDataPath + "/pedometer" + SQLITE_V);
    if(!db.open()) {
        //qDebug() << "Error opening database:" << db.lastError().number();
    }

//    db.exec("DROP TABLE history");

    if(!db.tables().contains("history")) {
        //qDebug("create history");
        db.exec("CREATE TABLE history(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, seconds INTEGER NOT NULL, steps INTEGER NOT NULL, date DATE NOT NULL DEFAULT (date('now'))); CREATE INDEX date_idx ON history(date);");
    }

    // set up sensors
//    QRotationSensor rotation;
//    rotation.setDataRate(10);
//    rotation.setProperty("alwaysOn", true);
    QAccelerometer sensor;
    sensor.setDataRate(20);
    sensor.setProperty("alwaysOn", true);    
    AppController::init(db, &sensor);
    StepHandler filter;
    sensor.addFilter(&filter);
//    RotationFilter rfilter;
//    rotation.addFilter(rfilter);
    QObject::connect(&filter, SIGNAL(onStep()), AppController::getInstance(), SLOT(incStep()));

    HistoryProvider historyProvider;
    QObject::connect(AppController::getInstance(), SIGNAL(entryAdded(int, int)), &historyProvider, SLOT(addEntry(int, int)));

    QApplication app(argc, argv);
    QDeclarativeView view;
    view.connect(view.engine(), SIGNAL(quit()), SLOT(close()));
    view.rootContext()->setContextProperty("appcontroller", AppController::getInstance());
    view.rootContext()->setContextProperty("historyProvider", &historyProvider);
    //view.rootContext()->setContextProperty("historyModel", (QObject*)&(appController->history));
    //view.rootContext()->setContextProperty("historyModel", QVariant::fromValue(*(appController->history.getList())));

//    Object* notif = view.rootObject()->findChild<QObject *>("goalReachedNotification");
//    appController->setGoalReachedNotificationObject(notif);

    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.showFullScreen();
    sensor.start();

    QObject::connect(&app, SIGNAL(lastWindowClosed()), AppController::getInstance(), SLOT(onClose()));

    return app.exec();
}
