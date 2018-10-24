#include "mainwindow.h"
#include "tabwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    TabWidget *tabs = new TabWidget(centralWidget);
    tabs->addTab(new QWidget, "Net Worth Tracker");
    tabs->addTab(new QWidget, "Budget Tracker");
}

MainWindow::~MainWindow() {}
