#ifndef BUDGETTARGET_H
#define BUDGETTARGET_H

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
    void EditRoot(QString Amount);
    void EditRoot_2(QString Amount);
    void EditRoot_3(QString Amount);

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

    // initialize QLineSeries, QBarSet, and QBarSeries for weekly line/bar plot
    QLineSeries *lineseries = new QLineSeries();
    QBarSet *daysOfWeek = new QBarSet("Daily Expense Value");
    QBarSeries *barseries = new QBarSeries();

    // initialize QLineSeries, QBarSet, QBarSeries for yearly line/bar plot
    QLineSeries *yearlyLineSeries = new QLineSeries();
    QBarSet *monthsOfYear = new QBarSet("Monthly Expense Value");
    QBarSeries *yearlyBarSeires = new QBarSeries();
};

#endif // BUDGETTARGET_H
