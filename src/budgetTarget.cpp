// internal dependencies
#include "budgettarget.h"
#include "ui_budgettarget.h"
#include "addexpense.h"
#include "adddialog.h"
#include "addexpensem.h"
#include "addexpensey.h"
#include "drilldownchart.h"
#include "drilldownslice.h"

// external dependencies
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

/////////////////////////////// MAKING AND FORMATING LINE CHARTS /////////////////////////////////

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
    axisY->setRange(0, 100);
    axisY->setTitleText("Expensed Amount ($)");

    // add a legend to the weekly plot
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // create a widget view of the weekly chart and resize accoringly
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedSize(8000, 6000);

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
    yearlyAxisY->setRange(0, 200000);
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

    // create chart, add data and a title for the monthly line plot
    QChart *monthlyChart = new QChart();
    monthlyChart->addSeries(monthlyBarSeries);
    monthlyChart->addSeries(monthlyLineSeries);
    monthlyChart->setTitle("Line and Bar Chart of Monthly Expenses");

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
    monthlyAxisY->setRange(0, 5000);
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

/////////////////////////////// MAKING AND ADDDING PIE CHARTS /////////////////////////////////

    /* Creating the pi chart for the weekly expenses */

    DrilldownChart *weeklyPieChart = new DrilldownChart();
    weeklyPieChart->setTheme(QChart::ChartThemeLight);
    weeklyPieChart->setAnimationOptions(QChart::AllAnimations);
    weeklyPieChart->legend()->setVisible(true);
    weeklyPieChart->legend()->setAlignment(Qt::AlignRight);

    weeklySeries->setName("Breakdown of Weekly Expenses by Day");

    const QStringList weeklyCategories = {
        "Groceries", "Hygiene Products", "Transportation", "Personal", "Food", "School Supplies"
    };
    const QStringList days = {
        "Mon", "Tues", "Wed", "Thur", "Fri", "Sat", "Sun"
    };

    // make a QPieSeries for each day
    monSeries->setName("Breakdown of Daily Expenses: Monday");
    tuesSeries->setName("Breakdown of Daily Expenses: Tuesday");
    wedSeries->setName("Breakdown of Daily Expenses: Wednesday");
    thursSeries->setName("Breakdown of Daily Expenses: Thursday");
    friSeries->setName("Breakdown of Daily Expenses: Friday");
    satSeries->setName("Breakdown of Daily Expenses: Saturday");
    sunSeries->setName("Breakdown of Daily Expenses: Sunday");

    // make a QPieSeries for each category within the day
    *monSeries << new DrilldownSlice(1, "Groceries", weeklySeries);
    *monSeries << new DrilldownSlice(0, "Hygiene Products", weeklySeries);
    *monSeries << new DrilldownSlice(0, "Transportation", weeklySeries);
    *monSeries << new DrilldownSlice(0, "Personal", weeklySeries);
    *monSeries << new DrilldownSlice(0, "Food", weeklySeries);
    *monSeries << new DrilldownSlice(0, "School Supplies", weeklySeries);

    *tuesSeries << new DrilldownSlice(1, "Groceries", weeklySeries);
    *tuesSeries << new DrilldownSlice(0, "Hygiene Products", weeklySeries);
    *tuesSeries << new DrilldownSlice(0, "Transportation", weeklySeries);
    *tuesSeries << new DrilldownSlice(0, "Personal", weeklySeries);
    *tuesSeries << new DrilldownSlice(0, "Food", weeklySeries);
    *tuesSeries << new DrilldownSlice(0, "School Supplies", weeklySeries);

    *wedSeries << new DrilldownSlice(1, "Groceries", weeklySeries);
    *wedSeries << new DrilldownSlice(0, "Hygiene Products", weeklySeries);
    *wedSeries << new DrilldownSlice(0, "Transportation", weeklySeries);
    *wedSeries << new DrilldownSlice(0, "Personal", weeklySeries);
    *wedSeries << new DrilldownSlice(0, "Food", weeklySeries);
    *wedSeries << new DrilldownSlice(0, "School Supplies", weeklySeries);

    *thursSeries << new DrilldownSlice(1, "Groceries", weeklySeries);
    *thursSeries << new DrilldownSlice(0, "Hygiene Products", weeklySeries);
    *thursSeries << new DrilldownSlice(0, "Transportation", weeklySeries);
    *thursSeries << new DrilldownSlice(0, "Personal", weeklySeries);
    *thursSeries << new DrilldownSlice(0, "Food", weeklySeries);
    *thursSeries << new DrilldownSlice(0, "School Supplies", weeklySeries);

    *friSeries << new DrilldownSlice(1, "Groceries", weeklySeries);
    *friSeries << new DrilldownSlice(0, "Hygiene Products", weeklySeries);
    *friSeries << new DrilldownSlice(0, "Transportation", weeklySeries);
    *friSeries << new DrilldownSlice(0, "Personal", weeklySeries);
    *friSeries << new DrilldownSlice(0, "Food", weeklySeries);
    *friSeries << new DrilldownSlice(0, "School Supplies", weeklySeries);

    *satSeries << new DrilldownSlice(1, "Groceries", weeklySeries);
    *satSeries << new DrilldownSlice(0, "Hygiene Products", weeklySeries);
    *satSeries << new DrilldownSlice(0, "Transportation", weeklySeries);
    *satSeries << new DrilldownSlice(0, "Personal", weeklySeries);
    *satSeries << new DrilldownSlice(0, "Food", weeklySeries);
    *satSeries << new DrilldownSlice(0, "School Supplies", weeklySeries);

    *sunSeries << new DrilldownSlice(1, "Groceries", weeklySeries);
    *sunSeries << new DrilldownSlice(0, "Hygiene Products", weeklySeries);
    *sunSeries << new DrilldownSlice(0, "Transportation", weeklySeries);
    *sunSeries << new DrilldownSlice(0, "Personal", weeklySeries);
    *sunSeries << new DrilldownSlice(0, "Food", weeklySeries);
    *sunSeries << new DrilldownSlice(0, "School Supplies", weeklySeries);

    // make a QPieSeries for the week
    *weeklySeries << new DrilldownSlice(1, "Mon", monSeries);
    *weeklySeries << new DrilldownSlice(0, "Tues", tuesSeries);
    *weeklySeries << new DrilldownSlice(0, "Wed", wedSeries);
    *weeklySeries << new DrilldownSlice(0, "Thur", thursSeries);
    *weeklySeries << new DrilldownSlice(0, "Fri", friSeries);
    *weeklySeries << new DrilldownSlice(0, "Sat", satSeries);
    *weeklySeries << new DrilldownSlice(0, "Sun", sunSeries);

    // connect each daily series to the weekly pie chart
    QObject::connect(monSeries, &QPieSeries::clicked, weeklyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(tuesSeries, &QPieSeries::clicked, weeklyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(wedSeries, &QPieSeries::clicked, weeklyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(thursSeries, &QPieSeries::clicked, weeklyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(friSeries, &QPieSeries::clicked, weeklyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(satSeries, &QPieSeries::clicked, weeklyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(sunSeries, &QPieSeries::clicked, weeklyPieChart, &DrilldownChart::handleSliceClicked);

    // connected the weekly series to the pie chart
    QObject::connect(weeklySeries, &QPieSeries::clicked, weeklyPieChart, &DrilldownChart::handleSliceClicked);

    weeklyPieChart->changeSeries(weeklySeries);

    QChartView *weeklyPieChartView = new QChartView(weeklyPieChart);
    weeklyPieChartView->setRenderHint(QPainter::Antialiasing);

    ui->verticalLayout_9->addWidget(weeklyPieChartView);


    /* Creating the pi chart for the monthly expenses */

    DrilldownChart *monthlyPieChart = new DrilldownChart();
    monthlyPieChart->setTheme(QChart::ChartThemeLight);
    monthlyPieChart->setAnimationOptions(QChart::AllAnimations);
    monthlyPieChart->legend()->setVisible(true);
    monthlyPieChart->legend()->setAlignment(Qt::AlignRight);

    monthlySeries->setName("Breakdown of Monthly Expenses by Week");

    const QStringList monthlyPieCategories = {
        "Phone Bill", "Utility Bill", "Rent/Lease/Loan", "Car Lease/Loan", "Credit Expenses", "Savings"
    };
    const QStringList weeks = {
        "Wk 1", "Wk 2", "Wk 3", "Wk 4", "Wk 5"
    };

    // make a QPieSeries for each day
    week1Series->setName("Breakdown of Monthly Expenses: Week 1");
    week2Series->setName("Breakdown of Monthly Expenses: Week 2");
    week3Series->setName("Breakdown of Monthly Expenses: Week 3");
    week4Series->setName("Breakdown of Monthly Expenses: Week 4");
    week5Series->setName("Breakdown of Monthly Expenses: Week 5");

    // make a QPieSeries for each category within the day
    *week1Series << new DrilldownSlice(1, "Phone Bill", weeklySeries);
    *week1Series << new DrilldownSlice(0, "Utility Bill", weeklySeries);
    *week1Series << new DrilldownSlice(0, "Rent/Lease/Loan", weeklySeries);
    *week1Series << new DrilldownSlice(0, "Car Lease/Loan", weeklySeries);
    *week1Series << new DrilldownSlice(0, "Credit Expenses", weeklySeries);
    *week1Series << new DrilldownSlice(0, "Savings", weeklySeries);

    *week2Series << new DrilldownSlice(1, "Phone Bill", weeklySeries);
    *week2Series << new DrilldownSlice(0, "Utility Bill", weeklySeries);
    *week2Series << new DrilldownSlice(0, "Rent/Lease/Loan", weeklySeries);
    *week2Series << new DrilldownSlice(0, "Car Lease/Loan", weeklySeries);
    *week2Series << new DrilldownSlice(0, "Credit Expenses", weeklySeries);
    *week2Series << new DrilldownSlice(0, "Savings", weeklySeries);

    *week3Series << new DrilldownSlice(1, "Phone Bill", weeklySeries);
    *week3Series << new DrilldownSlice(0, "Utility Bill", weeklySeries);
    *week3Series << new DrilldownSlice(0, "Rent/Lease/Loan", weeklySeries);
    *week3Series << new DrilldownSlice(0, "Car Lease/Loan", weeklySeries);
    *week3Series << new DrilldownSlice(0, "Credit Expenses", weeklySeries);
    *week3Series << new DrilldownSlice(0, "Savings", weeklySeries);

    *week4Series << new DrilldownSlice(1, "Phone Bill", weeklySeries);
    *week4Series << new DrilldownSlice(0, "Utility Bill", weeklySeries);
    *week4Series << new DrilldownSlice(0, "Rent/Lease/Lone", weeklySeries);
    *week4Series << new DrilldownSlice(0, "Car Lease/Loan", weeklySeries);
    *week4Series << new DrilldownSlice(0, "Credit Expenses", weeklySeries);
    *week4Series << new DrilldownSlice(0, "Savings", weeklySeries);

    *week5Series << new DrilldownSlice(1, "Phone Bill", weeklySeries);
    *week5Series << new DrilldownSlice(0, "Utility Bill", weeklySeries);
    *week5Series << new DrilldownSlice(0, "Rent/Lease/Loan", weeklySeries);
    *week5Series << new DrilldownSlice(0, "Car Lease/Loan", weeklySeries);
    *week5Series << new DrilldownSlice(0, "Credit Expenses", weeklySeries);
    *week5Series << new DrilldownSlice(0, "Savings", weeklySeries);

    // make a QPieSeries for the week
    *monthlySeries << new DrilldownSlice(1, "Wk 1", week1Series);
    *monthlySeries << new DrilldownSlice(0, "Wk 2", week2Series);
    *monthlySeries << new DrilldownSlice(0, "Wk 3", week3Series);
    *monthlySeries << new DrilldownSlice(0, "Wk 4", week4Series);
    *monthlySeries << new DrilldownSlice(0, "Wk 5", week5Series);

    // connect each daily series to the weekly pie chart
    QObject::connect(week1Series, &QPieSeries::clicked, monthlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(week2Series, &QPieSeries::clicked, monthlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(week3Series, &QPieSeries::clicked, monthlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(week4Series, &QPieSeries::clicked, monthlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(week5Series, &QPieSeries::clicked, monthlyPieChart, &DrilldownChart::handleSliceClicked);

    // connected the weekly series to the pie chart
    QObject::connect(monthlySeries, &QPieSeries::clicked, monthlyPieChart, &DrilldownChart::handleSliceClicked);

    monthlyPieChart->changeSeries(monthlySeries);

    QChartView *monthlyPieChartView = new QChartView(monthlyPieChart);
    monthlyPieChartView->setRenderHint(QPainter::Antialiasing);

    ui->verticalLayout_4->addWidget(monthlyPieChartView);


    /* Creating the pi chart for the yearly expenses */

    DrilldownChart *yearlyPieChart = new DrilldownChart();
    yearlyPieChart->setTheme(QChart::ChartThemeLight);
    yearlyPieChart->setAnimationOptions(QChart::AllAnimations);
    yearlyPieChart->legend()->setVisible(true);
    yearlyPieChart->legend()->setAlignment(Qt::AlignRight);

    yearlySeries->setName("Breakdown of Yearly Expenses by Month");

    const QStringList yearlyPieCategories = {
        "Life Insurance", "Car Insurance", "Health Insurance", "Gifts", "Clothing", "Home/Renovations", "Student Loans", "Tuition Fee"
    };
    const QStringList months = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"
    };

    // make a QPieSeries for each day
    janSeries->setName("Breakdown of Yearly Expenses: January");
    febSeries->setName("Breakdown of Yearly Expenses: February");
    marSeries->setName("Breakdown of Yearly Expenses: March");
    aprSeries->setName("Breakdown of Yearly Expenses: April");
    maySeries->setName("Breakdown of Yearly Expenses: May");
    junSeries->setName("Breakdown of Yearly Expenses: June");
    julSeries->setName("Breakdown of Yearly Expenses: July");
    augSeries->setName("Breakdown of Yearly Expenses: August");
    septSeries->setName("Breakdown of Yearly Expenses: September");
    octSeries->setName("Breakdown of Yearly Expenses: October");
    novSeries->setName("Breakdown of Yearly Expenses: November");
    decSeries->setName("Breakdown of Yearly Expenses: December");

    // make a QPieSeries for each category within the day
    *janSeries << new DrilldownSlice(1, "Life Insurance", yearlySeries);
    *janSeries << new DrilldownSlice(0, "Car Insurance", yearlySeries);
    *janSeries << new DrilldownSlice(0, "Health Insurance", yearlySeries);
    *janSeries << new DrilldownSlice(0, "Gifts", yearlySeries);
    *janSeries << new DrilldownSlice(0, "Clothing", yearlySeries);
    *janSeries << new DrilldownSlice(0, "Home/Renovations", yearlySeries);
    *janSeries << new DrilldownSlice(0, "Student Loans", yearlySeries);
    *janSeries << new DrilldownSlice(0, "Tuition Fee", yearlySeries);

    *febSeries << new DrilldownSlice(1, "Life Insurance", yearlySeries);
    *febSeries << new DrilldownSlice(0, "Car Insurance", yearlySeries);
    *febSeries << new DrilldownSlice(0, "Health Insurance", yearlySeries);
    *febSeries << new DrilldownSlice(0, "Gifts", yearlySeries);
    *febSeries << new DrilldownSlice(0, "Clothing", yearlySeries);
    *febSeries << new DrilldownSlice(0, "Home/Renovations", yearlySeries);
    *febSeries << new DrilldownSlice(0, "Student Loans", yearlySeries);
    *febSeries << new DrilldownSlice(0, "Tuition Fee", yearlySeries);

    *marSeries << new DrilldownSlice(1, "Life Insurance", yearlySeries);
    *marSeries << new DrilldownSlice(0, "Car Insurance", yearlySeries);
    *marSeries << new DrilldownSlice(0, "Health Insurance", yearlySeries);
    *marSeries << new DrilldownSlice(0, "Gifts", yearlySeries);
    *marSeries << new DrilldownSlice(0, "Clothing", yearlySeries);
    *marSeries << new DrilldownSlice(0, "Home/Renovations", yearlySeries);
    *marSeries << new DrilldownSlice(0, "Student Loans", yearlySeries);
    *marSeries << new DrilldownSlice(0, "Tuition Fee", yearlySeries);

    *aprSeries << new DrilldownSlice(1, "Life Insurance", yearlySeries);
    *aprSeries << new DrilldownSlice(0, "Car Insurance", yearlySeries);
    *aprSeries << new DrilldownSlice(0, "Health Insurance", yearlySeries);
    *aprSeries << new DrilldownSlice(0, "Gifts", yearlySeries);
    *aprSeries << new DrilldownSlice(0, "Clothing", yearlySeries);
    *aprSeries << new DrilldownSlice(0, "Home/Renovations", yearlySeries);
    *aprSeries << new DrilldownSlice(0, "Student Loans", yearlySeries);
    *aprSeries << new DrilldownSlice(0, "Tuition Fee", yearlySeries);

    *maySeries << new DrilldownSlice(1, "Life Insurance", yearlySeries);
    *maySeries << new DrilldownSlice(0, "Car Insurance", yearlySeries);
    *maySeries << new DrilldownSlice(0, "Health Insurance", yearlySeries);
    *maySeries << new DrilldownSlice(0, "Gifts", yearlySeries);
    *maySeries << new DrilldownSlice(0, "Clothing", yearlySeries);
    *maySeries << new DrilldownSlice(0, "Home/Renovations", yearlySeries);
    *maySeries << new DrilldownSlice(0, "Student Loans", yearlySeries);
    *maySeries << new DrilldownSlice(0, "Tuition Fee", yearlySeries);

    *junSeries << new DrilldownSlice(1, "Life Insurance", yearlySeries);
    *junSeries << new DrilldownSlice(0, "Car Insurance", yearlySeries);
    *junSeries << new DrilldownSlice(0, "Health Insurance", yearlySeries);
    *junSeries << new DrilldownSlice(0, "Gifts", yearlySeries);
    *junSeries << new DrilldownSlice(0, "Clothing", yearlySeries);
    *junSeries << new DrilldownSlice(0, "Home/Renovations", yearlySeries);
    *junSeries << new DrilldownSlice(0, "Student Loans", yearlySeries);
    *junSeries << new DrilldownSlice(0, "Tuition Fee", yearlySeries);

    *julSeries << new DrilldownSlice(1, "Life Insurance", yearlySeries);
    *julSeries << new DrilldownSlice(0, "Car Insurance", yearlySeries);
    *julSeries << new DrilldownSlice(0, "Health Insurance", yearlySeries);
    *julSeries << new DrilldownSlice(0, "Gifts", yearlySeries);
    *julSeries << new DrilldownSlice(0, "Clothing", yearlySeries);
    *julSeries << new DrilldownSlice(0, "Home/Renovations", yearlySeries);
    *julSeries << new DrilldownSlice(0, "Student Loans", yearlySeries);
    *julSeries << new DrilldownSlice(0, "Tuition Fee", yearlySeries);

    *augSeries << new DrilldownSlice(1, "Life Insurance", yearlySeries);
    *augSeries << new DrilldownSlice(0, "Car Insurance", yearlySeries);
    *augSeries << new DrilldownSlice(0, "Health Insurance", yearlySeries);
    *augSeries << new DrilldownSlice(0, "Gifts", yearlySeries);
    *augSeries << new DrilldownSlice(0, "Clothing", yearlySeries);
    *augSeries << new DrilldownSlice(0, "Home/Renovations", yearlySeries);
    *augSeries << new DrilldownSlice(0, "Student Loans", yearlySeries);
    *augSeries << new DrilldownSlice(0, "Tuition Fee", yearlySeries);

    *septSeries << new DrilldownSlice(1, "Life Insurance", yearlySeries);
    *septSeries << new DrilldownSlice(0, "Car Insurance", yearlySeries);
    *septSeries << new DrilldownSlice(0, "Health Insurance", yearlySeries);
    *septSeries << new DrilldownSlice(0, "Gifts", yearlySeries);
    *septSeries << new DrilldownSlice(0, "Clothing", yearlySeries);
    *septSeries << new DrilldownSlice(0, "Home/Renovations", yearlySeries);
    *septSeries << new DrilldownSlice(0, "Student Loans", yearlySeries);
    *septSeries << new DrilldownSlice(0, "Tuition Fee", yearlySeries);

    *octSeries << new DrilldownSlice(1, "Life Insurance", yearlySeries);
    *octSeries << new DrilldownSlice(0, "Car Insurance", yearlySeries);
    *octSeries << new DrilldownSlice(0, "Health Insurance", yearlySeries);
    *octSeries << new DrilldownSlice(0, "Gifts", yearlySeries);
    *octSeries << new DrilldownSlice(0, "Clothing", yearlySeries);
    *octSeries << new DrilldownSlice(0, "Home/Renovations", yearlySeries);
    *octSeries << new DrilldownSlice(0, "Student Loans", yearlySeries);
    *octSeries << new DrilldownSlice(0, "Tuition Fee", yearlySeries);

    *novSeries << new DrilldownSlice(1, "Life Insurance", yearlySeries);
    *novSeries << new DrilldownSlice(0, "Car Insurance", yearlySeries);
    *novSeries << new DrilldownSlice(0, "Health Insurance", yearlySeries);
    *novSeries << new DrilldownSlice(0, "Gifts", yearlySeries);
    *novSeries << new DrilldownSlice(0, "Clothing", yearlySeries);
    *novSeries << new DrilldownSlice(0, "Home/Renovations", yearlySeries);
    *novSeries << new DrilldownSlice(0, "Student Loans", yearlySeries);
    *novSeries << new DrilldownSlice(0, "Tuition Fee", yearlySeries);

    *decSeries << new DrilldownSlice(1, "Life Insurance", yearlySeries);
    *decSeries << new DrilldownSlice(0, "Car Insurance", yearlySeries);
    *decSeries << new DrilldownSlice(0, "Health Insurance", yearlySeries);
    *decSeries << new DrilldownSlice(0, "Gifts", yearlySeries);
    *decSeries << new DrilldownSlice(0, "Clothing", yearlySeries);
    *decSeries << new DrilldownSlice(0, "Home/Renovations", yearlySeries);
    *decSeries << new DrilldownSlice(0, "Student Loans", yearlySeries);
    *decSeries << new DrilldownSlice(0, "Tuition Fee", yearlySeries);

    // make a QPieSeries for the week
    *yearlySeries << new DrilldownSlice(1, "Jan", janSeries);
    *yearlySeries << new DrilldownSlice(0, "Feb", febSeries);
    *yearlySeries << new DrilldownSlice(0, "Mar", marSeries);
    *yearlySeries << new DrilldownSlice(0, "Apr", aprSeries);
    *yearlySeries << new DrilldownSlice(0, "May", maySeries);
    *yearlySeries << new DrilldownSlice(0, "Jun", junSeries);
    *yearlySeries << new DrilldownSlice(0, "Jul", julSeries);
    *yearlySeries << new DrilldownSlice(0, "Aug", augSeries);
    *yearlySeries << new DrilldownSlice(0, "Sept", septSeries);
    *yearlySeries << new DrilldownSlice(0, "Oct", octSeries);
    *yearlySeries << new DrilldownSlice(0, "Nov", novSeries);
    *yearlySeries << new DrilldownSlice(0, "Dec", decSeries);

    // connect each daily series to the weekly pie chart
    QObject::connect(janSeries, &QPieSeries::clicked, yearlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(febSeries, &QPieSeries::clicked, yearlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(marSeries, &QPieSeries::clicked, yearlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(aprSeries, &QPieSeries::clicked, yearlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(maySeries, &QPieSeries::clicked, yearlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(junSeries, &QPieSeries::clicked, yearlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(julSeries, &QPieSeries::clicked, yearlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(augSeries, &QPieSeries::clicked, yearlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(septSeries, &QPieSeries::clicked, yearlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(octSeries, &QPieSeries::clicked, yearlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(novSeries, &QPieSeries::clicked, yearlyPieChart, &DrilldownChart::handleSliceClicked);
    QObject::connect(decSeries, &QPieSeries::clicked, yearlyPieChart, &DrilldownChart::handleSliceClicked);

    // connected the weekly series to the pie chart
    QObject::connect(yearlySeries, &QPieSeries::clicked, yearlyPieChart, &DrilldownChart::handleSliceClicked);

    yearlyPieChart->changeSeries(yearlySeries);

    QChartView *yearlyPieChartView = new QChartView(yearlyPieChart);
    yearlyPieChartView->setRenderHint(QPainter::Antialiasing);

    ui->verticalLayout_13->addWidget(yearlyPieChartView);

/////////////////////////////// ADDING DEFAULT ITEMS TO  TREE WIDGETS /////////////////////////////////

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
    cateList << "Hygiene Products";
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

/////////////////////////////// BEHAVIOURAL FUNCTIONS FOR TREEE WIDGETS AND PLOTS /////////////////////////////////

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

    // find the index of the category chosen
    int cateIndex;
    if (cate == "Groceries")
        cateIndex = 0;
    else if (cate == "Hygiene Products")
        cateIndex = 1;
    else if (cate == "Transportation")
        cateIndex = 2;
    else if (cate == "Personal")
        cateIndex = 3;
    else if (cate == "Food")
        cateIndex = 4;
    else
        cateIndex = 5;

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

    // create switch case to figure out which category was added to
    switch(cateIndex) {
    case 0 :
        switch(selectedIndex) {
        case 0:
            monGrocery += amountInt;
            break;
        case 1:
            tuesGrocery += amountInt;
            break;
        case 2:
            wedGrocery += amountInt;
            break;
        case 3:
            thursGrocery += amountInt;
            break;
        case 4:
            friGrocery += amountInt;
            break;
        case 5:
            satGrocery += amountInt;
            break;
        case 6:
            sunGrocery += amountInt;
            break;
        }
        break;
    case 1:
        switch(selectedIndex) {
        case 0:
            monHyg += amountInt;
            break;
        case 1:
            tuesHyg += amountInt;
            break;
        case 2:
            wedHyg += amountInt;
            break;
        case 3:
            thursHyg += amountInt;
            break;
        case 4:
            friHyg += amountInt;
            break;
        case 5:
            satHyg += amountInt;
            break;
        case 6:
            sunHyg += amountInt;
            break;
        }
        break;
    case 2:
        switch(selectedIndex) {
        case 0:
            monTrans += amountInt;
            break;
        case 1:
            tuesTrans += amountInt;
            break;
        case 2:
            wedTrans += amountInt;
            break;
        case 3:
            thursTrans += amountInt;
            break;
        case 4:
            friTrans += amountInt;
            break;
        case 5:
            satTrans += amountInt;
            break;
        case 6:
            sunTrans += amountInt;
            break;
        default:
            break;
        }
        break;
    default:
        break;
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

    // clear the pie chart series and redistribute the slices
    weeklySeries->clear();
    *weeklySeries << new DrilldownSlice(monExpenses, "Mon", monSeries);
    *weeklySeries << new DrilldownSlice(tuesExpenses, "Tues", tuesSeries);
    *weeklySeries << new DrilldownSlice(wedExpenses, "Wed", wedSeries);
    *weeklySeries << new DrilldownSlice(thursExpenses, "Thur", thursSeries);
    *weeklySeries << new DrilldownSlice(friExpenses, "Fri", friSeries);
    *weeklySeries << new DrilldownSlice(satExpenses, "Sat", satSeries);
    *weeklySeries << new DrilldownSlice(sunExpenses, "Sun", sunSeries);

    // clear the pie charts for each category and redistribute the slices
    monSeries->clear();
    *monSeries << new DrilldownSlice(monGrocery, "Groceries", weeklySeries);
    *monSeries << new DrilldownSlice(monHyg, "Hygiene Products", weeklySeries);
    *monSeries << new DrilldownSlice(monTrans, "Transportation", weeklySeries);
    *monSeries << new DrilldownSlice(0, "Personal", weeklySeries);
    *monSeries << new DrilldownSlice(0, "Food", weeklySeries);
    *monSeries << new DrilldownSlice(0, "School Supplies", weeklySeries);

    tuesSeries->clear();
    *tuesSeries << new DrilldownSlice(tuesGrocery, "Groceries", weeklySeries);
    *tuesSeries << new DrilldownSlice(tuesHyg, "Hygiene Products", weeklySeries);
    *tuesSeries << new DrilldownSlice(tuesTrans, "Transportation", weeklySeries);
    *tuesSeries << new DrilldownSlice(0, "Personal", weeklySeries);
    *tuesSeries << new DrilldownSlice(0, "Food", weeklySeries);
    *tuesSeries << new DrilldownSlice(0, "School Supplies", weeklySeries);

    wedSeries->clear();
    *wedSeries << new DrilldownSlice(wedGrocery, "Groceries", weeklySeries);
    *wedSeries << new DrilldownSlice(wedHyg, "Hygiene Products", weeklySeries);
    *wedSeries << new DrilldownSlice(wedTrans, "Transportation", weeklySeries);
    *wedSeries << new DrilldownSlice(0, "Personal", weeklySeries);
    *wedSeries << new DrilldownSlice(0, "Food", weeklySeries);
    *wedSeries << new DrilldownSlice(0, "School Supplies", weeklySeries);

    thursSeries->clear();
    *thursSeries << new DrilldownSlice(thursGrocery, "Groceries", weeklySeries);
    *thursSeries << new DrilldownSlice(thursHyg, "Hygiene Products", weeklySeries);
    *thursSeries << new DrilldownSlice(thursTrans, "Transportation", weeklySeries);
    *thursSeries << new DrilldownSlice(0, "Personal", weeklySeries);
    *thursSeries << new DrilldownSlice(0, "Food", weeklySeries);
    *thursSeries << new DrilldownSlice(0, "School Supplies", weeklySeries);

    friSeries->clear();
    *friSeries << new DrilldownSlice(friGrocery, "Groceries", weeklySeries);
    *friSeries << new DrilldownSlice(friHyg, "Hygiene Products", weeklySeries);
    *friSeries << new DrilldownSlice(friTrans, "Transportation", weeklySeries);
    *friSeries << new DrilldownSlice(0, "Personal", weeklySeries);
    *friSeries << new DrilldownSlice(0, "Food", weeklySeries);
    *friSeries << new DrilldownSlice(0, "School Supplies", weeklySeries);

    satSeries->clear();
    *satSeries << new DrilldownSlice(satGrocery, "Groceries", weeklySeries);
    *satSeries << new DrilldownSlice(satHyg, "Hygiene Products", weeklySeries);
    *satSeries << new DrilldownSlice(satTrans, "Transportation", weeklySeries);
    *satSeries << new DrilldownSlice(0, "Personal", weeklySeries);
    *satSeries << new DrilldownSlice(0, "Food", weeklySeries);
    *satSeries << new DrilldownSlice(0, "School Supplies", weeklySeries);

    sunSeries->clear();
    *sunSeries << new DrilldownSlice(satGrocery, "Groceries", weeklySeries);
    *sunSeries << new DrilldownSlice(satHyg, "Hygiene Products", weeklySeries);
    *sunSeries << new DrilldownSlice(satTrans, "Transportation", weeklySeries);
    *sunSeries << new DrilldownSlice(0, "Personal", weeklySeries);
    *sunSeries << new DrilldownSlice(0, "Food", weeklySeries);
    *sunSeries << new DrilldownSlice(0, "School Supplies", weeklySeries);

    // update the maximum bounding of the chart
    int axisVal = maxWeeklyVal;
    int checkingExpenses[] = {monExpenses, tuesExpenses, wedExpenses, thursExpenses, friExpenses, satExpenses, sunExpenses};
    for (int i = 1; i < 7; i++) {
        if (checkingExpenses[i] > maxWeeklyVal)
            axisVal = checkingExpenses[i];
    }
    axisY->setRange(0, axisVal + 100);
    //axisY->maxChanged(axisVal + 100);
    //axisY->applyNiceNumbers();
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

    // clear the pie chart series and redistribute the slices
    monthlySeries->clear();
    *monthlySeries << new DrilldownSlice(week1, "Wk 1", week1Series);
    *monthlySeries << new DrilldownSlice(week2, "Wk 2", week2Series);
    *monthlySeries << new DrilldownSlice(week3, "Wk 3", week3Series);
    *monthlySeries << new DrilldownSlice(week4, "Wk 4", week4Series);
    *monthlySeries << new DrilldownSlice(extraDays, "Wk 5", week5Series);
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

    // clear the pie chart series and redistribute the slices
    yearlySeries->clear();
    *yearlySeries << new DrilldownSlice(janExp, "Jan", janSeries);
    *yearlySeries << new DrilldownSlice(febExp, "Feb", febSeries);
    *yearlySeries << new DrilldownSlice(marExp, "Mar", marSeries);
    *yearlySeries << new DrilldownSlice(aprExp, "Apr", aprSeries);
    *yearlySeries << new DrilldownSlice(mayExp, "May", maySeries);
    *yearlySeries << new DrilldownSlice(junExp, "Jun", junSeries);
    *yearlySeries << new DrilldownSlice(julExp, "Jul", julSeries);
    *yearlySeries << new DrilldownSlice(augExp, "Aug", augSeries);
    *yearlySeries << new DrilldownSlice(septExp, "Sept", septSeries);
    *yearlySeries << new DrilldownSlice(octExp, "Oct", octSeries);
    *yearlySeries << new DrilldownSlice(novExp, "Nov", novSeries);
    *yearlySeries << new DrilldownSlice(decExp, "Dec", decSeries);
}

void BudgetTarget::on_DeleteE_clicked(){
    // if an item in the tree widget is selected
    if(ui->treeWidget_2->currentItem()!=nullptr) {
        QModelIndex index = ui->treeWidget_2->currentIndex();
        QString category = ui->treeWidget_2->currentItem()->text(0);
        int cateIndex = cateList.indexOf(category);
        QString before = ui->treeWidget->topLevelItem(cateIndex)->text(2);
        QString add = ui->treeWidget_2->currentItem()->text(1);

        QString day = ui->treeWidget_2->currentItem()->text(2);

        int update = before.toInt()+add.toInt();

        QString dayString = ui->treeWidget_2->currentItem()->text(2);
        int selectedIndex = dayList.indexOf(dayString);

        ui->treeWidget->topLevelItem(cateIndex)->setData(2,Qt::DisplayRole,update);
        ui->treeWidget_2->takeTopLevelItem(index.row());

        // connect to the database and store information
        // needs to go above the deleting code
        s.connectsql();
        QString oldValue = s.select(s.sql_query, "database", category,2);
        QString amount = (QString)(add.toInt()* - 1);
        QString oldExp = s.select(s.sql_query, "database", category, dayList.indexOf(day)+3);
        s.updateExpenditureD(s.sql_query, "database", category, oldValue, oldExp,add, day);

        s.close();

        // adjust the plot according to the amount deleted
        int deletedAmount = add.toInt();
        switch(selectedIndex) {
            case 0 : monExpenses -= deletedAmount;
            break;
            case 1 : tuesExpenses -= deletedAmount;
            break;
            case 2 : wedExpenses -= deletedAmount;
            break;
            case 3 : thursExpenses -= deletedAmount;
            break;
            case 4 : friExpenses -= deletedAmount;
            break;
            case 5 : satExpenses -= deletedAmount;
            break;
            case 6 : sunExpenses -= deletedAmount;
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

        // clear the pie chart series and redistribute the slices
        weeklySeries->clear();
        *weeklySeries << new DrilldownSlice(monExpenses, "Monday", monSeries);
        *weeklySeries << new DrilldownSlice(tuesExpenses, "Tuesday", tuesSeries);
        *weeklySeries << new DrilldownSlice(wedExpenses, "Wednesday", wedSeries);
        *weeklySeries << new DrilldownSlice(thursExpenses, "Thursday", thursSeries);
        *weeklySeries << new DrilldownSlice(friExpenses, "Friday", friSeries);
        *weeklySeries << new DrilldownSlice(satExpenses, "Saturday", satSeries);
        *weeklySeries << new DrilldownSlice(sunExpenses, "Sunday", sunSeries);
    }
}

void BudgetTarget::on_DeleteE_2_clicked(){
    // if an item in the tree widget is selected
    if(ui->treeWidget_4->currentItem()!=nullptr){
        QModelIndex index = ui->treeWidget_4->currentIndex();
        QString category = ui->treeWidget_4->currentItem()->text(0);
        int cateIndex = cateListM.indexOf(category);
        QString before = ui->treeWidget_3->topLevelItem(cateIndex)->text(2);
        QString add = ui->treeWidget_4->currentItem()->text(1);
        QString day = ui->treeWidget_4->currentItem()->text(2);
        int update = before.toInt()+add.toInt();

        QString dayString = ui->treeWidget_4->currentItem()->text(2);
        int dayInt = dayString.toInt();
        int selectedIndex = dayInt / 7;

        ui->treeWidget_3->topLevelItem(cateIndex)->setData(2,Qt::DisplayRole,update);
        ui->treeWidget_4->takeTopLevelItem(index.row());

        s.connectsql1();
        QString oldValue = s.select(s.sql_query1, "database1", category,2);
        QString amount = (QString)(add.toInt()*-1);
        QString oldExp = s.select(s.sql_query1, "database1", category, day.toInt()+3);
        s.updateExpenditureD(s.sql_query1, "database1", category, oldValue, oldExp,add, day);
        s.close();

        // adjust the plot accoring to the amount deleted
        int deletedAmount = add.toInt();
        switch(selectedIndex) {
            case 0 : week1 -= deletedAmount;
            break;
            case 1 : week2 -= deletedAmount;
            break;
            case 2 : week3 -= deletedAmount;
            break;
            case 3 : week4 -= deletedAmount;
            break;
            case 4 : extraDays -= deletedAmount;
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


        // clear the pie chart series and redistribute the slices
        monthlySeries->clear();
        *monthlySeries << new DrilldownSlice(week1, "Week 1", week1Series);
        *monthlySeries << new DrilldownSlice(week2, "Week 2", week2Series);
        *monthlySeries << new DrilldownSlice(week3, "Week 3", week3Series);
        *monthlySeries << new DrilldownSlice(week4, "Week 5", week4Series);
        *monthlySeries << new DrilldownSlice(extraDays, "Week 6", week5Series);
    }
}

void BudgetTarget::on_DeleteE_3_clicked(){
    // if an item in the tree widget is selected
    if(ui->treeWidget_7->currentItem()!=nullptr){
        QModelIndex index = ui->treeWidget_7->currentIndex();
        QString category = ui->treeWidget_7->currentItem()->text(0);
        int cateIndex = cateListY.indexOf(category);
        QString before = ui->treeWidget_6->topLevelItem(cateIndex)->text(2);
        QString add = ui->treeWidget_7->currentItem()->text(1);
        QString month = ui->treeWidget_7->currentItem()->text(2);

        int update = before.toInt()+add.toInt();

        s.connectsql2();
        QString oldValue = s.select(s.sql_query2, "database2", category,2);
        QString amount = (QString)(add.toInt()*-1);
        QString oldExp = s.select(s.sql_query2, "database2", category, dayListY.indexOf(month)+3);
        s.updateExpenditureD(s.sql_query2, "database2", category, oldValue, oldExp,add, month);

        s.close();

        QString dayString = ui->treeWidget_7->currentItem()->text(2);
        int selectedIndex = dayListY.indexOf(dayString);

        ui->treeWidget_6->topLevelItem(cateIndex)->setData(2,Qt::DisplayRole,update);
        ui->treeWidget_7->takeTopLevelItem(index.row());

        // convert the string to an int and add it to the corresponding date
        int amountInt = add.toInt();
        switch(selectedIndex) {
            case 0 : janExp -= amountInt;
            break;
            case 1 : febExp -= amountInt;
            break;
            case 2 : marExp -= amountInt;
            break;
            case 3 : aprExp -= amountInt;
            break;
            case 4 : mayExp -= amountInt;
            break;
            case 5 : junExp -= amountInt;
            break;
            case 6 : julExp -= amountInt;
            break;
            case 7 : augExp -= amountInt;
            break;
            case 8 : septExp -= amountInt;
            break;
            case 9 : octExp -= amountInt;
            break;
            case 10 : novExp -= amountInt;
            break;
            case 11 : decExp -= amountInt;
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

        // clear the pie chart series and redistribute the slices
        yearlySeries->clear();
        *yearlySeries << new DrilldownSlice(janExp, "January", janSeries);
        *yearlySeries << new DrilldownSlice(febExp, "Febrary", febSeries);
        *yearlySeries << new DrilldownSlice(marExp, "March", marSeries);
        *yearlySeries << new DrilldownSlice(aprExp, "April", aprSeries);
        *yearlySeries << new DrilldownSlice(mayExp, "May", maySeries);
        *yearlySeries << new DrilldownSlice(junExp, "June", junSeries);
        *yearlySeries << new DrilldownSlice(julExp, "July", julSeries);
        *yearlySeries << new DrilldownSlice(augExp, "August", augSeries);
        *yearlySeries << new DrilldownSlice(septExp, "September", septSeries);
        *yearlySeries << new DrilldownSlice(octExp, "October", octSeries);
        *yearlySeries << new DrilldownSlice(novExp, "November", novSeries);
        *yearlySeries << new DrilldownSlice(decExp, "December", decSeries);
    }
}
