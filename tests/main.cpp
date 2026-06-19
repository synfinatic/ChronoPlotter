#include <QApplication>
#include <QtTest>

#include "TestMath.h"
#include "TestShotGroup.h"
#include "TestParsers.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    int status = 0;
    status |= QTest::qExec(new TestMath,      argc, argv);
    status |= QTest::qExec(new TestShotGroup, argc, argv);
    status |= QTest::qExec(new TestParsers,   argc, argv);
    return status;
}
