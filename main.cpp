#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
//#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QAccelerometer>
#include <QDesktopServices>
//#include <qmdisplaystate.h>

#include "stephandler.h"
#include "historyprovider.h"
#include "appcontroller.h"

#define APP_NAME "Pedometer"
#define SQLITE_V ".sqlite3"


AppController* appController;

int main(int argc, char *argv[])
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString baseDataPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + APP_NAME;
    QDir().mkpath(baseDataPath);
    //qDebug() << baseDataPath;
    db.setDatabaseName(baseDataPath + "/pedometer" + SQLITE_V);
    if(!db.open())
        ;//qDebug() << "Error opening database:" << db.lastError().number();

//    db.exec("DROP TABLE history");

    if(!db.tables().contains("history")) {
        qDebug("create history");
        db.exec("CREATE TABLE history(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, seconds INTEGER NOT NULL, steps INTEGER NOT NULL, date DATE NOT NULL DEFAULT (date('now')))");
    }

    QAccelerometer sensor;
    sensor.setDataRate(10);
    sensor.setProperty("alwaysOn", true);
    appController = new AppController(db, &sensor);
    StepHandler filter;
    sensor.addFilter(&filter);
    QObject::connect(&filter, SIGNAL(onStep()), appController, SLOT(incStep()));

    HistoryProvider historyProvider;
    QObject::connect(appController, SIGNAL(entryAdded(int, int)), &historyProvider, SLOT(addEntry(int, int)));

    QApplication app(argc, argv);
    QDeclarativeView view;
    view.connect(view.engine(), SIGNAL(quit()), SLOT(close()));
    view.rootContext()->setContextProperty("appcontroller", appController);
    view.rootContext()->setContextProperty("historyProvider", &historyProvider);
    //view.rootContext()->setContextProperty("historyModel", (QObject*)&(appController->history));
    //view.rootContext()->setContextProperty("historyModel", QVariant::fromValue(*(appController->history.getList())));

//    Object* notif = view.rootObject()->findChild<QObject *>("goalReachedNotification");
//    appController->setGoalReachedNotificationObject(notif);

    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.showFullScreen();
    sensor.start();

    QObject::connect(&app, SIGNAL(lastWindowClosed()), appController, SLOT(onClose()));

    return app.exec();
}
