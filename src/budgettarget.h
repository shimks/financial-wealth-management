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
    void AddRoot_2(QString Category, QString Amount);
    void AddRoot_3(QString Category, QString Amount);


    void EditRoot(QString Amount);
    void EditRoot_2(QString Amount);
    void EditRoot_3(QString Amount);


    void AddExpense(QString Category, QString Amount, QString Day);
    void AddExpense_2(QString Category, QString Amount, QString Day);
    void AddExpense_3(QString Category, QString Amount, QString Day);


private slots:
    void on_Edit_clicked();
    void on_AddE_clicked();
    void on_DeleteE_clicked();
    void on_Edit_2_clicked();
    void on_AddE_2_clicked();
    void on_DeleteE_2_clicked();

    void on_Edit_4_clicked();
    void on_AddE_3_clicked();
    void on_DeleteE_3_clicked();


private:
    Ui::BudgetTarget *ui;
    QStringList cateList;
    QStringList dayList;
    QStringList cateListM;
    QStringList cateListY;
    QStringList dayListY;
};

#endif // BUDGETTARGET_H
