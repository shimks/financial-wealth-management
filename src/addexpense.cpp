#include "addexpense.h"
#include "ui_addexpense.h"

addExpense::addExpense(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addExpense)
{
    ui->setupUi(this);
}

addExpense::~addExpense()
{
    delete ui;
}
