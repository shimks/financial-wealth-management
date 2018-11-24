#ifndef ASSET_H
#define ASSET_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QTreeWidgetItem>
#include <QVector>

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
     QVector<long> sumVector;
     long sum = 0;

     QVector<long> LiabilityVector;
     long totalLiability = 0;

};

#endif // Asset_H
