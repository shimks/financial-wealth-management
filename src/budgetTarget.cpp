#include "budgettarget.h"
#include "ui_budgettarget.h"
#include "addexpense.h"
#include "ui_popup_window.h"
#include "popup_window.h"
#include <QMessageBox>
#include "adddialog.h"
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
    AddRoot("Grocery","0");
    AddRoot("Clothes","0");
    AddRoot("Others","0");
    cateList<<"Grocery";
    cateList<<"Clothes";
    cateList<<"Others";
    dayList<<"Monday";
    dayList<<"Tuesday";
    dayList<<"Wednesday";
    dayList<<"Thursday";
    dayList<<"Friday";
    dayList<<"Saturday";
    dayList<<"Sunday";



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


void BudgetTarget::AddExpense(QString Category, QString Amount, QString Day)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_2);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    itm->setText(2,Day);
    ui->treeWidget_2->addTopLevelItem(itm);
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
/*
void BudgetTarget::EditExpense(QString Category, QString Amount, QString Day)
{
    ui->treeWidget_2->currentItem()->setData(0,Qt::DisplayRole,Category);
    ui->treeWidget_2->currentItem()->setData(1,Qt::DisplayRole,Amount);
    ui->treeWidget_2->currentItem()->setData(2,Qt::DisplayRole,Day);

}*/

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

void BudgetTarget::on_AddE_clicked(){
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
/*
void BudgetTarget::on_EditE_clicked(){
    if(ui->treeWidget_2->currentItem()!=nullptr){
        QString before = ui->treeWidget_2->currentItem()->text(1);
        addExpense addexpense;
        addexpense.setModal(true);
        addexpense.exec();
        QString amount = addexpense.returnAmo();
        QString category = cateList[addexpense.returnVal()];
        QString day = dayList[addexpense.returnDay()];
        if(amount!=nullptr){
            int beforeIndex =ui->treeWidget_2->currentIndex().row();
            int update =ui->treeWidget->topLevelItem(beforeIndex)->text(2).toInt();
            update= before.toInt() + update;
            ui->treeWidget->topLevelItem(beforeIndex)->setData(2,Qt::DisplayRole,update);

            int cateIndex = cateList.indexOf(category);
            QString before2 = ui->treeWidget->topLevelItem(cateIndex)->text(2);
            int update2 = before2.toInt() - amount.toInt();
            EditExpense(category, amount, day);
            ui->treeWidget->topLevelItem(cateIndex)->setData(2,Qt::DisplayRole,update2);
        }
    }

}*/
