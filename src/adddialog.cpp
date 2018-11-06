#include "adddialog.h"
#include "ui_adddialog.h"

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
    AddDialog::categroy = ui->textEdit->toPlainText();
    QString amount = ui->textEdit_2->toPlainText();
    AddDialog::close();


}



QString AddDialog::returnVal(){
    return AddDialog::categroy;

}

void AddDialog::on_pushButton_2_clicked()
{
    AddDialog::close();
}
