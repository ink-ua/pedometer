#include "dbutils.h"

DBUtils* DBUtils::instance = 0;

DBUtils::DBUtils(QString& baseDataPath) :
    m_db(QSqlDatabase::addDatabase("QSQLITE"))
{
    //qDebug() << baseDataPath + "/pedometer" + SQLITE_V;
    m_db.setDatabaseName(baseDataPath + "/pedometer" + SQLITE_V);
}
