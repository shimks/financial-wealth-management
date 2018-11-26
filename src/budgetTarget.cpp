#include "budgettarget.h"
#include "ui_budgettarget.h"
#include "addexpense.h"
#include "adddialog.h"
#include "addexpensem.h"
#include "addexpensey.h"
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


using namespace std;

QT_CHARTS_USE_NAMESPACE

// BudgetTarget constructor
BudgetTarget::BudgetTarget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BudgetTarget)
{
    // set up ui
    ui->setupUi(this);

    // create two tree widgets to hold budget item information
    ui->treeWidget->setColumnCount(3);
    ui->treeWidget_2->setColumnCount(3);

    // create arrays of the expenses for each day and month
    int dailyExpenses[] = {monExpenses, tuesExpenses, wedExpenses, thursExpenses, friExpenses, satExpenses, sunExpenses};
    int monthlyExpenses[] = {janExp, febExp, marExp, aprExp, mayExp, junExp, julExp, augExp, septExp, octExp, novExp, decExp};
    int weeklyExpenses[] = {week1, week2, week3, week4, extraDays};

    // find the maximum daily expense value and use this to change the range of the weekly line/bar chart
    int maxWeeklyVal = monExpenses;
    for (int i = 1; i < 7; i++) {
        if (dailyExpenses[i] > dailyExpenses[i-1])
            maxWeeklyVal = dailyExpenses[i];
    }

    // find the maximum monthly expense value and use this to change the range of the yearly line/bar chart
    int maxYearlyVal = janExp;
    for (int i = 1; i < 12; i++) {
        if (monthlyExpenses[i] > monthlyExpenses[i-1])
            maxYearlyVal = monthlyExpenses[i];
    }

    // find the maxium weekly expense value and use this to change the range of the monthly line/bar chart
    int maxMonthlyVal = week1;
    for (int i = 1; i < 5; i++) {
        if (weeklyExpenses[i] > weeklyExpenses[i-1])
            maxMonthlyVal = weeklyExpenses[i];
    }

    // add days of the week and months of the year to QBarSet
    *daysOfWeek << monExpenses << tuesExpenses << wedExpenses << thursExpenses << friExpenses << satExpenses << sunExpenses;
    *monthsOfYear << janExp << febExp << marExp << aprExp << mayExp << junExp << julExp << augExp << septExp << octExp << novExp << decExp;
    *weeksOfMonth << week1 << week2 << week3 << week4 << extraDays;

    /* Creating the weekly line and bar plot */

    // add the days of the week and months of the year to their QBarSeries
    barseries->append(daysOfWeek);

    // add points for the weekly line chart to the plot
    lineseries->setName("Weekly Expense Trend");
    lineseries->append(QPoint(0, monExpenses));
    lineseries->append(QPoint(1, tuesExpenses));
    lineseries->append(QPoint(2, wedExpenses));
    lineseries->append(QPoint(3, thursExpenses));
    lineseries->append(QPoint(4, friExpenses));
    lineseries->append(QPoint(5, satExpenses));
    lineseries->append(QPoint(6, sunExpenses));

    // create chart, add data and a title for weekly line plot
    QChart *chart = new QChart();
    chart->addSeries(barseries);
    chart->addSeries(lineseries);
    chart->setTitle("Line and Bar Chart of Weekly Expenses");

    // add categories for each of the bars for the weekly chart
    QStringList categories;
    categories << "Mon" << "Tues" << "Wed" << "Thurs" << "Fri" << "Sat" << "Sun";

    // create and format x-axis with categories for weekly chart
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->setAxisX(axisX, lineseries);
    chart->setAxisX(axisX, barseries);
    axisX->setRange(QString("Mon"), QString("Sat"));
    axisX->setTitleText("Day of the Week");

    // create and format y-axis for weekly chart
    QValueAxis *axisY = new QValueAxis();
    chart->setAxisY(axisY, lineseries);
    chart->setAxisY(axisY, barseries);
    axisY->setRange(0, maxWeeklyVal + 100);
    axisY->setTitleText("Expensed Amount ($)");

    // add a legend to the weekly plot
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // create a widget view of the weekly chart and resize accoringly
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedSize(10000, 7000);

    // add the weekly chart to the layout
    ui->verticalLayout_5->addWidget(chartView);

    /* Creating the yearly line and bar plot */

    // add the days of the week and months of the year to their QBarSeries
    yearlyBarSeries->append(monthsOfYear);

    // add points for the monthly line chart to plot
    yearlyLineSeries->setName("Yearly Expense Trend");
    yearlyLineSeries->append(QPoint(0, janExp));
    yearlyLineSeries->append(QPoint(1, febExp));
    yearlyLineSeries->append(QPoint(2, marExp));
    yearlyLineSeries->append(QPoint(3, aprExp));
    yearlyLineSeries->append(QPoint(4, mayExp));
    yearlyLineSeries->append(QPoint(5, junExp));
    yearlyLineSeries->append(QPoint(6, julExp));
    yearlyLineSeries->append(QPoint(7, augExp));
    yearlyLineSeries->append(QPoint(8, septExp));
    yearlyLineSeries->append(QPoint(9, octExp));
    yearlyLineSeries->append(QPoint(10, novExp));
    yearlyLineSeries->append(QPoint(11, decExp));

    // create chart, add data and a title for the yearly line plot
    QChart *yearlyChart = new QChart();
    yearlyChart->addSeries(yearlyBarSeries);
    yearlyChart->addSeries(yearlyLineSeries);
    yearlyChart->setTitle("Line and Bar Chart of Yearly Expenses");

    // add categories for each of the bars for the yearly chart
    QStringList yearlyCategories;
    yearlyCategories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sept" << "Oct" << "Sept" << "Nov" << "Dec";

    // create and format x-axis with categories for yearly chart
    QBarCategoryAxis *yearlyAxisX = new QBarCategoryAxis();
    yearlyAxisX->append(yearlyCategories);
    yearlyChart->setAxisX(yearlyAxisX, yearlyLineSeries);
    yearlyChart->setAxisX(yearlyAxisX, yearlyBarSeries);
    yearlyAxisX->setRange(QString("Jan"), QString("Dec"));
    yearlyAxisX->setTitleText("Month of the Year");

    // create and format y-axis for yearly chart
    QValueAxis *yearlyAxisY = new QValueAxis();
    yearlyChart->setAxisY(yearlyAxisY, yearlyLineSeries);
    yearlyChart->setAxisY(yearlyAxisY, yearlyBarSeries);
    yearlyAxisY->setRange(0, maxYearlyVal + 100);
    yearlyAxisY->setTitleText("Exepensed Amount ($)");

    // add a legend to the yearly plot
    yearlyChart->legend()->setVisible(true);
    yearlyChart->legend()->setAlignment(Qt::AlignBottom);

    // create a widget view of the yearly chart and resize accordingly
    QChartView *yearlyChartView = new QChartView(yearlyChart);
    yearlyChartView->setRenderHint(QPainter::Antialiasing);
    yearlyChartView->setFixedSize(10000, 7000);

    // add the yearly chart to the layout
    ui->verticalLayout_10->addWidget(yearlyChartView);

    /* Creating the monthly line and bar plot */

    // add the days of the week and months of the year to their QBarSeries
    monthlyBarSeries->append(weeksOfMonth);

    // add points for the monthly line chart to plot
    monthlyLineSeries->setName("Yearly Expense Trend");
    monthlyLineSeries->append(QPoint(0, week1));
    monthlyLineSeries->append(QPoint(1, week2));
    monthlyLineSeries->append(QPoint(2, week3));
    monthlyLineSeries->append(QPoint(3, week4));
    monthlyLineSeries->append(QPoint(4, extraDays));

    // create chart, add data and a title for the yearly line plot
    QChart *monthlyChart = new QChart();
    monthlyChart->addSeries(monthlyBarSeries);
    monthlyChart->addSeries(monthlyLineSeries);
    monthlyChart->setTitle("Line and Bar Chart of Yearly Expenses");

    // add categories for each of the bars for the yearly chart
    QStringList monthlyCategories;
    monthlyCategories << "Wk 1" << "Wk 2" << "Wk 3" << "Wk 4" << "Wk 5";

    // create and format x-axis with categories for yearly chart
    QBarCategoryAxis *monthlyAxisX = new QBarCategoryAxis();
    monthlyAxisX->append(monthlyCategories);
    monthlyChart->setAxisX(monthlyAxisX, monthlyLineSeries);
    monthlyChart->setAxisX(monthlyAxisX, monthlyBarSeries);
    monthlyAxisX->setRange(QString("Wk 1"), QString("Wk 5"));
    monthlyAxisX->setTitleText("Week of the Month");

    // create and format y-axis for yearly chart
    QValueAxis *monthlyAxisY = new QValueAxis();
    monthlyChart->setAxisY(monthlyAxisY, monthlyLineSeries);
    monthlyChart->setAxisY(monthlyAxisY, monthlyBarSeries);
    monthlyAxisY->setRange(0, maxMonthlyVal + 100);
    monthlyAxisY->setTitleText("Exepensed Amount ($)");

    // add a legend to the yearly plot
    monthlyChart->legend()->setVisible(true);
    monthlyChart->legend()->setAlignment(Qt::AlignBottom);

    // create a widget view of the yearly chart and resize accordingly
    QChartView *monthlyChartView = new QChartView(monthlyChart);
    monthlyChartView->setRenderHint(QPainter::Antialiasing);
    monthlyChartView->setFixedSize(10000, 7000);

    // add the yearly chart to the layout
    ui->verticalLayout_6->addWidget(monthlyChartView);

    // add budgeting categories for weekly timeline
    AddRoot("Groceries","0");
    AddRoot("Hygiene Products","0");
    AddRoot("Transportation","0");
    AddRoot("Personal","0");
    AddRoot("Food","0");
    AddRoot("School Supplies","0");

    // add budgeting categories for a monthly timeline
    AddRoot_2("Phone Bill","0");
    AddRoot_2("Utility Bill","0");
    AddRoot_2("Rent/Leas/Morgage","0");
    AddRoot_2("Car Lease/Loan","0");
    AddRoot_2("Credit Expenses","0");
    AddRoot_2("Savings","0");

    // add budgeting categories for a yearly timeline
    AddRoot_3("Life Insurance","0");
    AddRoot_3("Car Insurnce","0");
    AddRoot_3("Health Insurance","0");
    AddRoot_3("Gifts","0");
    AddRoot_3("Clothing","0");
    AddRoot_3("Home/Renovations","0");
    AddRoot_3("Student Loans","0");
    AddRoot_3("Tuition Fee","0");

    // append weekly categories
    cateList << "Groceries";
    cateList << "Hyginen Products";
    cateList << "Transportation";
    cateList << "Personal";
    cateList << "Food";
    cateList << "School Supplies";

    // append monthly categories
    cateListM << "Phone Bill";
    cateListM << "Utility Bill";
    cateListM << "Rent/Lease/Loan";
    cateListM << "Car Lease/Loan";
    cateListM << "Credit Expenses";
    cateListM << "Savings";

    // append days of the week
    dayList << "Monday";
    dayList << "Tuesday";
    dayList << "Wednesday";
    dayList << "Thursday";
    dayList << "Friday";
    dayList << "Saturday";
    dayList << "Sunday";

    // append yearly categories
    cateListY << "Life Insurance";
    cateListY << "Car Insurance";
    cateListY << "Health Insurance";
    cateListY << "Gifts";
    cateListY << "Clothing";
    cateListY << "Home/Renovations";
    cateListY << "Student Loans";
    cateListY << "Tuition Fee";

    // append months of the year
    dayListY << "January";
    dayListY << "February";
    dayListY << "March";
    dayListY << "April";
    dayListY << "May";
    dayListY << "June";
    dayListY << "July";
    dayListY << "August";
    dayListY << "September";
    dayListY << "October";
    dayListY << "November";
    dayListY << "December";

    //sql database
    int sum=0;

    s.connectsql();
    for (int i = 3; i < 10; i++) {
        QString str = s.select(s.sql_query, "database", "Groceries", i);
        if (str != "0")
            AddExpense("Groceries",str, s.transition(i-2));
            sum+=str.toInt();
    }

    for (int i = 3; i < 10; i++) {
        QString str = s.select(s.sql_query, "database", "Hygiene Products", i);
        if (str != "0")
            AddExpense("Hygiene Products",str, s.transition(i-2));
    }
    for (int i = 3; i < 10; i++) {
        QString str = s.select(s.sql_query, "database", "Transportation", i);
        if (str != "0")
            AddExpense("Transportation",str, s.transition(i-2));
    }
    for (int i = 3; i < 10; i++) {
        QString str = s.select(s.sql_query, "database", "Personal", i);
        if (str != "0")
            AddExpense("Personal",str, s.transition(i-2));
    }
    for (int i = 3; i < 10; i++) {
        QString str = s.select(s.sql_query, "database", "Food", i);
        if (str != "0")
            AddExpense("Food",str, s.transition(i-2));
    }
    for (int i = 3; i < 10; i++) {
        QString str = s.select(s.sql_query, "database", "School Supplies", i);
        if (str != "0")
            AddExpense("School Supplies",str, s.transition(i-2));
    }
    s.close();

    s.connectsql1();
    for (int i = 3; i < 34; i++) {
        QString str = s.select(s.sql_query1, "database1", "Phone Bill", i);
        if (str != "0")
            AddExpense_2("Phone Bill",str, s.transition1(i-2));
    }
    for (int i = 3; i < 34; i++) {
        QString str = s.select(s.sql_query1, "database1", "Utility Bill", i);
        if (str != "0")
            AddExpense_2("Utility Bill",str, s.transition1(i-2));
    }
    for (int i = 3; i < 34; i++) {
        QString str = s.select(s.sql_query1, "database1", "Rent/Leas/Morgage", i);
        if (str != "0")
            AddExpense_2("Rent/Leas/Morgage",str, s.transition1(i-2));
    }
    for (int i = 3; i < 34; i++) {
        QString str = s.select(s.sql_query1, "database1", "Car Lease/Loan", i);
        if (str != "0")
            AddExpense_2("Car Lease/Loan",str, s.transition1(i-2));
    }
    for (int i = 3; i < 34; i++) {
        QString str = s.select(s.sql_query1, "database1", "Credit Expenses", i);
        if (str != "0")
            AddExpense_2("Credit Expenses",str, s.transition1(i-2));
    }
    for (int i = 3; i < 34; i++) {
        QString str = s.select(s.sql_query1, "database1", "Savings", i);
        if (str != "0")
            AddExpense_2("Savings",str, s.transition1(i-2));
    }
    s.close();

    s.connectsql2();
    for (int i = 3; i < 15; i++) {
        QString str = s.select(s.sql_query2, "database2", "Life Insurance", i);
        if (str != "0")
            AddExpense_3("Life Insurance",str, s.transition2(i-2));
    }
    for (int i = 3; i < 15; i++) {
        QString str = s.select(s.sql_query2, "database2", "Car Insurnce", i);
        if (str != "0")
            AddExpense_3("Car Insurnce",str, s.transition2(i-2));
    }
    for (int i = 3; i < 15; i++) {
        QString str = s.select(s.sql_query2, "database2", "Health Insurance", i);
        if (str != "0")
            AddExpense_3("Health Insurance",str, s.transition2(i-2));
    }
    for (int i = 3; i < 15; i++) {
        QString str = s.select(s.sql_query2, "database2", "Gifts", i);
        if (str != "0")
            AddExpense_3("Gifts",str, s.transition2(i-2));
    }
    for (int i = 3; i < 15; i++) {
        QString str = s.select(s.sql_query2, "database2", "Clothing", i);
        if (str != "0")
            AddExpense_3("Clothing",str, s.transition2(i-2));
    }
    for (int i = 3; i < 15; i++) {
        QString str = s.select(s.sql_query2, "database2", "Home/Renovations", i);
        if (str != "0")
            AddExpense_3("Home/Renovations",str, s.transition2(i-2));
    }
    for (int i = 3; i < 15; i++) {
        QString str = s.select(s.sql_query2, "database2", "Student Loans", i);
        if (str != "0")
            AddExpense_3("Student Loans",str, s.transition2(i-2));
    }
    for (int i = 3; i < 15; i++) {
        QString str = s.select(s.sql_query2, "database2", "Tuition Fee", i);
        if (str != "0")
            AddExpense_3("Tuition Fee",str, s.transition2(i-2));
    }

    s.close();
    s.connectsql();
    lineseries->clear();

    for (int i = 0; i < 6; i++) {
        QString str = s.select(s.sql_query, "database", cateList[i], 3);
        monExpenses=monExpenses+str.toInt();
    }

    for (int i = 0; i < 6; i++) {
        QString str = s.select(s.sql_query, "database", cateList[i], 4);
        tuesExpenses=tuesExpenses+str.toInt();
    }
    for (int i = 0; i < 6; i++) {
        QString str = s.select(s.sql_query, "database", cateList[i], 5);
        wedExpenses=wedExpenses+str.toInt();
    }
    for (int i = 0; i < 6; i++) {
        QString str = s.select(s.sql_query, "database", cateList[i], 6);
        thursExpenses=thursExpenses+str.toInt();
    }
    for (int i = 0; i < 6; i++) {
        QString str = s.select(s.sql_query, "database", cateList[i], 7);
        friExpenses=friExpenses+str.toInt();
    }
    for (int i = 0; i < 6; i++) {
        QString str = s.select(s.sql_query, "database", cateList[i], 8);
        satExpenses=satExpenses+str.toInt();
    }
    for (int i = 0; i < 6; i++) {
        QString str = s.select(s.sql_query, "database", cateList[i], 9);
        sunExpenses=sunExpenses+str.toInt();
    }
    s.close();
    lineseries->append(QPoint(0, monExpenses));
    lineseries->append(QPoint(1, tuesExpenses));
    lineseries->append(QPoint(2, wedExpenses));
    lineseries->append(QPoint(3, thursExpenses));
    lineseries->append(QPoint(4, friExpenses));
    lineseries->append(QPoint(5, satExpenses));
    lineseries->append(QPoint(6, sunExpenses));
    daysOfWeek->replace(0, monExpenses);
    daysOfWeek->replace(1, tuesExpenses);
    daysOfWeek->replace(2, wedExpenses);
    daysOfWeek->replace(3, thursExpenses);
    daysOfWeek->replace(4, friExpenses);
    daysOfWeek->replace(5, satExpenses);
    daysOfWeek->replace(6, sunExpenses);

    s.connectsql2();
    for (int i = 0; i < 8; i++) {
        QString str = s.select(s.sql_query2, "database2", cateListY[i], 3);
        janExp=janExp+str.toInt();
    }

    for (int i = 0; i < 8; i++) {
        QString str = s.select(s.sql_query2, "database2", cateListY[i], 4);
        febExp=febExp+str.toInt();
    }
    for (int i = 0; i < 8; i++) {
        QString str = s.select(s.sql_query2, "database2", cateListY[i], 5);
        marExp=marExp+str.toInt();
    }
    for (int i = 0; i < 8; i++) {
        QString str = s.select(s.sql_query2, "database2", cateListY[i], 6);
        aprExp=aprExp+str.toInt();
    }
    for (int i = 0; i < 8; i++) {
        QString str = s.select(s.sql_query2, "database2", cateListY[i], 7);
        mayExp=mayExp+str.toInt();
    }
    for (int i = 0; i < 8; i++) {
        QString str = s.select(s.sql_query2, "database2", cateListY[i], 8);
        junExp=junExp+str.toInt();
    }
    for (int i = 0; i < 8; i++) {
        QString str = s.select(s.sql_query2, "database2", cateListY[i], 9);
        julExp=julExp+str.toInt();
    }
    for (int i = 0; i < 8; i++) {
        QString str = s.select(s.sql_query2, "database2", cateListY[i], 10);
        augExp=augExp+str.toInt();
    }
    for (int i = 0; i < 8; i++) {
        QString str = s.select(s.sql_query2, "database2", cateListY[i], 11);
        septExp=septExp+str.toInt();
    }
    for (int i = 0; i < 8; i++) {
        QString str = s.select(s.sql_query2, "database2", cateListY[i], 12);
        octExp=octExp+str.toInt();
    }
    for (int i = 0; i < 8; i++) {
        QString str = s.select(s.sql_query2, "database2", cateListY[i], 13);
        novExp=novExp+str.toInt();
    }
    for (int i = 0; i < 8; i++) {
        QString str = s.select(s.sql_query2, "database2", cateListY[i], 14);
        decExp=decExp+str.toInt();
    }
    s.close();
    yearlyLineSeries->clear();
    yearlyLineSeries->append(QPoint(0, janExp));
    yearlyLineSeries->append(QPoint(1, febExp));
    yearlyLineSeries->append(QPoint(2, marExp));
    yearlyLineSeries->append(QPoint(3, aprExp));
    yearlyLineSeries->append(QPoint(4, mayExp));
    yearlyLineSeries->append(QPoint(5, junExp));
    yearlyLineSeries->append(QPoint(6, julExp));
    yearlyLineSeries->append(QPoint(7, augExp));
    yearlyLineSeries->append(QPoint(8, septExp));
    yearlyLineSeries->append(QPoint(9, octExp));
    yearlyLineSeries->append(QPoint(10, novExp));
    yearlyLineSeries->append(QPoint(11, decExp));
    monthsOfYear->replace(0, janExp);
    monthsOfYear->replace(1, febExp);
    monthsOfYear->replace(2, marExp);
    monthsOfYear->replace(3, aprExp);
    monthsOfYear->replace(4, mayExp);
    monthsOfYear->replace(5, junExp);
    monthsOfYear->replace(6, julExp);
    monthsOfYear->replace(7, augExp);
    monthsOfYear->replace(8, septExp);
    monthsOfYear->replace(9, octExp);
    monthsOfYear->replace(10, novExp);
    monthsOfYear->replace(11, decExp);


}

