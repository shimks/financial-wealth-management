#ifndef TESTCASE_H
#define TESTCASE_H

#include <QtTest>

// add necessary includes here

class testcase : public QObject
{
    Q_OBJECT

public:
    testcase();
    ~testcase();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

#endif // TESTCASE_H
