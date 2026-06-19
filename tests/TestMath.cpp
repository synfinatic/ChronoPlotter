#include "TestMath.h"
#include "ChronoPlotter.h"
#include <QtTest>
#include <cmath>

void TestMath::sampleStdev_basic()
{
    QList<double> vals = {10.0, 20.0, 30.0};
    // mean=20, variance=(100+0+100)/2=100, stdev=10
    QVERIFY(std::abs(sampleStdev(vals) - 10.0) < 1e-10);
}

void TestMath::sampleStdev_allSame()
{
    QList<double> vals = {5.0, 5.0, 5.0};
    QVERIFY(std::abs(sampleStdev(vals)) < 1e-10);
}

void TestMath::sampleStdev_singleElement()
{
    // Single element causes 0/0 => NaN
    QList<double> vals = {5.0};
    QVERIFY(std::isnan(sampleStdev(vals)));
}

void TestMath::sampleStdev_doubles()
{
    // [1.0, 3.0] -> mean=2, stdev=sqrt(2)
    QList<double> vals = {1.0, 3.0};
    QVERIFY(std::abs(sampleStdev(vals) - std::sqrt(2.0)) < 1e-10);
}

void TestMath::GetLinearFit_perfectLine()
{
    QVector<double> x = {0.0, 1.0, 2.0};
    QVector<double> y = {0.0, 1.0, 2.0};
    auto res = GetLinearFit(x, y);
    QVERIFY(std::abs(res.at(0) - 1.0) < 1e-10);  // slope
    QVERIFY(std::abs(res.at(1) - 0.0) < 1e-10);  // intercept
}

void TestMath::GetLinearFit_withIntercept()
{
    QVector<double> x = {0.0, 1.0, 2.0};
    QVector<double> y = {5.0, 6.0, 7.0};
    auto res = GetLinearFit(x, y);
    QVERIFY(std::abs(res.at(0) - 1.0) < 1e-10);  // slope
    QVERIFY(std::abs(res.at(1) - 5.0) < 1e-10);  // intercept
}

void TestMath::GetLinearFit_negativeSlope()
{
    QVector<double> x = {0.0, 1.0, 2.0};
    QVector<double> y = {10.0, 8.0, 6.0};
    auto res = GetLinearFit(x, y);
    QVERIFY(std::abs(res.at(0) - (-2.0)) < 1e-10); // slope
    QVERIFY(std::abs(res.at(1) - 10.0) < 1e-10);   // intercept
}

void TestMath::spline_passesThroughKnots()
{
    QVector<double> x = {0.0, 1.0, 2.0, 3.0};
    QVector<double> y = {0.0, 1.0, 4.0, 9.0};
    auto s = spline(x, y);
    QCOMPARE((int)s.size(), 3);
    for (int i = 0; i < (int)s.size(); i++) {
        QVERIFY(std::abs(s[i].a - y[i]) < 1e-10);
        QVERIFY(std::abs(s[i].x - x[i]) < 1e-10);
    }
}

void TestMath::spline_linearData()
{
    QVector<double> x = {0.0, 1.0, 2.0, 3.0};
    QVector<double> y = {0.0, 1.0, 2.0, 3.0};
    auto s = spline(x, y);
    for (int i = 0; i < (int)s.size(); i++) {
        QVERIFY(std::abs(s[i].b - 1.0) < 1e-10);
        QVERIFY(std::abs(s[i].c) < 1e-10);
        QVERIFY(std::abs(s[i].d) < 1e-10);
    }
}
