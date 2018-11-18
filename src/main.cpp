#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //create database
    QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("Database.db");
        database.setUserName("");
        database.setPassword("123456");
    }
    //open database
    if (!database.open())
    {
        qDebug()<<"Errror: Failed to connect to database."<< database.lastError();
    }
    else
    {
        qDebug()<<"Succeed to connect to database.";
    }
    MainWindow w;
    w.show();

    return a.exec();
}
