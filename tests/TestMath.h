#pragma once
#include <QObject>

class TestMath : public QObject
{
    Q_OBJECT

private slots:
    void sampleStdev_basic();
    void sampleStdev_allSame();
    void sampleStdev_singleElement();
    void sampleStdev_doubles();

    void GetLinearFit_perfectLine();
    void GetLinearFit_withIntercept();
    void GetLinearFit_negativeSlope();

    void spline_passesThroughKnots();
    void spline_linearData();
};
