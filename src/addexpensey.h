#ifndef ADDEXPENSEY_H
#define ADDEXPENSEY_H

#include <QDialog>

namespace Ui {
class addExpenseY;
}

class addExpenseY : public QDialog
{
    Q_OBJECT

public:
    explicit addExpenseY(QWidget *parent = nullptr);
    ~addExpenseY();
    int returnVal();
    QString returnAmo();
    int returnDay();
private slots:

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();


private:
    Ui::addExpenseY *ui;
    int category;
    QString amount;
    int day;
};

#endif // ADDEXPENSEY_H
