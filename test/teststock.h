#ifndef STOCKTEST_H
#define STOCKTEST_H

#include <QtTest>

class TestStock: public QObject
{
    Q_OBJECT

public:
    TestStock();

private slots:
    void findStock();
    void findStockNotValid();
};

#endif // STOCKTEST_H
