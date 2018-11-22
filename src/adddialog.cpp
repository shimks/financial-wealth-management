#include "adddialog.h"
#include "ui_adddialog.h"
#include <QMessageBox>



AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);

}

AddDialog::~AddDialog()
{
    delete ui;
}


void AddDialog::on_pushButton_clicked()
{
    AddDialog::category = ui->textEdit->toPlainText();
    AddDialog::amount = ui->textEdit_2->toPlainText();
    if(category==nullptr or amount==nullptr){
        QMessageBox::information(this, tr("Error"),tr("Invalid Input"));
    }
    AddDialog::close();


}



QString AddDialog::returnVal(){
    return AddDialog::category;

}

QString AddDialog::returnAmo(){
    return AddDialog::amount;
}



void AddDialog::on_pushButton_2_clicked()
{
    AddDialog::close();
}


