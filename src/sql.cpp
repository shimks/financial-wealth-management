#include "sql.h"

sql::sql()
{

}

void sql::close()
{
        this->database.close();
}

void sql::connectsql()
{
    //QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("Database.db");
        database.setUserName("root");
        database.setPassword("123456");
    }
    //open database
    if (!database.open())
    {
        qDebug()<<"Errror: Failed to connect to database."<< database.lastError();
    }
    else
    {
        sql_query = new QSqlQuery();
        //sql_query;
        QString create_sql = "create table database(Gategory varchar(20) primary key, Amount int, Remaining int,"
                             "Monday int default 0, Tuesday int default 0, Wednesday int default 0, Thursday int default 0, "
                             "Friday int default 0, Saturday int default 0, Sunday int default 0)";
        sql_query->prepare(create_sql);
        if(!sql_query->exec())
        {
            qDebug() << "Error: Failed to create table."<< sql_query->lastError();
        }
        else
        {
            qDebug() << "Table created";
        }
        QString insert_sql = "insert into database values (?, ?, ?,?,?,?,?,?,?,?)";
        sql_query->prepare(insert_sql);
        sql_query->addBindValue("Groceries");
        sql_query->addBindValue(0);
        sql_query->addBindValue(0);
        sql_query->addBindValue(0);
        sql_query->addBindValue(0);
        sql_query->addBindValue(0);
        sql_query->addBindValue(0);
        sql_query->addBindValue(0);
        sql_query->addBindValue(0);
        sql_query->addBindValue(0);
        if(!sql_query->exec())
        {
            qDebug() << sql_query->lastError();
        }
        else
        {
            qDebug() << "inserted";
        }
        if(!sql_query->exec("INSERT INTO database VALUES(\"Hygiene Products\", 0, 0,0,0,0,0,0,0,0)"))
        {
            qDebug() << sql_query->lastError();
        }
        else
        {
            sql_query->exec("INSERT INTO database VALUES(\"Transportation\", 0, 0,0,0,0,0,0,0,0)");
            sql_query->exec("INSERT INTO database VALUES(\"Personal\", 0, 0,0,0,0,0,0,0,0)");
            qDebug() << "inserted succeed!";
        }
        sql_query->exec("INSERT INTO database VALUES(\"Food\", 0, 0,0,0,0,0,0,0,0)");
        sql_query->exec("INSERT INTO database VALUES(\"School Supplies\", 0, 0,0,0,0,0,0,0,0)");

    }
}

void sql::update(QSqlQuery* sql_query,QString db,QString gategory, QString amount)
{
    QString update_sql = QString("update %1 set Amount = :Amount, Remaining = :Remaining where Gategory = :Gategory").arg(db);
    sql_query->prepare(update_sql);
    sql_query->bindValue(":Amount", amount.toInt());
    sql_query->bindValue(":Remaining", amount.toInt());
    sql_query->bindValue(":Gategory", gategory);

    if(!sql_query->exec())
    {
        qDebug() << "error" <<  sql_query->lastError();
    }
    else
    {
        qDebug() << "updated!";
    }
}

void sql::updateExpenditure(QSqlQuery* sql_query, QString db, QString gategory, QString old,QString oldExpense, QString amount, QString day)
{
    QString update_sql = QString("update %3 set Remaining = :Remaining, %1 = :%2 where Gategory = :Gategory")
            .arg(day)
//            .arg(oldExpense.toInt()+amount.toInt())
            .arg(day)
            .arg(db);
    sql_query->prepare(update_sql);
    sql_query->bindValue(":Remaining", old.toInt() - amount.toInt());
    sql_query->bindValue(QString(":%1").arg(day), oldExpense.toInt()+amount.toInt());
    sql_query->bindValue(":Gategory", gategory);

    if(!sql_query->exec())
    {
        qDebug() << "error" <<  sql_query->lastError();
    }
    else
    {
        qDebug() << "updated!";
    }
}

