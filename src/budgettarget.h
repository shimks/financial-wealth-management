#ifndef BUDGETTARGET_H
#define BUDGETTARGET_H

#include <QWidget>

namespace Ui {
class BudgetTarget;
}

class BudgetTarget : public QWidget
{
    Q_OBJECT

public:
    explicit BudgetTarget(QWidget *parent = nullptr);
    ~BudgetTarget();

    void AddRoot(QString Category, QString Amount);
    void EditRoot(int Index, QString Category, QString Amount);


private slots:
    void on_pushButton_4_clicked();


    void on_Add_clicked();


    void on_Delete_clicked();

    void on_Edit_clicked();


private:
    Ui::BudgetTarget *ui;
    QStringList cateList;
    QList<int> amountList;
};

#endif // BUDGETTARGET_H
