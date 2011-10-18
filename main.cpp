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

int main(int argc, char *argv[])
{
    QAccelerometer sensor;
    sensor.setDataRate(10);
    StepHandler filter;
    sensor.addFilter(&filter);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString baseDataPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    qDebug() << baseDataPath;
    db.setDatabaseName(baseDataPath + APP_NAME + "/pedometer" + SQLITE_V);
    if(!db.open())
        qDebug() << "Error opening database:" << db.lastError().number();

//    db.exec("DROP TABLE history");

    if(!db.tables().contains("history")) {
        qDebug("create history");
        db.exec("CREATE TABLE history(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, seconds INTEGER NOT NULL, steps INTEGER NOT NULL, date DATE NOT NULL DEFAULT (date('now')))");
    }

    AppController * appController = new AppController(db, &sensor);
    QObject::connect(&filter, SIGNAL(onStep()), appController, SLOT(incStep()));

    QApplication app(argc, argv);
    QDeclarativeView view;
    view.connect(view.engine(), SIGNAL(quit()), SLOT(close()));
    view.rootContext()->setContextProperty("appcontroller", appController);
    view.rootContext()->setContextProperty("historyModel", QVariant::fromValue(appController->historyList));
    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.show();
    sensor.start();
    return app.exec();
}