// BudgetTarget destructor
BudgetTarget::~BudgetTarget() {
    delete ui;
}

void BudgetTarget::AddRoot(QString Category, QString Amount) {
    s.connectsql();
    Amount = s.select(s.sql_query, "database", Category,2);
    s.close();

    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,Category);
    s.connectsql();
    QString Amount1 = s.select(s.sql_query, "database", Category,1);
    s.close();
    itm->setText(1,Amount1);
    itm->setText(2,Amount);
    ui->treeWidget->addTopLevelItem(itm);
}

void BudgetTarget::AddRoot_2(QString Category, QString Amount) {
    s.connectsql1();
    Amount = s.select(s.sql_query1, "database1", Category,2);
    s.close();

    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_3);
    itm->setText(0,Category);
    s.connectsql1();
    QString Amount1 = s.select(s.sql_query1, "database1", Category, 1);
    s.close();
    itm->setText(1,Amount1);    itm->setText(2,Amount);
    ui->treeWidget_3->addTopLevelItem(itm);
}

void BudgetTarget::AddRoot_3(QString Category, QString Amount) {
    s.connectsql2();
    Amount = s.select(s.sql_query2, "database2", Category, 2);
    s.close();

    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_6);
    itm->setText(0,Category);
    s.connectsql2();
    QString Amount1 = s.select(s.sql_query2, "database2", Category, 1);
    s.close();
    itm->setText(1,Amount1);    itm->setText(2,Amount);
    ui->treeWidget_7->addTopLevelItem(itm);
}

