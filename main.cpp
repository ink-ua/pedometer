#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QtSql/QSqlQuery>
#include "appcontroller.h"

int main(int argc, char *argv[])
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("Pedometer");
    if(!db.open()) {
        qDebug() << db.lastError().number();
        return 1;
    }

//    db.exec("DROP TABLE var");
//    db.exec("DROP TABLE history");

    if(!db.tables().contains("var")) {
        QSqlQuery query(db);
        bool r = query.exec("CREATE TABLE var(key VARCHAR(64) NOT NULL PRIMARY KEY, val TEXT NOT NULL)");
        qDebug() << "create var " << (r ? "true" : "false");
    }
    if(!db.tables().contains("history")) {
        qDebug("create history");
        db.exec("CREATE TABLE history(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, seconds INTEGER NOT NULL, steps INTEGER NOT NULL, date DATE NOT NULL DEFAULT (date('now')))");
    }

    AppController * appController = new AppController(db);

    QApplication app(argc, argv);
    QDeclarativeView view;
    view.connect(view.engine(), SIGNAL(quit()), SLOT(close()));
    view.rootContext()->setContextProperty("appcontroller", appController);
    view.rootContext()->setContextProperty("historyModel", QVariant::fromValue(appController->historyList));
    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.showFullScreen();
    return app.exec();
}
