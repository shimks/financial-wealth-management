#include "teststock.h"
#include <QTest>
#include <QApplication>

int main(int argc, char** argv) {
    QApplication a(argc, argv);

    int status = 0;
    auto ASSERT_TEST = [&status, argc, argv](QObject* obj) {
      status |= QTest::qExec(obj, argc, argv);
      delete obj;
    };

    ASSERT_TEST(new TestStock());

    return status;
}
