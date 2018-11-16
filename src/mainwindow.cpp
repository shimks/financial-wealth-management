#include "mainwindow.h"
#include "sidebartabstyle.h"
#include "networthtab.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QTabWidget *tabs = new QTabWidget;
    tabs->setTabPosition(QTabWidget::West);
    tabs->tabBar()->setStyle(new SidebarTabStyle);
    tabs->addTab(new NetWorthTab, "Net Worth Tracker");
    tabs->addTab(new QWidget, "Budget Tracker");
    setCentralWidget(tabs);
}

MainWindow::~MainWindow() {}
