#include "networthtab.h"
#include "stock.h"
#include <QtWidgets>

NetWorthTab::NetWorthTab(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new Stock);
    setLayout(layout);
}
