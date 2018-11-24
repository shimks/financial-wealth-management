#include "popup.h"
#include "ui_popup.h"
#include <QMessageBox>

popup::popup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::popup)
{
    ui->setupUi(this);
}

popup::~popup()
{
    delete ui;
}

void popup::on_pushButton_clicked()
{
    QString name = ui->lineEdit->text();
    QString value = ui->lineEdit_2->text();

    popup::assetName = name;
    popup::assetValue = value;
}

QString popup::returnAsset(){
    QString rA = popup::assetName;
    return rA;
}

QString popup::returnValue(){
    QString rV = popup::assetValue;
    return rV;
}