void sql::updateExpenditureD(QSqlQuery* sql_query, QString db, QString gategory, QString old, QString oldexpense, QString amount, QString day)
{
    QString update_sql = QString("update %3 set Remaining = :Remaining, %1 = :%2 where Gategory = :Gategory").arg(day).arg(day).arg(db);
    sql_query->prepare(update_sql);
    sql_query->bindValue(":Remaining", old.toInt() + amount.toInt());
    sql_query->bindValue(QString(":%1").arg(day), oldexpense.toInt()-amount.toInt());
    sql_query->bindValue(":Gategory", gategory);

    if(!sql_query->exec())
    {
        qDebug() << "error" <<  sql_query->lastError();
    }
    else
    {
        qDebug() << "updated!";
    }
}


QString sql::select(QSqlQuery* sql_query, QString db, QString gategory,int i)
{
    //sql_query->prepare(QString("SELECT Remaining FROM database WHERE Gategory = %1").arg(gategory));
    //sql_query->next();
    QString name;
    QString select_sql = QString("select * from %1").arg(db);
    if(!sql_query->exec(select_sql))
    {
        qDebug()<<sql_query->lastError();
    }
    else
    {
        while(sql_query->next())
        {
            if (sql_query->value(0).toString() == gategory)
                name = sql_query->value(i).toString();
        }
    }
    qDebug() << name;
    return name;
}

void sql::print(QSqlQuery* sql_query, QString db)
{
    QString select_sql = QString("select * from %1").arg(db);
    if(!sql_query->exec(select_sql))
    {
        qDebug()<<sql_query->lastError();
    }
    else
    {
        while(sql_query->next())
        {
            QString name = sql_query->value(0).toString();
            int Amount = sql_query->value(1).toInt();
            int Remaining = sql_query->value(2).toInt();
            qDebug() << QString("Groceries:%1").arg(name)
                     << "\t\t"
                     << QString("Amount:%1").arg(Amount)
                     << "\t"
                     << QString("Remaining:%1").arg(Remaining)
                     << "\t"
                     << QString("Monday:%1").arg(sql_query->value(3).toInt())
                     << "\t"
                     << QString("Tuesday:%1").arg(sql_query->value(4).toInt())
                     << "\t"
                     << QString("Wednesday:%1").arg(sql_query->value(5).toInt())
                     << "\t"
                     << QString("Thursday:%1").arg(sql_query->value(6).toInt())
                     << "\t"
                     << QString("Friday:%1").arg(sql_query->value(7).toInt())
                     << "\t"
                     << QString("Saturday:%1").arg(sql_query->value(8).toInt())
                     << "\t"
                     << QString("Sunday:%1").arg(sql_query->value(9).toInt());
        }
    }
}

