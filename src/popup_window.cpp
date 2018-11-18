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


popup_window::popup_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::popup_window)
{

    ui->setupUi(this);
    ui->treeWidget->setColumnCount(2);
}

popup_window::~popup_window()
{
    delete ui;
}

void popup_window::on_Show_clicked()
{
    //QString cate = adddialog.AddDialog::returnVal();
    QMessageBox::information(this,tr("info"),tr(""));

}

void popup_window::on_Add_clicked()
{
    AddDialog adddialog;
    adddialog.setModal(true);
    adddialog.exec();
    QString cate = adddialog.AddDialog::returnVal();
    popup_window::cateList<<cate;
    QString amount = adddialog.AddDialog::returnAmo();
    popup_window::amountList<<amount.toInt();
    if(cate!=0){
        AddRoot(cate, amount);
    }
    //qDebug() << ui->treeWidget-> << endl;
}


void popup_window::AddRoot(QString Category, QString Amount)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,Category);
    itm->setText(1,Amount);
    ui->treeWidget->addTopLevelItem(itm);
}

void popup_window::EditRoot(int index, QString Category, QString Amount)
{

    ui->treeWidget->currentItem()->setData(0,Qt::DisplayRole,Category);
    ui->treeWidget->currentItem()->setData(1,Qt::DisplayRole,Amount);
    cateList[index] = Category;
    amountList[index] = Amount.toInt();
    //qDebug() << index << endl;
}


void popup_window::on_pushButton_clicked()
{

    QModelIndex index = ui->treeWidget->currentIndex();
    ui->treeWidget->takeTopLevelItem(index.row());
    amountList.removeAt(index.row());
    cateList.removeAt(index.row());
}

void popup_window::on_pushButton_2_clicked()
{
    AddDialog adddialog1;
    adddialog1.setModal(true);
    adddialog1.exec();
    QString cate = adddialog1.AddDialog::returnVal();
    popup_window::cateList<<cate;
    QString amount = adddialog1.AddDialog::returnAmo();
    popup_window::amountList<<amount.toInt();
    QModelIndex index = ui->treeWidget->currentIndex();

    if(cate!=0){
        EditRoot(index.row(),cate, amount);
    }


}
