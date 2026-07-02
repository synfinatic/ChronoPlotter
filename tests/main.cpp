#include <QApplication>
#include <QtTest>

#include "TestMath.h"
#include "TestShotGroup.h"
#include "TestParsers.h"
#include "TestUntar.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    int status = 0;
    TestMath      testMath;
    TestShotGroup testShotGroup;
    TestParsers   testParsers;
    TestUntar     testUntar;
    status |= QTest::qExec(&testMath,      argc, argv);
    status |= QTest::qExec(&testShotGroup, argc, argv);
    status |= QTest::qExec(&testParsers,   argc, argv);
    status |= QTest::qExec(&testUntar,     argc, argv);
    return status;
}