void sql::connectsql1()
{
    QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection1"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection1");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("Database.db");
        database.setUserName("root");
        database.setPassword("123456");
    }
    //open database
    if (!database.open())
    {
        qDebug()<<"Errror: Failed to connect to database."<< database.lastError();
    }
    else
    {
        sql_query1 = new QSqlQuery();
        //sql_query;
        QString create_sql = "create table database1(Gategory varchar(20) primary key, Amount int, Remaining int,"
                             "One int default 0, Tow int default 0, Three int default 0, Four int default 0, "
                             "Five int default 0, Six int default 0, Seven int default 0, Eight int default 0,"
                             "Ninety int default 0, Ten int default 0, Eleven int default 0, Twelve int default 0,"
                             "Thirteen int default 0, Fourteen int default 0, Fifteen int default 0, Sixteen int default 0,"
                             "Seventeen int default 0, Eighteen int default 0, Nineteen int default 0, Twenty int default 0,"
                             "TwentyOne int default 0, TwentyTwo int default 0, TwentyThree int default 0, TwentyFour int default 0,"
                             "TwentyFive int default 0, TwentySix int default 0, TwentySeven int default 0, TwentyEight int default 0,"
                             "TwentyNine int default 0, Thirty int default 0, ThirtyOne int default 0)";
        sql_query1->prepare(create_sql);
        if(!sql_query1->exec())
        {
            qDebug() << "Error: Failed to create table."<< sql_query1->lastError();
        }
        else
        {
            qDebug() << "Table created";
        }
        QString insert_sql = "insert into database1 values (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
        sql_query1->prepare(insert_sql);
        sql_query1->addBindValue("Phone Bill");
        for (int i = 0; i < 33; i++)
            sql_query1->addBindValue(0);

        if(!sql_query1->exec())
        {
            qDebug() << sql_query1->lastError();
        }
        else
        {
            qDebug() << "inserted!";
        }
        if(!sql_query1->exec("INSERT INTO database1 VALUES(\"Utility Bill\", 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)"))
        {
            qDebug() << sql_query1->lastError();
        }
        else
        {
            sql_query1->exec("INSERT INTO database1 VALUES(\"Rent/Leas/Morgage\", 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
            sql_query1->exec("INSERT INTO database1 VALUES(\"Car Lease/Loan\", 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
            qDebug() << "inserted succeed!";
        }
        sql_query1->exec("INSERT INTO database1 VALUES(\"Credit Expenses\", 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
        sql_query1->exec("INSERT INTO database1 VALUES(\"Savings\", 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");

    }
}

void sql::connectsql2()
{
    QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection2"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection2");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("Database.db");
        database.setUserName("root");
        database.setPassword("123456");
    }
    //open database
    if (!database.open())
    {
        qDebug()<<"Errror: Failed to connect to database."<< database.lastError();
    }
    else
    {
        sql_query2 = new QSqlQuery();
        //sql_query;
        QString create_sql = "create table database2(Gategory varchar(20) primary key, Amount int, Remaining int,"
                             "January int default 0, February int default 0, March int default 0, April int default 0, "
                             "May int default 0, June int default 0, July int default 0, August int default 0"
                             ", September int default 0, October int default 0, November int default 0, December int default 0)";

        sql_query2->prepare(create_sql);
        if(!sql_query2->exec())
        {
            qDebug() << "Error: Failed to create table."<< sql_query2->lastError();
        }
        else
        {
            qDebug() << "Table created";
        }
        QString insert_sql = "insert into database2 values (?, ?, ?,?,?,?,?,?,?,?,?,?,?,?,?)";
        sql_query2->prepare(insert_sql);
        sql_query2->addBindValue("Life Insurance");
        sql_query2->addBindValue(0);sql_query2->addBindValue(0);sql_query2->addBindValue(0);sql_query2->addBindValue(0);
        sql_query2->addBindValue(0);sql_query2->addBindValue(0);sql_query2->addBindValue(0);sql_query2->addBindValue(0);
        sql_query2->addBindValue(0);sql_query2->addBindValue(0);sql_query2->addBindValue(0);sql_query2->addBindValue(0);
        sql_query2->addBindValue(0);sql_query2->addBindValue(0);
        if(!sql_query2->exec())
        {
            qDebug() << sql_query2->lastError();
        }
        else
        {
            qDebug() << "inserted!";
        }
        if(!sql_query2->exec("INSERT INTO database2 VALUES(\"Car Insurnce\", 0, 0,0,0,0,0,0,0,0,0,0,0,0,0)"))
        {
            qDebug() << sql_query2->lastError();
        }
        else
        {
            sql_query2->exec("INSERT INTO database2 VALUES(\"Health Insurance\", 0, 0,0,0,0,0,0,0,0,0,0,0,0,0)");
            sql_query2->exec("INSERT INTO database2 VALUES(\"Gifts\", 0, 0,0,0,0,0,0,0,0,0,0,0,0,0)");
            qDebug() << "inserted succeed!";
        }
        sql_query2->exec("INSERT INTO database2 VALUES(\"Clothing\", 0, 0,0,0,0,0,0,0,0,0,0,0,0,0)");
        sql_query2->exec("INSERT INTO database2 VALUES(\"Home/Renovations\", 0, 0,0,0,0,0,0,0,0,0,0,0,0,0)");
        sql_query2->exec("INSERT INTO database2 VALUES(\"Student Loans\", 0, 0,0,0,0,0,0,0,0,0,0,0,0,0)");
        sql_query2->exec("INSERT INTO database2 VALUES(\"Tuition Fee\", 0, 0,0,0,0,0,0,0,0,0,0,0,0,0)");

    }
}
