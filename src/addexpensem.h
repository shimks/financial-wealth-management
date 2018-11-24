#ifndef ADDEXPENSEM_H
#define ADDEXPENSEM_H

#include <QDialog>

namespace Ui {
class addExpenseM;
}

class addExpenseM : public QDialog
{
    Q_OBJECT

public:
    explicit addExpenseM(QWidget *parent = nullptr);
    ~addExpenseM();
    int returnVal();
    QString returnAmo();
    QString returnDay();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::addExpenseM *ui;
    int category;
    QString amount;
    QString day;
};

#endif // ADDEXPENSEM_H
