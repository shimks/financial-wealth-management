#include "budgettrackertab.h"
#include "popup_window.h"
#include "budgettarget.h"
#include <QtWidgets>

BudgetTrackerTab::BudgetTrackerTab(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new BudgetTarget);
    setLayout(layout);
}

