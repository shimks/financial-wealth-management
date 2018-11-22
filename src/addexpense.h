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

private:
    Ui::addExpense *ui;
};

#endif // ADDEXPENSE_H
