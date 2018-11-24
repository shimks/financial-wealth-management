#include "addexpense.h"
#include "ui_addexpense.h"
#include <QMessageBox>

addExpense::addExpense(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addExpense)
{
    ui->setupUi(this);
}

addExpense::~addExpense()
{
    delete ui;
}
int addExpense::returnVal(){

    return addExpense::category;

}

QString addExpense::returnAmo(){
    return addExpense::amount;
}

int addExpense::returnDay(){
    return addExpense::day;
}

void addExpense::on_pushButton_clicked()
{
    addExpense::category = ui->comboBox->currentIndex();
    addExpense::amount = ui->textEdit->toPlainText();
    addExpense::day = ui->comboBox_2->currentIndex();
    if(amount==nullptr){
        QMessageBox::information(this, tr("Error"),tr("Invalid Input"));
    }
    else{
        addExpense::close();
    }

}

void addExpense::on_pushButton_2_clicked()
{
    addExpense::category = 0;
    addExpense::amount = nullptr;
    addExpense::day = 0;
    addExpense::close();

}