// adding weekly expenses
void BudgetTarget::AddExpense(QString Category, QString Amount, QString Day) {
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_2);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    itm->setText(2,Day);
    ui->treeWidget_2->addTopLevelItem(itm);
}

// adding monthly expenses
void BudgetTarget::AddExpense_2(QString Category, QString Amount, QString Day) {
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_4);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    itm->setText(2,Day);
    ui->treeWidget_4->addTopLevelItem(itm);
}

// adding yearly expenses
void BudgetTarget::AddExpense_3(QString Category, QString Amount, QString Day) {
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_7);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    itm->setText(2,Day);
    ui->treeWidget_7->addTopLevelItem(itm);
}

QString BudgetTarget::EditRoot(QString Amount) {
    QString before = ui->treeWidget->currentItem()->text(1);
    QString before2 = ui->treeWidget->currentItem()->text(2);
    int diff = Amount.toInt()-before.toInt();
    int update = before2.toInt() + diff;
    ui->treeWidget->currentItem()->setData(1,Qt::DisplayRole,Amount);
    ui->treeWidget->currentItem()->setData(2,Qt::DisplayRole, update);
    return ui->treeWidget->currentItem()->text(0);

}

QString BudgetTarget::EditRoot_2(QString Amount) {
    QString before = ui->treeWidget_3->currentItem()->text(1);
    QString before2 = ui->treeWidget_3->currentItem()->text(2);
    int diff = Amount.toInt()-before.toInt();
    int update = before2.toInt() + diff;
    ui->treeWidget_3->currentItem()->setData(1,Qt::DisplayRole,Amount);
    ui->treeWidget_3->currentItem()->setData(2,Qt::DisplayRole, update);
    return ui->treeWidget_3->currentItem()->text(0);

}

