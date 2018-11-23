#ifndef ADDEXPENSE_H
#define ADDEXPENSE_H

#include <QDialog>

namespace Ui {
class addExpense;
}

class addExpense : public QDialog
{
    Q_OBJECT

public:
    explicit addExpense(QWidget *parent = nullptr);
    ~addExpense();
    int returnVal();
    QString returnAmo();
    int returnDay();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::addExpense *ui;
    int category;
    QString amount;
    int day;
};

#endif // ADDEXPENSE_H
