#ifndef SIDEBARTABSTYLE_H
#define SIDEBARTABSTYLE_H

#include <QWidget>
#include <QProxyStyle>

class SidebarTabStyle : public QProxyStyle
{
    Q_OBJECT
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption* option,
                           const QSize& size, const QWidget* widget) const;
    void drawControl(ControlElement element, const QStyleOption* option,
                     QPainter* painter, const QWidget* widget) const;
};

#endif // SIDEBARTABSTYLE_H