QString BudgetTarget::EditRoot_3(QString Amount) {
    QString before = ui->treeWidget_6->currentItem()->text(1);
    QString before2 = ui->treeWidget_6->currentItem()->text(2);
    int diff = Amount.toInt()-before.toInt();
    int update = before2.toInt() + diff;
    ui->treeWidget_6->currentItem()->setData(1,Qt::DisplayRole,Amount);
    ui->treeWidget_6->currentItem()->setData(2,Qt::DisplayRole, update);

    return ui->treeWidget_6->currentItem()->text(0);

}

void BudgetTarget::on_Edit_clicked() {
    if(ui->treeWidget->currentItem()!=nullptr){
        AddDialog adddialog1;
        adddialog1.setModal(true);
        adddialog1.exec();
        QString amount = adddialog1.returnAmo();
        if(amount!=nullptr){
            QString category = EditRoot(amount);
            s.connectsql();
            s.update(s.sql_query, "database",category, amount);
            s.print(s.sql_query, "database");
            s.close();
        }
    }
}

void BudgetTarget::on_Edit_2_clicked() {
    if(ui->treeWidget_3->currentItem()!=nullptr){
        AddDialog adddialog1;
        adddialog1.setModal(true);
        adddialog1.exec();
        QString amount = adddialog1.returnAmo();
        if(amount!=nullptr){
            QString category = EditRoot_2(amount);
            s.connectsql1();
            s.update(s.sql_query1, "database1",category, amount);
            s.print(s.sql_query1, "database1");
            s.close();
        }
    }

}
void BudgetTarget::on_Edit_4_clicked()
{
    if(ui->treeWidget_6->currentItem()!=nullptr){
        AddDialog adddialog1;
        adddialog1.setModal(true);
        adddialog1.exec();
        QString amount = adddialog1.returnAmo();
        if(amount!=nullptr){
            QString category = EditRoot_3(amount);
            s.connectsql2();
            s.update(s.sql_query2, "database2",category, amount);
            s.print(s.sql_query2, "database2");
            s.close();
        }
    }
}

