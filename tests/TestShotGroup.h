#pragma once
#include <QObject>
#include "TunerTest.h"

class TunerTestExposed : public Tuner::TunerTest
{
public:
    using Tuner::TunerTest::calculateES;
    using Tuner::TunerTest::calculateXStdev;
    using Tuner::TunerTest::calculateYStdev;
    using Tuner::TunerTest::calculateRSD;
    using Tuner::TunerTest::calculateMR;
};

class TestShotGroup : public QObject
{
    Q_OBJECT

private:
    TunerTestExposed *t = nullptr;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void calculateES_twoPoints();
    void calculateES_square4Shots();
    void calculateES_singleShot();
    void calculateES_twoSamePoint();

    void calculateXStdev_axisAligned();
    void calculateYStdev_axisAligned();
    void calculateXStdev_spread();

    void calculateRSD_knownValues();

    void calculateMR_centeredGroup();
    void calculateMR_singleShot();
};
