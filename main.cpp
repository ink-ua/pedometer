#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QAccelerometer>
#include <QDesktopServices>

#include "stephandler.h"
#include "appcontroller.h"

#define APP_NAME "Pedometer"
#define SQLITE_V ".sqlite3"

AppController* appController;

// we want descending order
bool historyLessThan(const QObject* h1, const QObject* h2) {
     return ((const HistoryEntry*)h1)->getDate() > ((const HistoryEntry*)h2)->getDate();
}

int main(int argc, char *argv[])
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString baseDataPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + APP_NAME;
    QDir().mkpath(baseDataPath);
    //qDebug() << baseDataPath;
    db.setDatabaseName(baseDataPath + "/pedometer" + SQLITE_V);
    if(!db.open())
        qDebug() << "Error opening database:" << db.lastError().number();

//    db.exec("DROP TABLE history");

    if(!db.tables().contains("history")) {
        qDebug("create history");
        db.exec("CREATE TABLE history(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, seconds INTEGER NOT NULL, steps INTEGER NOT NULL, date DATE NOT NULL DEFAULT (date('now')))");
    }

    QAccelerometer sensor;
    sensor.setDataRate(10);
    appController = new AppController(db, &sensor);
    StepHandler filter;
    sensor.addFilter(&filter);
    QObject::connect(&filter, SIGNAL(onStep()), appController, SLOT(incStep()));

    QApplication app(argc, argv);
    QDeclarativeView view;
    view.connect(view.engine(), SIGNAL(quit()), SLOT(close()));
    view.rootContext()->setContextProperty("appcontroller", appController);

    QList<QObject*> history = appController->historyList.values();
    qSort(history.begin(), history.end(), historyLessThan);
    view.rootContext()->setContextProperty("historyModel", QVariant::fromValue(history));

    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.showFullScreen();
    sensor.start();

    QObject::connect(&app, SIGNAL(lastWindowClosed()), appController, SLOT(onClose()));

    return app.exec();
}
