#include "addexpensey.h"
#include "ui_addexpensey.h"
#include <QMessageBox>

addExpenseY::addExpenseY(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addExpenseY)
{
    ui->setupUi(this);
}

addExpenseY::~addExpenseY()
{
    delete ui;
}

int addExpenseY::returnVal(){

    return addExpenseY::category;

}

QString addExpenseY::returnAmo(){
    return addExpenseY::amount;
}

int addExpenseY::returnDay(){
    return addExpenseY::day;
}

void addExpenseY::on_pushButton_3_clicked()
{
    addExpenseY::category = ui->comboBox_3->currentIndex();
    addExpenseY::amount = ui->textEdit_2->toPlainText();
    addExpenseY::day = ui->comboBox_4->currentIndex();
    if(amount==nullptr){
        QMessageBox::information(this, tr("Error"),tr("Invalid Input"));
    }
    else{
        addExpenseY::close();
    }
}

void addExpenseY::on_pushButton_4_clicked()
{
    addExpenseY::category = 0;
    addExpenseY::amount = nullptr;
    addExpenseY::day = 0;
    addExpenseY::close();

}
