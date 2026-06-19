#include "TestShotGroup.h"
#include <QtTest>
#include <cmath>

void TestShotGroup::initTestCase()
{
    t = new TunerTestExposed();
}

void TestShotGroup::cleanupTestCase()
{
    delete t;
    t = nullptr;
}

void TestShotGroup::calculateES_twoPoints()
{
    QList<QPair<double,double>> pts = { {0.0, 0.0}, {3.0, 4.0} };
    QVERIFY(std::abs(t->calculateES(pts) - 5.0) < 1e-10);
}

void TestShotGroup::calculateES_square4Shots()
{
    QList<QPair<double,double>> pts = { {-1.0,-1.0}, {-1.0,1.0}, {1.0,-1.0}, {1.0,1.0} };
    QVERIFY(std::abs(t->calculateES(pts) - 2.0 * std::sqrt(2.0)) < 1e-10);
}

void TestShotGroup::calculateES_singleShot()
{
    QList<QPair<double,double>> pts = { {1.0, 2.0} };
    QVERIFY(std::isnan(t->calculateES(pts)));
}

void TestShotGroup::calculateES_twoSamePoint()
{
    QList<QPair<double,double>> pts = { {1.0, 1.0}, {1.0, 1.0} };
    QVERIFY(std::abs(t->calculateES(pts)) < 1e-10);
}

void TestShotGroup::calculateXStdev_axisAligned()
{
    QList<QPair<double,double>> pts = { {0.0,-1.0}, {0.0,0.0}, {0.0,1.0} };
    QVERIFY(std::abs(t->calculateXStdev(pts)) < 1e-10);
}

void TestShotGroup::calculateYStdev_axisAligned()
{
    QList<QPair<double,double>> pts = { {-1.0,0.0}, {0.0,0.0}, {1.0,0.0} };
    QVERIFY(std::abs(t->calculateYStdev(pts)) < 1e-10);
}

void TestShotGroup::calculateXStdev_spread()
{
    // x=[10,20,30], mean=20, variance=100, stdev=10
    QList<QPair<double,double>> pts = { {10.0,0.0}, {20.0,0.0}, {30.0,0.0} };
    QVERIFY(std::abs(t->calculateXStdev(pts) - 10.0) < 1e-10);
}

void TestShotGroup::calculateRSD_knownValues()
{
    // [(1,0),(0,1),(-1,0),(0,-1)]: xStdev=yStdev=sqrt(2/3), RSD=2/sqrt(3)
    QList<QPair<double,double>> pts = { {1.0,0.0}, {0.0,1.0}, {-1.0,0.0}, {0.0,-1.0} };
    double expected = 2.0 / std::sqrt(3.0);
    QVERIFY(std::abs(t->calculateRSD(pts) - expected) < 1e-10);
}

void TestShotGroup::calculateMR_centeredGroup()
{
    // Shots on unit circle around origin: each distance=1.0, MR=1.0
    QList<QPair<double,double>> pts = { {1.0,0.0}, {-1.0,0.0}, {0.0,1.0}, {0.0,-1.0} };
    QVERIFY(std::abs(t->calculateMR(pts) - 1.0) < 1e-10);
}

void TestShotGroup::calculateMR_singleShot()
{
    QList<QPair<double,double>> pts = { {5.0, 3.0} };
    QVERIFY(std::isnan(t->calculateMR(pts)));
}
