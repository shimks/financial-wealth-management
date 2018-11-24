#include "budgettarget.h"
#include "ui_budgettarget.h"
#include "addexpense.h"
#include "ui_popup_window.h"
#include "popup_window.h"
#include <QMessageBox>
#include "adddialog.h"
#include "addexpensem.h"
#include "addexpensey.h"
#include <QFile>
#include <QTextStream>
#include <QtWidgets>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <QtCharts>
#include <algorithm>

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
    int weeklyExpenses[] = {monExpenses, tuesExpenses, wedExpenses, thursExpenses, friExpenses, satExpenses, sunExpenses};
    int monthlyExpenses[] = {janExp, febExp, marExp, aprExp, mayExp, junExp, julExp, augExp, septExp, octExp, novExp, decExp};

    // find the maximum weekly expense value and use this to change the range of the weekly line/bar chart
    int maxWeeklyVal = monExpenses;
    for (int i = 1; i < 7; i++) {
        if (weeklyExpenses[i] > weeklyExpenses[i-1])
            maxWeeklyVal = weeklyExpenses[i];
    }

    // find the maximum monthly expense value and use this to change the range of the monthly line/bar chart
    int maxMonthlyVal = janExp;
    for (int i = 1; i < 12; i++) {
        if (monthlyExpenses[i] > monthlyExpenses[i-1])
            maxMonthlyVal = monthlyExpenses[i];
    }

    // add days of the week and months of the year to QBarSet
    *daysOfWeek << monExpenses << tuesExpenses << wedExpenses << thursExpenses << friExpenses << satExpenses << sunExpenses;
    *monthsOfYear << janExp << febExp << marExp << aprExp << mayExp << junExp << julExp << augExp << septExp << octExp << novExp << decExp;

    // add the days of the week and months of the year to their QBarSeries
    barseries->append(daysOfWeek);
    yearlyBarSeires->append(monthsOfYear);

    // add points for the weekly line chart to the plot
    lineseries->setName("Weekly Expense Trend");
    lineseries->append(QPoint(0, monExpenses));
    lineseries->append(QPoint(1, tuesExpenses));
    lineseries->append(QPoint(2, wedExpenses));
    lineseries->append(QPoint(3, thursExpenses));
    lineseries->append(QPoint(4, friExpenses));
    lineseries->append(QPoint(5, satExpenses));
    lineseries->append(QPoint(6, sunExpenses));

    // add points for the monthly line chart to plot
    yearlyLineSeries->setName("YearlyExpense Trend");
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

    // create chart, add data and a title
    QChart *chart = new QChart();
    chart->addSeries(barseries);
    chart->addSeries(lineseries);
    chart->setTitle("Line and Bar Chart of Weekly Expenses");

    // add categories for each of the bars
    QStringList categories;
    categories << "Mon" << "Tues" << "Wed" << "Thurs" << "Fri" << "Sat" << "Sun";

    // create and format x-axis with categories
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->setAxisX(axisX, lineseries);
    chart->setAxisX(axisX, barseries);
    axisX->setRange(QString("Mon"), QString("Sat"));
    axisX->setTitleText("Day of the Week");

    // create and format y-axis
    QValueAxis *axisY = new QValueAxis();
    chart->setAxisY(axisY, lineseries);
    chart->setAxisY(axisY, barseries);
    axisY->setRange(0, maxWeeklyVal + 100);
    axisY->setTitleText("Exepensed Amount ($)");

    // add a legend to the plot
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // create a widget view of the chart and resize accoringly
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedSize(10000, 7000);

    // add the chart to the layout
    ui->verticalLayout_5->setMargin(0);
    ui->verticalLayout_5->addWidget(chartView);

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
}

// BudgetTarget destructor
BudgetTarget::~BudgetTarget() {
    delete ui;
}


void BudgetTarget::AddRoot(QString Category, QString Amount) {
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    itm->setText(2,Amount);
    ui->treeWidget->addTopLevelItem(itm);
}

void BudgetTarget::AddRoot_2(QString Category, QString Amount) {
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_3);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    itm->setText(2,Amount);
    ui->treeWidget_3->addTopLevelItem(itm);
}

void BudgetTarget::AddRoot_3(QString Category, QString Amount) {
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_6);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    itm->setText(2,Amount);
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

