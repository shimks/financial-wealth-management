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
    void EditRoot(QString Amount);
    void AddExpense(QString Category, QString Amount, QString Day);
    void EditExpense(QString Category, QString Amount, QString Day);



private slots:
    void on_Edit_clicked();
    void on_AddE_clicked();
    void on_DeleteE_clicked();

    //void on_EditE_clicked();

private:
    Ui::BudgetTarget *ui;
    QStringList cateList;
    QList<int> amountList;
    QStringList expenseList;
    QList<int> expenseAmoList;
    QStringList dayList;
};

#endif // BUDGETTARGET_H
