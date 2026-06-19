#include <QApplication>
#include <QtTest>

#include "TestMath.h"
#include "TestShotGroup.h"
#include "TestParsers.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    int status = 0;
    TestMath      testMath;
    TestShotGroup testShotGroup;
    TestParsers   testParsers;
    status |= QTest::qExec(&testMath,      argc, argv);
    status |= QTest::qExec(&testShotGroup, argc, argv);
    status |= QTest::qExec(&testParsers,   argc, argv);
    return status;
}
