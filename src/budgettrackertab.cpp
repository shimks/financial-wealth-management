#include "budgettrackertab.h"
#include "popup_window.h"
#include <QtWidgets>

BudgetTrackerTab::BudgetTrackerTab(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new popup_window());
    setLayout(layout);
}
