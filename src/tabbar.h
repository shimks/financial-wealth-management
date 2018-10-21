#ifndef TABBAR_H
#define TABBAR_H

#include <QTabBar>
#include <QWidget>

class TabBar : public QTabBar {
public:
    QSize tabSizeHint(int index) const;

protected:
    void paintEvent(QPaintEvent *_);
};

#endif // TABBAR_H
