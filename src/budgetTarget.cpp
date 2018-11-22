#include "budgettarget.h"
#include "ui_budgettarget.h"
#include "addexpense.h"
#include "popup_window.h"
#include "ui_popup_window.h"
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
    ui->treeWidget->setColumnCount(2);

    ui->lineEdit_4->setReadOnly(true);
}

BudgetTarget::~BudgetTarget()
{
    delete ui;
}

void BudgetTarget::on_pushButton_4_clicked()
{
   addExpense expense;
   expense.setModal(true);
   expense.exec();
}

void BudgetTarget::on_Add_clicked()
{
    AddDialog adddialog;
    adddialog.setModal(true);
    adddialog.exec();
    QString cate = adddialog.returnVal();
    QString amount = adddialog.returnAmo();
    if(cate!=nullptr and amount!=nullptr){
        AddRoot(cate, amount);
        cateList<<cate;
        amountList<<amount.toInt();

    }

}



void BudgetTarget::AddRoot(QString Category, QString Amount)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    ui->treeWidget->addTopLevelItem(itm);
}

void BudgetTarget::EditRoot(int index, QString Category, QString Amount)
{

    ui->treeWidget->currentItem()->setData(0,Qt::DisplayRole,Category);
    ui->treeWidget->currentItem()->setData(1,Qt::DisplayRole,Amount);
    cateList[index] = Category;
    amountList[index] = Amount.toInt();
}


void BudgetTarget::on_Delete_clicked()
{

    QModelIndex index = ui->treeWidget->currentIndex();
    ui->treeWidget->takeTopLevelItem(index.row());
    amountList.removeAt(index.row());
    cateList.removeAt(index.row());
}

void BudgetTarget::on_Edit_clicked()
{
    if(ui->treeWidget->currentItem()!=nullptr){
        AddDialog adddialog1;
        adddialog1.setModal(true);
        adddialog1.exec();
        QString cate = adddialog1.returnVal();
        QString amount = adddialog1.returnAmo();
        QModelIndex index = ui->treeWidget->currentIndex();

        if(cate!=nullptr and amount!=nullptr){
            EditRoot(index.row(),cate, amount);
            cateList<<cate;
            amountList<<amount.toInt();

        }
    }


}
