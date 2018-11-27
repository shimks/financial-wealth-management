#ifndef BUDGETTARGET_H
#define BUDGETTARGET_H

// external dependencies
#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QtWidgets>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <QtCharts>
#include <algorithm>
#include "sql.h"



namespace Ui {
class BudgetTarget;
}

class BudgetTarget : public QWidget
{
    Q_OBJECT

public:
    // constructor and destructor definitions for BudgetTarget
    explicit BudgetTarget(QWidget *parent = nullptr);
    ~BudgetTarget();

    // weekly, monthly, yearly add root functions
    void AddRoot(QString Category, QString Amount);
    void AddRoot_2(QString Category, QString Amount);
    void AddRoot_3(QString Category, QString Amount);

    // weekly, monthly, yearly edit root functions

    QString EditRoot(QString Amount);
    QString EditRoot_2(QString Amount);
    QString EditRoot_3(QString Amount);
    /*
    void EditRoot(QString Amount);
    void EditRoot_2(QString Amount);
    void EditRoot_3(QString Amount);*/

    // weekly, monthly, yearly add expense functions
    void AddExpense(QString Category, QString Amount, QString Day);
    void AddExpense_2(QString Category, QString Amount, QString Day);
    void AddExpense_3(QString Category, QString Amount, QString Day);


private slots:
    // slot functions for edit buttons


    void on_Edit_clicked();
    void on_Edit_2_clicked();
    void on_Edit_4_clicked();

    // slot functons for logging expenses
    void on_AddE_clicked();
    void on_AddE_2_clicked();
    void on_AddE_3_clicked();

    // slot function for deleting expenses
    void on_DeleteE_clicked();
    void on_DeleteE_2_clicked();
    void on_DeleteE_3_clicked();

private:
    // initializing ui and variables for QTreeWidgets
    Ui::BudgetTarget *ui;
    QStringList cateList;
    QStringList dayList;
    QStringList cateListM;
    QStringList cateListY;
    QStringList dayListY;

    // initialize expenses for each day of the week
    int monExpenses = 0;
    int tuesExpenses = 0;
    int wedExpenses = 0;
    int thursExpenses = 0;
    int friExpenses = 0;
    int satExpenses = 0;
    int sunExpenses = 0;

    // initialize expenses for each month of the year
    int janExp = 0;
    int febExp = 0;
    int marExp = 0;
    int aprExp = 0;
    int mayExp = 0;
    int junExp = 0;
    int julExp = 0;
    int augExp = 0;
    int septExp = 0;
    int octExp = 0;
    int novExp = 0;
    int decExp = 0;

    // initialize expenses for each week of the month
    int week1 = 0;
    int week2 = 0;
    int week3 = 0;
    int week4 = 0;
    int extraDays = 0;

    // initialize the maximum expense category values for weekly, monthly, and yearly timelines
    int maxWeeklyVal;
    int maxMonthlyVal;
    int maxYeatlyVal;

    // initialize QLineSeries, QBarSet, and QBarSeries for weekly line/bar plot
    QLineSeries *lineseries = new QLineSeries();
    QBarSet *daysOfWeek = new QBarSet("Daily Expense Value");
    QBarSeries *barseries = new QBarSeries();

    // initialize QLineSeries, QBarSet, QBarSeries for yearly line/bar plot
    QLineSeries *yearlyLineSeries = new QLineSeries();
    QBarSet *monthsOfYear = new QBarSet("Monthly Expense Value");
    QBarSeries *yearlyBarSeries = new QBarSeries();

    // initialize QLineSeries, QBarSet, QBarSeries for monthly line/bar plot
    QLineSeries *monthlyLineSeries = new QLineSeries();
    QBarSet *weeksOfMonth = new QBarSet("WeeklyExpenseValue");
    QBarSeries *monthlyBarSeries = new QBarSeries();

    //sql database
    sql s;
    sql s1;
    sql s2;

    // initialize the axes for each of the charts
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    QValueAxis *monthlyAxisX = new QValueAxis();
    QValueAxis *monthlyAxisY = new QValueAxis();
    QValueAxis *yearlyAxisX = new QValueAxis();
    QValueAxis *yearlyAxisY = new QValueAxis();

    // initialize variables for the weekly pie chart
    QPieSeries *weeklySeries = new QPieSeries();
    QPieSeries *monSeries = new QPieSeries();
    QPieSeries *tuesSeries = new QPieSeries();
    QPieSeries *wedSeries = new QPieSeries();
    QPieSeries *thursSeries = new QPieSeries();
    QPieSeries *friSeries = new QPieSeries();
    QPieSeries *satSeries = new QPieSeries();
    QPieSeries *sunSeries = new QPieSeries();

    // initialize variables for the monthly pie chart
    QPieSeries *monthlySeries = new QPieSeries();
    QPieSeries *week1Series = new QPieSeries();
    QPieSeries *week2Series = new QPieSeries();
    QPieSeries *week3Series = new QPieSeries();
    QPieSeries *week4Series = new QPieSeries();
    QPieSeries *week5Series = new QPieSeries();

    // initialize variables for the yearly pie chart
    QPieSeries *yearlySeries = new QPieSeries();
    QPieSeries *janSeries = new QPieSeries();
    QPieSeries *febSeries = new QPieSeries();
    QPieSeries *marSeries = new QPieSeries();
    QPieSeries *aprSeries = new QPieSeries();
    QPieSeries *maySeries = new QPieSeries();
    QPieSeries *junSeries = new QPieSeries();
    QPieSeries *julSeries = new QPieSeries();
    QPieSeries *augSeries = new QPieSeries();
    QPieSeries *septSeries = new QPieSeries();
    QPieSeries *octSeries = new QPieSeries();
    QPieSeries *novSeries = new QPieSeries();
    QPieSeries *decSeries = new QPieSeries();

    // define category expense vals for weekly
    int monGrocery = 0;
    int tuesGrocery = 0;
    int wedGrocery = 0;
    int thursGrocery = 0;
    int friGrocery = 0;
    int satGrocery = 0;
    int sunGrocery = 0;
    int monHyg = 0;
    int tuesHyg = 0;
    int wedHyg = 0;
    int thursHyg = 0;
    int friHyg = 0;
    int satHyg = 0;
    int sunHyg = 0;
    int monTrans = 0;
    int tuesTrans = 0;
    int wedTrans = 0;
    int thursTrans = 0;
    int friTrans = 0;
    int satTrans = 0;
    int sunTrans = 0;
};

#endif // BUDGETTARGET_H
