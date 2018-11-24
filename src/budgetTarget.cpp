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
#include <QTreeWidget>
#include <QtCore>
#include <QtGui>
#include <iostream>
#include <QDebug>

BudgetTarget::BudgetTarget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BudgetTarget)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(3);
    ui->treeWidget_2->setColumnCount(3);
    AddRoot("Groceries","0");
    AddRoot("Hygiene Products","0");
    AddRoot("Transportation","0");
    AddRoot("Personal","0");
    AddRoot("Food","0");
    AddRoot("School Supplies","0");

    AddRoot_2("Phone Bill","0");
    AddRoot_2("Utility Bill","0");
    AddRoot_2("Rent/Leas/Morgage","0");
    AddRoot_2("Car Lease/Loan","0");
    AddRoot_2("Credit Expenses","0");
    AddRoot_2("Savings","0");

    AddRoot_3("Life Insurance","0");
    AddRoot_3("Car Insurnce","0");
    AddRoot_3("Health Insurance","0");
    AddRoot_3("Gifts","0");
    AddRoot_3("Clothing","0");
    AddRoot_3("Home/Renovations","0");
    AddRoot_3("Student Loans","0");
    AddRoot_3("Tuition Fee","0");

    cateList<<"Groceries";
    cateList<<"Hyginen Products";
    cateList<<"Transportation";
    cateList<<"Personal";
    cateList<<"Food";
    cateList<<"School Supplies";

    cateListM<<"Phone Bill";
    cateListM<<"Utility Bill";
    cateListM<<"Rent/Lease/Loan";
    cateListM<<"Car Lease/Loan";
    cateListM<<"Credit Expenses";
    cateListM<<"Savings";

    dayList<<"Monday";
    dayList<<"Tuesday";
    dayList<<"Wednesday";
    dayList<<"Thursday";
    dayList<<"Friday";
    dayList<<"Saturday";
    dayList<<"Sunday";

    cateListY<<"Life Insurance";
    cateListY<<"Car Insurance";
    cateListY<<"Health Insurance";
    cateListY<<"Gifts";
    cateListY<<"Clothing";
    cateListY<<"Home/Renovations";
    cateListY<<"Student Loans";
    cateListY<<"Tuition Fee";

    dayListY<<"January";
    dayListY<<"February";
    dayListY<<"March";
    dayListY<<"April";
    dayListY<<"May";
    dayListY<<"June";
    dayListY<<"July";
    dayListY<<"August";
    dayListY<<"September";
    dayListY<<"October";
    dayListY<<"November";
    dayListY<<"December";




}

BudgetTarget::~BudgetTarget()
{
    delete ui;
}


void BudgetTarget::AddRoot(QString Category, QString Amount)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    itm->setText(2,Amount);
    ui->treeWidget->addTopLevelItem(itm);
}
void BudgetTarget::AddRoot_2(QString Category, QString Amount)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_3);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    itm->setText(2,Amount);
    ui->treeWidget_3->addTopLevelItem(itm);
}
void BudgetTarget::AddRoot_3(QString Category, QString Amount)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_6);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    itm->setText(2,Amount);
    ui->treeWidget_7->addTopLevelItem(itm);
}

void BudgetTarget::AddExpense(QString Category, QString Amount, QString Day)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_2);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    itm->setText(2,Day);
    ui->treeWidget_2->addTopLevelItem(itm);
}
void BudgetTarget::AddExpense_2(QString Category, QString Amount, QString Day)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_4);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    itm->setText(2,Day);
    ui->treeWidget_4->addTopLevelItem(itm);
}
void BudgetTarget::AddExpense_3(QString Category, QString Amount, QString Day)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_7);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    itm->setText(2,Day);
    ui->treeWidget_7->addTopLevelItem(itm);
}
void BudgetTarget::EditRoot(QString Amount)
{
    QString before = ui->treeWidget->currentItem()->text(1);
    QString before2 = ui->treeWidget->currentItem()->text(2);
    int diff = Amount.toInt()-before.toInt();
    int update = before2.toInt() + diff;
    ui->treeWidget->currentItem()->setData(1,Qt::DisplayRole,Amount);
    ui->treeWidget->currentItem()->setData(2,Qt::DisplayRole, update);
}
void BudgetTarget::EditRoot_2(QString Amount)
{
    QString before = ui->treeWidget_3->currentItem()->text(1);
    QString before2 = ui->treeWidget_3->currentItem()->text(2);
    int diff = Amount.toInt()-before.toInt();
    int update = before2.toInt() + diff;
    ui->treeWidget_3->currentItem()->setData(1,Qt::DisplayRole,Amount);
    ui->treeWidget_3->currentItem()->setData(2,Qt::DisplayRole, update);
}
void BudgetTarget::EditRoot_3(QString Amount)
{
    QString before = ui->treeWidget_6->currentItem()->text(1);
    QString before2 = ui->treeWidget_6->currentItem()->text(2);
    int diff = Amount.toInt()-before.toInt();
    int update = before2.toInt() + diff;
    ui->treeWidget_6->currentItem()->setData(1,Qt::DisplayRole,Amount);
    ui->treeWidget_6->currentItem()->setData(2,Qt::DisplayRole, update);
}
void BudgetTarget::on_Edit_clicked()
{
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
void BudgetTarget::on_Edit_2_clicked()
{
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

void BudgetTarget::on_AddE_clicked(){
    addExpense addexpense;
    addexpense.setModal(true);
    addexpense.exec();
    QString cate = cateList[addexpense.returnVal()];
    QString amount = addexpense.returnAmo();
    QString day = dayList[addexpense.returnDay()];
    if(amount!=nullptr){
        AddExpense_3(cate, amount,day);
        QString before = ui->treeWidget->topLevelItem(addexpense.returnVal())->text(2);
        int update = before.toInt()-amount.toInt();
        ui->treeWidget->topLevelItem(addexpense.returnVal())->setData(2,Qt::DisplayRole,update);
    }
}

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
