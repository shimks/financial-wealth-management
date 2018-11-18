#include "teststock.h"
#include "../src/stock.h"
#include <QtTest>

TestStock::TestStock() {}

void TestStock::findStock() {
    Stock stock;
    QSignalSpy spy(&stock, &Stock::stockFound);

    QTest::keyClicks(stock.stockSearchBar, "MSFT");
    QTest::keyClick(stock.stockSearchBar, Qt::Key_Enter);

    // wait until stockFound is emitted
    QVERIFY(spy.wait(5000));

    // check if text is a number
    bool isValidPrice;
    stock.searchedStockPrice->text().toDouble(&isValidPrice);
    QVERIFY(isValidPrice);
}

void TestStock::findStockNotValid() {
    Stock stock;
    QSignalSpy spy(&stock, &Stock::stockNotFound);

    QTest::keyClicks(stock.stockSearchBar, "Not Valid");
    QTest::keyClick(stock.stockSearchBar, Qt::Key_Enter);

    QVERIFY(spy.wait(5000));
    QCOMPARE(stock.searchedStockPrice->text(), "Stock not found");
}