// weekly
void BudgetTarget::on_AddE_clicked() {
    addExpense addexpense;
    addexpense.setModal(true);
    addexpense.exec();
    QString cate = cateList[addexpense.returnVal()];
    QString amount = addexpense.returnAmo();
    QString day = dayList[addexpense.returnDay()];
    if(amount!=nullptr){
        AddExpense(cate, amount,day);
        QString before = ui->treeWidget->topLevelItem(addexpense.returnVal())->text(2);
        int update = before.toInt()-amount.toInt();
        ui->treeWidget->topLevelItem(addexpense.returnVal())->setData(2,Qt::DisplayRole,update);

        qDebug() << cate << amount << day;
        s.connectsql();
        QString oldValue = s.select(s.sql_query, "database", cate,2);
        QString oldExp = s.select(s.sql_query, "database", cate, dayList.indexOf(day)+3); // column for Monday

        qDebug() << cate << oldValue << oldExp << amount << day << endl;
        s.updateExpenditure(s.sql_query, "database", cate, oldValue, oldExp, amount, day);
        s.close();

    }
    // convert the string to an int and add it to the corresponding date
    int amountInt = amount.toInt();
    int selectedIndex = addexpense.returnDay();
    switch(selectedIndex) {
        case 0 : monExpenses += amountInt;
        break;
        case 1 : tuesExpenses += amountInt;
        break;
        case 2 : wedExpenses += amountInt;
        break;
        case 3 : thursExpenses += amountInt;
        break;
        case 4 : friExpenses += amountInt;
        break;
        case 5 : satExpenses += amountInt;
        break;
        case 6 : sunExpenses += amountInt;
        break;
        default : break;
    }
    lineseries->clear();
    lineseries->append(QPoint(0, monExpenses));
    lineseries->append(QPoint(1, tuesExpenses));
    lineseries->append(QPoint(2, wedExpenses));
    lineseries->append(QPoint(3, thursExpenses));
    lineseries->append(QPoint(4, friExpenses));
    lineseries->append(QPoint(5, satExpenses));
    lineseries->append(QPoint(6, sunExpenses));
    daysOfWeek->replace(0, monExpenses);
    daysOfWeek->replace(1, tuesExpenses);
    daysOfWeek->replace(2, wedExpenses);
    daysOfWeek->replace(3, thursExpenses);
    daysOfWeek->replace(4, friExpenses);
    daysOfWeek->replace(5, satExpenses);
    daysOfWeek->replace(6, sunExpenses);
}

