#include "asset.h"
#include "ui_asset.h"
#include "popup.h"
#include <QtCore>
#include <QtGui>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>



Asset::Asset(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Asset)
{

    ui->setupUi(this);

    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setHeaderLabels(QStringList() << "Name" << "Value ($)");

    ui->treeWidget_LB->setColumnCount(2);
    ui->treeWidget_LB->setHeaderLabels(QStringList() << "Name" << "Value ($)");

     QSqlQuery query;

     //Loading Data into Tables - ASSETS
     query.prepare("select ID, AssetName, AssetValue from networthasset");
     if(query.exec()){
         qDebug() << "Data is loaded";
     }
     else{
         qDebug() << "Data is not loaded";
     }
     while(query.next()){
         Asset::idVecAsset << query.value(0).toInt();
         Asset::sumVector << query.value(2).toInt();
         Asset::assetName << query.value(1).toString();

         QString name = query.value(1).toString();
         QString value = query.value(2).toString();

         AddRoot(name,value);
     }
     if(query.exec()){
         qDebug() << "Successful for Assets";
     }
     else{
         qDebug() << "Not Successful for Assets";
     }

     //------------------
     //Loading Data into Tables - Liabilities


     query.prepare("select ID, liabilityName, liabilityValue from networthliability");
     while(query.next()){
         Asset::idVecLiability << query.value(0).toInt();
         Asset::totalLiability << query.value(1).toInt();
         Asset::LiabilityName << query.value(2).toString();

         QString name = query.value(1).toString();
         QString value = query.value(2).toString();

         AddLiability(name,value);
     }
     if(query.exec()){
         qDebug() << "Successful for Liabilities";
     }
     else{
         qDebug() << "Not Successful for Liabilities";
     }
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

    //Create Assets Table for database
    QSqlQuery query;
    query.exec("CREATE TABLE networthasset(ID integer, assetName varchar(20) , assetValue integer, sum integer)");   //make ID primary key?
    qDebug() << query.lastError() << endl;

    QString assetV = popup.popup::returnValue();
    QString assetN = popup.popup::returnAsset();


    long assetNum = assetV.toLong();
    bool isNumeric = false;
    assetV.toLong(&isNumeric);

    if(isNumeric == true){
    Asset::sumVector << assetNum;
    Asset::idVecAsset << Asset::idNumAsset;
    idNumAsset++;
    }

    Asset::sum = 0;
    for(int i = 0; i < sumVector.size(); i++){
       Asset::sum += sumVector[i];
    }

    //SQL CODE
    //Creating a unique ID for each item
    int id = idNumAsset - 1;    //idNumAsset is incremented after value is added to list
    QString idValue = QString::number(id);

    if(isNumeric == false){
       QMessageBox::information(this,"Invalid Inputs", "Please Enter Valid inputs");
    }
    else if(assetN == nullptr || assetV == nullptr){
        QMessageBox::information(this,"Invalid Inputs", "Please Enter Valid inputs");
    }
    else{
      AddRoot(assetN,assetV);
      query.prepare("INSERT INTO networthasset(ID, AssetName, AssetValue, sum) VALUES ('"+idValue+"','"+assetN+"','"+assetV+"','"+sum+"')");
      qDebug() << query.lastError() << endl;
     if(query.exec()){
      //   QMessageBox::information(this,tr("Save"), tr("Data is saved"));
         qDebug() << "Data is saved";
     }
     else{
      //   QMessageBox::critical(this,tr("ERROR"), tr("Data is not saved"));
          qDebug() << "Data is not saved";

     }

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

    QSqlQuery query; //SQL CODE

    QModelIndex index = ui->treeWidget->currentIndex();
    ui->treeWidget->takeTopLevelItem(index.row());
    Asset::sumVector.remove(index.row());

    //SQL CODE
    int id = idVecAsset[index.row()];   //Getting ID of item being deleted
    QString idValue = QString::number(id);

    Asset::idVecAsset.remove(index.row());  //SQL CODE

    //SQL CODE
    query.prepare("Delete from networthasset where ID = '"+idValue+"'");
    if(query.exec()){
        qDebug() << "Deleted from Database";
    }
    else{
         qDebug() << "Not deleted from Database";
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

void Asset::on_pushButton_3_clicked() //EDITING ASSETS
{
  if(ui->treeWidget->currentItem() != nullptr){        //Checks if item selected
    popup popup;
    popup.setModal(true);
    popup.exec();

    QSqlQuery query;

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

        //ID at where the index is that is selected
        int id = Asset::idVecAsset[index.row()];
        QString idValue = QString::number(id);

        if(isNumeric == false){
           QMessageBox::information(this,"Invalid Inputs", "Please Enter Valid inputs");
        }
        else if(assetN == nullptr || assetV == nullptr){
            QMessageBox::information(this,"Invalid Inputs", "Please Enter Valid inputs");
        }
        else{
            EditRoot(index.row(),assetN,assetV);
            query.prepare("update networthasset set AssetName='"+assetN+"', AssetValue'"+assetV+"' where ID='"+idValue+"'");
            if(query.exec()){
                qDebug() << "Edited from the table";
            }
            else{
                qDebug() << "Not edited from the table";
            }
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

    //SQL CODE
    //Create Liability Table for database
    QSqlQuery query;
    query.exec("CREATE TABLE networthliability(ID integer, liabilityName varchar(20) , liabilityValue integer, liabilitysum integer)");

    QString assetN = popup.popup::returnAsset();
    QString assetV = popup.popup::returnValue();

    long liabilityNum = assetV.toLong();
    bool isNumeric = false;
    assetV.toLong(&isNumeric);

    if(isNumeric == true){
    Asset::LiabilityVector << liabilityNum;
    Asset::idVecLiability << Asset::idNumLiability; //SQL CODE
    idNumLiability++;   //SQL CODE
    }

    //SQL CODE
    //Creating a unique ID for each item
    int id = idNumLiability - 1;    //idNumAsset is incremented after value is added to list
    QString idValue = QString::number(id);


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

      //SQL CODE
      query.prepare("INSERT INTO networthliability(ID,liabilityName, liabilityValue, liabilitysum) VALUES ('"+idValue+"','"+assetN+"','"+assetV+"','"+totalLiability+"')");
      qDebug() << query.lastError() << endl;
      if(query.exec()){
       //   QMessageBox::information(this,tr("Save"), tr("Data is saved"));
          qDebug() << "Data is saved";
      }
      else{
       //   QMessageBox::critical(this,tr("ERROR"), tr("Data is not saved"));
           qDebug() << "Data is not saved";

      }
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
    QSqlQuery query;    //SQL CODE

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

    //SQL CODE
    //ID at where the index is that is selected
    int id = Asset::idVecLiability[index.row()];
    QString idValue = QString::number(id);

    if(isNumeric == false){
       QMessageBox::information(this,"Invalid Inputs", "Please Enter Valid inputs");
    }
    else if(assetN == nullptr || assetV == nullptr){
        QMessageBox::information(this,"Invalid Inputs", "Please Enter Valid inputs");
    }
    else{
        EditRoot2(index.row(),assetN,assetV);

        //SQL CODE
        query.prepare("update networthasset set liabilityName='"+assetN+"', liabilityValue'"+assetV+"' where ID='"+idValue+"'");
        if(query.exec()){
            qDebug() << "Edited from the table";
        }
        else{
            qDebug() << "Not edited from the table";
        }
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

    QSqlQuery query; //SQL CODE

    QModelIndex index = ui->treeWidget_LB->currentIndex();
    ui->treeWidget_LB->takeTopLevelItem(index.row());

    Asset::LiabilityVector.remove(index.row());

    int id = idVecLiability[index.row()];   //SQL CODE getting the ID of the item being deleted
    QString idValue = QString::number(id);  //SQL CODE
    Asset::idVecLiability.remove(index.row());  //SQL CODE to delete the ID from the vector holding IDs


    Asset::totalLiability = 0;

    query.prepare("Delete from networthliability where ID = '"+idValue+"'");
    if(query.exec()){
        qDebug() << "Deleted Item from Database";
    }
    else {
       qDebug() << "Not Deleted";
    }

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
