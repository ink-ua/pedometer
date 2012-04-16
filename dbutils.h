#ifndef DBUTILS_H
#define DBUTILS_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtDeclarative>
//#include <QDebug>

#include "appcontroller.h"

#define SQLITE_V ".sqlite3"

class DBUtils : public QObject {
    Q_OBJECT
public:
    static void init(QString basePath) {
        instance = new DBUtils(basePath);
    }

    static DBUtils* getInstance() {
        return instance;
    }

    void setUp() {
        if(!m_db.open()) {
            //qDebug() << "Error opening database:" << db.lastError().number();
        }

        //db.exec("DROP TABLE history");

        // create table if not exists
        if(!m_db.tables().contains("history")) {
            //qDebug("create history");
            m_db.exec("CREATE TABLE history(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, seconds INTEGER NOT NULL, steps INTEGER NOT NULL, distance REAL, calories REAL, date DATE NOT NULL DEFAULT (date('now','localtime'))); CREATE INDEX date_idx ON history(date);");
        }
    }

    double getTodayDistance() const {
        //qDebug() << "getTodayDistance()";
        QSqlQuery queryToday = m_db.exec("SELECT SUM(distance) FROM history WHERE date = (date('now','localtime'))");
        return queryToday.next() ? queryToday.value(0).toDouble() : 0;
    }

    QSqlQuery getTotalsByDay() const {
        //qDebug() << "getTotalsByDay()";
        QSqlQuery q = m_db.exec("SELECT SUM(seconds), SUM(steps), SUM(distance), SUM(calories), date FROM history GROUP BY date ORDER BY date DESC");
        //qDebug() << q.lastError();
        return q;
    }

//    const QSqlDatabase& getDB() const {
//        return m_db;
//    }

public slots:

    void onVersionChanged(QString oldVersion, QString newVersion) {
        // add distance and calories columns migrating from version 1.1
        //qDebug() << "onVersionChanged()";
        if(oldVersion == "1.1.0") {
            //qDebug() << "1.1";
            QSqlQuery q(m_db);
            if(q.exec("ALTER TABLE history ADD COLUMN distance REAL") == false)
                qDebug() << "error altering" << m_db.lastError().text();;
            if(q.exec("ALTER TABLE history ADD COLUMN calories REAL") == false)
                qDebug() << "error altering2" << m_db.lastError().text();;
            q.prepare("UPDATE history SET distance=steps*:stepLength, calories=steps*:calPerStep");
            q.bindValue(":stepLength", AppController::getInstance()->getStepLength());
            q.bindValue(":calPerStep", AppController::getInstance()->getCalPerStep());
            if(q.exec() == false)
                ;//qDebug() << "error updating" << m_db.lastError().text();
        }
    }

    void addEntry(int t, int s, double d, double c, QDate date) {
        //qDebug() << "addEntry()";
        QSqlQuery q(m_db);
        q.prepare("INSERT INTO history(seconds, steps, distance, calories, date) VALUES(:seconds, :steps, :distance, :calories, :date)");
        q.bindValue(":seconds", t);
        q.bindValue(":steps", s);
        q.bindValue(":distance", d);
        q.bindValue(":calories", c);
        q.bindValue(":date", date.toString("yyyy-MM-dd"));
        if(q.exec() == false)
            ;//qDebug() << m_db.lastError().text();
    }

    void onClose() {
        //qDebug("onClose()");
        m_db.close();
        QSqlDatabase::removeDatabase(m_db.connectionName());
    }

private:
    DBUtils(QString&);
    static DBUtils* instance;

    QSqlDatabase m_db;
};

#endif // DBUTILS_H