// monthly
void BudgetTarget::on_AddE_2_clicked(){
    addExpenseM addexpense;
    addexpense.setModal(true);
    addexpense.exec();
    QString cate = cateListM[addexpense.returnVal()];
    QString amount = addexpense.returnAmo();
    QString day = addexpense.returnDay();
    if(amount!=nullptr and day!=nullptr){
        AddExpense_2(cate, amount,day);
        QString before = ui->treeWidget_3->topLevelItem(addexpense.returnVal())->text(2);
        int update = before.toInt()-amount.toInt();
        ui->treeWidget_3->topLevelItem(addexpense.returnVal())->setData(2,Qt::DisplayRole,update);
        qDebug() << cate << amount << day;
        s.connectsql1();
        QString oldValue = s.select(s.sql_query1, "database1", cate,2);
        QString oldExp = s.select(s.sql_query1, "database1", cate, dayList.indexOf(day)+3);

        s.updateExpenditure(s.sql_query1, "database1", cate, oldValue, oldExp, amount, day);
        s.close();
    }
    // convert the string to an int and add it to the corresponding date
    int amountInt = amount.toInt();
    int dayInt = day.toInt();
    int index = dayInt / 7;
    switch(index) {
        case 0 : week1 += amountInt;
        break;
        case 1 : week2 += amountInt;
        break;
        case 2 : week3 += amountInt;
        break;
        case 3 : week4 += amountInt;
        break;
        case 4 : extraDays += amountInt;
        break;
        default : break;
    }
    monthlyLineSeries->clear();
    monthlyLineSeries->append(QPoint(0, week1));
    monthlyLineSeries->append(QPoint(1, week2));
    monthlyLineSeries->append(QPoint(2, week3));
    monthlyLineSeries->append(QPoint(3, week4));
    monthlyLineSeries->append(QPoint(4, extraDays));
    weeksOfMonth->replace(0, week1);
    weeksOfMonth->replace(1, week2);
    weeksOfMonth->replace(2, week3);
    weeksOfMonth->replace(3, week4);
    weeksOfMonth->replace(4, extraDays);
}

