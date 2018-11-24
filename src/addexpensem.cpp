#include "addexpensem.h"
#include "ui_addexpensem.h"
#include <QMessageBox>

addExpenseM::addExpenseM(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addExpenseM)
{
    ui->setupUi(this);
}

addExpenseM::~addExpenseM()
{
    delete ui;
}



int addExpenseM::returnVal(){

    return addExpenseM::category;

}

QString addExpenseM::returnAmo(){
    return addExpenseM::amount;
}

QString addExpenseM::returnDay(){
    return addExpenseM::day;
}

void addExpenseM::on_pushButton_clicked()
{
    addExpenseM::category = ui->comboBox->currentIndex();
    addExpenseM::amount = ui->textEdit->toPlainText();
    addExpenseM::day = ui->textEdit_2->toPlainText();
    if(amount==nullptr){
        QMessageBox::information(this, tr("Error"),tr("Invalid Input"));
    }
    else{
        addExpenseM::close();
    }
}

void addExpenseM::on_pushButton_2_clicked()
{
    addExpenseM::category = 0;
    addExpenseM::amount = nullptr;
    addExpenseM::day = nullptr;
    addExpenseM::close();

}
