#ifndef ASSET_H
#define ASSET_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QTreeWidgetItem>
#include <QVector>
#include <QtSql>
#include <QtDebug>

namespace Ui {
class Asset;
}

class Asset : public QWidget
{
    Q_OBJECT

    void AddRoot(QString name, QString Description);
    void AddLiability(QString name, QString Description);
    void EditRoot(int index, QString Name, QString Description);
    void EditRoot2(int index, QString Name, QString Description);

public:
    explicit Asset(QWidget *parent = nullptr);
    ~Asset();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_addLB_clicked();

    void on_pushButton_LBEdit_clicked();

    void on_pushButton_LBDelete_clicked();

private:
    Ui::Asset *ui;

    //Global Variable
     QVector<int> idVecAsset;   //Track the ID for Asset
     int idNumAsset = 0;
     QVector<long> sumVector;   //Tracks all Asset Values
     QVector<QString> assetName;    //Track all Asset names
     long sum = 0;

     QVector<int> idVecLiability;      //Track the ID for Liabilitiy
     int idNumLiability = 0;
     QVector<long> LiabilityVector; //Tracks all Liabilitiy values
     QVector<QString> LiabilityName;   //Track all Liability names
     long totalLiability = 0;

};

#endif // Asset_H