// yearly
void BudgetTarget::on_AddE_3_clicked(){
    addExpenseY addexpense;
    addexpense.setModal(true);
    addexpense.exec();
    QString cate = cateListY[addexpense.returnVal()];
    QString amount = addexpense.returnAmo();
    QString day = dayListY[addexpense.returnDay()];
    if(amount!=nullptr){
        AddExpense_3(cate, amount,day);
        QString before = ui->treeWidget_6->topLevelItem(addexpense.returnVal())->text(2);
        int update = before.toInt()-amount.toInt();
        ui->treeWidget_6->topLevelItem(addexpense.returnVal())->setData(2,Qt::DisplayRole,update);

        qDebug() << cate << amount << day;
        s.connectsql2();
        QString oldValue = s.select(s.sql_query2, "database2", cate,2);
        QString oldExp = s.select(s.sql_query2, "database2", cate, addexpense.returnDay()+3);

        s.updateExpenditure(s.sql_query2, "database2", cate, oldValue,oldExp, amount, day);
        s.close();
    }
    // convert the string to an int and add it to the corresponding date
    int amountInt = amount.toInt();
    int selectedIndex = addexpense.returnDay();
    switch(selectedIndex) {
        case 0 : janExp += amountInt;
        break;
        case 1 : febExp += amountInt;
        break;
        case 2 : marExp += amountInt;
        break;
        case 3 : aprExp += amountInt;
        break;
        case 4 : mayExp += amountInt;
        break;
        case 5 : junExp += amountInt;
        break;
        case 6 : julExp += amountInt;
        break;
        case 7 : augExp += amountInt;
        break;
        case 8 : septExp += amountInt;
        break;
        case 9 : octExp += amountInt;
        break;
        case 10 : novExp += amountInt;
        break;
        case 11 : decExp += amountInt;
        break;
        default : break;
    }
    yearlyLineSeries->clear();
    yearlyLineSeries->append(QPoint(0, janExp));
    yearlyLineSeries->append(QPoint(1, febExp));
    yearlyLineSeries->append(QPoint(2, marExp));
    yearlyLineSeries->append(QPoint(3, aprExp));
    yearlyLineSeries->append(QPoint(4, mayExp));
    yearlyLineSeries->append(QPoint(5, junExp));
    yearlyLineSeries->append(QPoint(6, julExp));
    yearlyLineSeries->append(QPoint(7, augExp));
    yearlyLineSeries->append(QPoint(8, septExp));
    yearlyLineSeries->append(QPoint(9, octExp));
    yearlyLineSeries->append(QPoint(10, novExp));
    yearlyLineSeries->append(QPoint(11, decExp));
    monthsOfYear->replace(0, janExp);
    monthsOfYear->replace(1, febExp);
    monthsOfYear->replace(2, marExp);
    monthsOfYear->replace(3, aprExp);
    monthsOfYear->replace(4, mayExp);
    monthsOfYear->replace(5, junExp);
    monthsOfYear->replace(6, julExp);
    monthsOfYear->replace(7, augExp);
    monthsOfYear->replace(8, septExp);
    monthsOfYear->replace(9, octExp);
    monthsOfYear->replace(10, novExp);
    monthsOfYear->replace(11, decExp);
}
void BudgetTarget::on_DeleteE_clicked(){
    if(ui->treeWidget_2->currentItem()!=nullptr){

        QModelIndex index = ui->treeWidget_2->currentIndex();
        QString category = ui->treeWidget_2->currentItem()->text(0);
        int cateIndex = cateList.indexOf(category);
        QString before = ui->treeWidget->topLevelItem(cateIndex)->text(2);
        QString add = ui->treeWidget_2->currentItem()->text(1);
        QString day = ui->treeWidget_2->currentItem()->text(2);
        int update = before.toInt()+add.toInt();
        ui->treeWidget->topLevelItem(cateIndex)->setData(2,Qt::DisplayRole,update);
        ui->treeWidget_2->takeTopLevelItem(index.row());

        s.connectsql();
        QString oldValue = s.select(s.sql_query, "database", category,2);
        QString amount = (QString)(add.toInt()*-1);
        QString oldExp = s.select(s.sql_query, "database", category, dayList.indexOf(day)+3);
        s.updateExpenditureD(s.sql_query, "database", category, oldValue, oldExp,add, day);

        s.close();
    }
}

