#ifndef SQL_H
#define SQL_H

#include <QtDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class sql
{
public:
    sql();
    void close();
    void connectsql();
    void connectsql1();
    void connectsql2();
    void update(QSqlQuery*, QString , QString, QString);
    void updateExpenditure(QSqlQuery*, QString, QString , QString , QString, QString, QString);
    void updateExpenditureD(QSqlQuery*, QString, QString , QString, QString, QString, QString);

    QString select(QSqlQuery*, QString, QString , int);
    void print(QSqlQuery*, QString db);

    QString transition(int i){switch (i){
        case 1:return "Monday";
        case 2:return "Tuesday";
        case 3:return "Wednesday";
        case 4:return "Thursday";
        case 5:return "Friday";
        case 6:return "Saturday";
        case 7:return "Sunday";
        default:return "0";
        }}
    QString transition1(int i){switch (i){
        case 1:return "One";case 2:return "Tow";case 3:return "Three";case 4:return "Four";
        case 5:return "Five";case 6:return "Six";case 7:return "Seven"; case 8:return "Twelve";
        case 9:return "Ninety";case 10:return "Ten";case 11:return "Eleven";case 12:return "Thursday";
        case 13:return "Thirteen";case 14:return "Fourteen";case 15:return "Fifteen"; case 16:return "Sixteen";
        case 17:return "Seventeen";case 18:return "Eighteen";case 19:return "Nineteen";case 20:return "Twenty";
        case 21:return "TwentyOne";case 22:return "TwentyTwo";case 23:return "TwentyThree"; case 24:return "TwentyFour";
        case 25:return "TwentyFive";case 26:return "TwentySix";case 27:return "TwentySeven";case 28:return "TwentyEight";
        case 29:return "TwentyNine";case 30:return "Thirty";case 31:return "ThirtyOne";
        default:return "0";
        }}
    QString transition2(int i){switch (i){
        case 1:return "January";
        case 2:return "February";
        case 3:return "March";
        case 4:return "April";
        case 5:return "May";
        case 6:return "June";
        case 7:return "July";
        case 8:return "August";
        case 9:return "September";
        case 10:return "October";
        case 11:return "November";
        case 12:return "December";
        default:return "0";
        }}

    QSqlQuery* sql_query;
    QSqlQuery* sql_query1;
    QSqlQuery* sql_query2;

    //QString select1(QSqlQuery *sql_query, QString db, QString gategory);
private:

    QSqlDatabase database;

};

#endif // SQL_H
