#include "asset.h"
#include "ui_asset.h"
#include "popup.h"
#include <QtCore>
#include <QtGui>
#include <QTreeWidgetItem>
#include <QMessageBox>

Asset::Asset(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Asset)
{

    ui->setupUi(this);
    /*
    QListWidgetItem *item = new QListWidgetItem(ui->lineEdit->text());
    ui->listWidget->addItem(item);
    */

    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setHeaderLabels(QStringList() << "Name" << "Value ($)");

    ui->treeWidget_LB->setColumnCount(2);
    ui->treeWidget_LB->setHeaderLabels(QStringList() << "Name" << "Value ($)");

}

Asset::~Asset()
{
    delete ui;
}

void Asset::on_pushButton_clicked() //ADDING ASSET
{
    popup popup;
    popup.setModal(true);
    popup.exec();

    QString assetV = popup.popup::returnValue();
    QString assetN = popup.popup::returnAsset();


    long assetNum = assetV.toLong();
    bool isNumeric = false;
    assetV.toLong(&isNumeric);

    if(isNumeric == true){
    Asset::sumVector << assetNum;
    }

    Asset::sum = 0;
    for(int i = 0; i < sumVector.size(); i++){
       Asset::sum += sumVector[i];
    }



    if(isNumeric == false){
       QMessageBox::information(this,"Invalid Inputs", "Please Enter Valid inputs");
    }
    else if(assetN == nullptr || assetV == nullptr){
        QMessageBox::information(this,"Invalid Inputs", "Please Enter Valid inputs");
    }
    else{
      AddRoot(assetN,assetV);
    }

    QString s = QString::number(sum);
    ui->label_7->setText(s);

    //Setting the value for total networth
    long total = sum - totalLiability;
    QString t = QString::number(total);
    ui->label_11->setText(t);
}


void Asset::AddRoot(QString name, QString value){
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,name);
    itm->setText(1,value);
    ui->treeWidget->addTopLevelItem(itm);

}


void Asset::AddLiability(QString name, QString value){
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget_LB);
    itm->setText(0,name);
    itm->setText(1,value);
    ui->treeWidget_LB->addTopLevelItem(itm);

}


void Asset::EditRoot(int index, QString Name, QString Description){

    ui->treeWidget->currentItem()->setData(0,Qt::DisplayRole, Name);
    ui->treeWidget->currentItem()->setData(1,Qt::DisplayRole, Description);
}

void Asset::EditRoot2(int index, QString Name, QString Description){

    ui->treeWidget_LB->currentItem()->setData(0,Qt::DisplayRole, Name);
    ui->treeWidget_LB->currentItem()->setData(1,Qt::DisplayRole, Description);
}

void Asset::on_pushButton_2_clicked() //DELETING ASSET
{
if(ui->treeWidget->currentItem() != nullptr){   //Checks if item selected

    QModelIndex index = ui->treeWidget->currentIndex();
    ui->treeWidget->takeTopLevelItem(index.row());
    Asset::sumVector.remove(index.row());
    Asset::sum = 0;
    for(int i = 0; i < sumVector.size(); i++){
           Asset::sum += sumVector[i];
        }
    QString s = QString::number(sum);
    ui->label_7->setText(s);

    //Updating total Networth
    long total = sum - totalLiability;
    QString t = QString::number(total);
    ui->label_11->setText(t);
    }
}

void Asset::on_pushButton_3_clicked() //EDITING ASSETS
{
  if(ui->treeWidget->currentItem() != nullptr){        //Checks if item selected
    popup popup;
    popup.setModal(true);
    popup.exec();


    QString assetN = popup.popup::returnAsset();
    QString assetV = popup.popup::returnValue();
    QModelIndex index = ui->treeWidget->currentIndex();

        long insertVal = assetV.toLong();
        bool isNumeric = false;
        assetV.toLong(&isNumeric);


        if(isNumeric == true){
        Asset::sumVector.remove(index.row());
        Asset::sumVector.insert(index.row(),insertVal);
        }

        if(isNumeric == false){
           QMessageBox::information(this,"Invalid Inputs", "Please Enter Valid inputs");
        }
        else if(assetN == nullptr || assetV == nullptr){
            QMessageBox::information(this,"Invalid Inputs", "Please Enter Valid inputs");
        }
        else{
            EditRoot(index.row(),assetN,assetV);
        }

        Asset::sum = 0;
        for(int i = 0; i < sumVector.size(); i++){
               Asset::sum += sumVector[i];
            }
        QString s = QString::number(sum);
        ui->label_7->setText(s);

        //Updating total Networth
        long total = sum - totalLiability;
        QString t = QString::number(total);
        ui->label_11->setText(t);
    }
}