void BudgetTarget::on_DeleteE_2_clicked(){
    if(ui->treeWidget_4->currentItem()!=nullptr){

        QModelIndex index = ui->treeWidget_4->currentIndex();
        QString category = ui->treeWidget_4->currentItem()->text(0);
        int cateIndex = cateListM.indexOf(category);
        QString before = ui->treeWidget_3->topLevelItem(cateIndex)->text(2);
        QString add = ui->treeWidget_4->currentItem()->text(1);
        int update = before.toInt()+add.toInt();
        QString day = ui->treeWidget_4->currentItem()->text(2);
        ui->treeWidget_3->topLevelItem(cateIndex)->setData(2,Qt::DisplayRole,update);
        ui->treeWidget_4->takeTopLevelItem(index.row());
        s.connectsql1();
        QString oldValue = s.select(s.sql_query1, "database1", category,2);
        QString amount = (QString)(add.toInt()*-1);
        QString oldExp = s.select(s.sql_query1, "database1", category, day.toInt()+3);
        s.updateExpenditureD(s.sql_query1, "database1", category, oldValue, oldExp,add, day);

        s.close();

    }
}
void BudgetTarget::on_DeleteE_3_clicked(){
    if(ui->treeWidget_7->currentItem()!=nullptr){
        QModelIndex index = ui->treeWidget_7->currentIndex();
        QString category = ui->treeWidget_7->currentItem()->text(0);
        int cateIndex = cateListY.indexOf(category);
        QString before = ui->treeWidget_6->topLevelItem(cateIndex)->text(2);
        QString add = ui->treeWidget_7->currentItem()->text(1);
        QString month = ui->treeWidget_7->currentItem()->text(2);

        int update = before.toInt()+add.toInt();
        ui->treeWidget_6->topLevelItem(cateIndex)->setData(2,Qt::DisplayRole,update);
        ui->treeWidget_7->takeTopLevelItem(index.row());

        s.connectsql2();
        QString oldValue = s.select(s.sql_query2, "database2", category,2);
        QString amount = (QString)(add.toInt()*-1);
        QString oldExp = s.select(s.sql_query2, "database2", category, dayListY.indexOf(month)+3);
        s.updateExpenditureD(s.sql_query2, "database2", category, oldValue, oldExp,add, month);

        s.close();
    }
}