void BudgetTarget::EditRoot(QString Amount) {
    QString before = ui->treeWidget->currentItem()->text(1);
    QString before2 = ui->treeWidget->currentItem()->text(2);
    int diff = Amount.toInt()-before.toInt();
    int update = before2.toInt() + diff;
    ui->treeWidget->currentItem()->setData(1,Qt::DisplayRole,Amount);
    ui->treeWidget->currentItem()->setData(2,Qt::DisplayRole, update);
}

void BudgetTarget::EditRoot_2(QString Amount) {
    QString before = ui->treeWidget_3->currentItem()->text(1);
    QString before2 = ui->treeWidget_3->currentItem()->text(2);
    int diff = Amount.toInt()-before.toInt();
    int update = before2.toInt() + diff;
    ui->treeWidget_3->currentItem()->setData(1,Qt::DisplayRole,Amount);
    ui->treeWidget_3->currentItem()->setData(2,Qt::DisplayRole, update);
}

void BudgetTarget::EditRoot_3(QString Amount) {
    QString before = ui->treeWidget_6->currentItem()->text(1);
    QString before2 = ui->treeWidget_6->currentItem()->text(2);
    int diff = Amount.toInt()-before.toInt();
    int update = before2.toInt() + diff;
    ui->treeWidget_6->currentItem()->setData(1,Qt::DisplayRole,Amount);
    ui->treeWidget_6->currentItem()->setData(2,Qt::DisplayRole, update);
}

void BudgetTarget::on_Edit_clicked() {
    if(ui->treeWidget->currentItem()!=nullptr){
        AddDialog adddialog1;
        adddialog1.setModal(true);
        adddialog1.exec();
        QString amount = adddialog1.returnAmo();
        if(amount!=nullptr){
            EditRoot(amount);
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
            EditRoot_2(amount);
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
            EditRoot_3(amount);
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
    *daysOfWeek << monExpenses << tuesExpenses << wedExpenses << thursExpenses << friExpenses << satExpenses << sunExpenses;
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
    }
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
    }
}
void BudgetTarget::on_DeleteE_clicked(){
    if(ui->treeWidget_2->currentItem()!=nullptr){

    QModelIndex index = ui->treeWidget_2->currentIndex();
    QString category = ui->treeWidget_2->currentItem()->text(0);
    int cateIndex = cateList.indexOf(category);
    QString before = ui->treeWidget->topLevelItem(cateIndex)->text(2);
    QString add = ui->treeWidget_2->currentItem()->text(1);
    int update = before.toInt()+add.toInt();
    ui->treeWidget->topLevelItem(cateIndex)->setData(2,Qt::DisplayRole,update);
    ui->treeWidget_2->takeTopLevelItem(index.row());
    }
}

void BudgetTarget::on_DeleteE_2_clicked(){
    if(ui->treeWidget_4->currentItem()!=nullptr){

    QModelIndex index = ui->treeWidget_4->currentIndex();
    QString category = ui->treeWidget_4->currentItem()->text(0);
    int cateIndex = cateList.indexOf(category);
    QString before = ui->treeWidget_3->topLevelItem(cateIndex)->text(2);
    QString add = ui->treeWidget_4->currentItem()->text(1);
    int update = before.toInt()+add.toInt();
    ui->treeWidget_3->topLevelItem(cateIndex)->setData(2,Qt::DisplayRole,update);
    ui->treeWidget_4->takeTopLevelItem(index.row());
    }
}
void BudgetTarget::on_DeleteE_3_clicked(){
    if(ui->treeWidget_7->currentItem()!=nullptr){
    QModelIndex index = ui->treeWidget_7->currentIndex();
    QString category = ui->treeWidget_7->currentItem()->text(0);
    int cateIndex = cateListY.indexOf(category);
    QString before = ui->treeWidget_6->topLevelItem(cateIndex)->text(2);
    QString add = ui->treeWidget_7->currentItem()->text(1);
    int update = before.toInt()+add.toInt();
    ui->treeWidget_6->topLevelItem(cateIndex)->setData(2,Qt::DisplayRole,update);
    ui->treeWidget_7->takeTopLevelItem(index.row());
    }
}
