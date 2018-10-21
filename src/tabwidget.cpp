#include "tabwidget.h"
#include <QTabWidget>
#include "tabbar.h"
#include <QTabBar>

TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent) {
    setTabBar(new TabBar);
    setTabPosition(QTabWidget::West);
}