void Asset::on_pushButton_addLB_clicked()  //ADDING LIABILITIES
{
    popup popup;
    popup.setModal(true);
    popup.exec();

    QString assetN = popup.popup::returnAsset();
    QString assetV = popup.popup::returnValue();

    long liabilityNum = assetV.toLong();
    bool isNumeric = false;
    assetV.toLong(&isNumeric);

    if(isNumeric == true){
    Asset::LiabilityVector << liabilityNum;
    }


    Asset::totalLiability = 0;
    for(int i = 0; i < LiabilityVector.size(); i++){
       Asset::totalLiability = LiabilityVector[i] + totalLiability;
    }
    if(isNumeric == false){
       QMessageBox::information(this,"Invalid Inputs", "Please Enter Valid inputs");
    }
    else if(assetN == nullptr || assetV == nullptr){
        QMessageBox::information(this,"Invalid Inputs", "Please Enter Valid inputs");
    }
    else{
      AddLiability(assetN,assetV);
    }

    QString s = QString::number(totalLiability);
    ui->label_8->setText(s);

    //Updating total Networth
    long total = sum - totalLiability;
    QString t = QString::number(total);
    ui->label_11->setText(t);
}

void Asset::on_pushButton_LBEdit_clicked() //EDIT LIABILITIES
{
    if(ui->treeWidget_LB->currentItem() != nullptr){       //Checks if item selected
    popup popup;
    popup.setModal(true);
    popup.exec();

    QString assetN = popup.popup::returnAsset();
    QString assetV = popup.popup::returnValue();
    QModelIndex index = ui->treeWidget_LB->currentIndex();

    long insertVal = assetV.toLong();
    bool isNumeric = false;
    assetV.toLong(&isNumeric);

    if(isNumeric == true){
    Asset::LiabilityVector.remove(index.row());
    Asset::LiabilityVector.insert(index.row(),insertVal);
    }


    if(isNumeric == false){
       QMessageBox::information(this,"Invalid Inputs", "Please Enter Valid inputs");
    }
    else if(assetN == nullptr || assetV == nullptr){
        QMessageBox::information(this,"Invalid Inputs", "Please Enter Valid inputs");
    }
    else{
        EditRoot2(index.row(),assetN,assetV);
    }

    Asset::totalLiability = 0;
    for(int i = 0; i < LiabilityVector.size(); i++){
           Asset::totalLiability += LiabilityVector[i];
        }

    QString s = QString::number(totalLiability);
    ui->label_8->setText(s);

    //Ppdating total Networth
    long total = sum - totalLiability;
    QString t = QString::number(total);
    ui->label_11->setText(t);
    }
}
void Asset::on_pushButton_LBDelete_clicked()    //DELETING LIABILITIES
{

if(ui->treeWidget_LB->currentItem() != nullptr){   //Checks if item selected


    QModelIndex index = ui->treeWidget_LB->currentIndex();
    ui->treeWidget_LB->takeTopLevelItem(index.row());

    Asset::LiabilityVector.remove(index.row());
    Asset::totalLiability = 0;

    for(int i = 0; i < LiabilityVector.size(); i++){
           Asset::totalLiability += LiabilityVector[i];
        }
    QString s = QString::number(totalLiability);
    ui->label_8->setText(s);

    //Updating total Networth
    long total = sum - totalLiability;
    QString t = QString::number(total);
    ui->label_11->setText(t);
    }
}
