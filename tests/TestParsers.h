#pragma once
#include <QObject>
#include "PowderTest.h"

class PowderTestExposed : public Powder::PowderTest
{
public:
    using Powder::PowderTest::ExtractLabRadarSeries;
    using Powder::PowderTest::ExtractMagnetoSpeedSeries;
    using Powder::PowderTest::ExtractProChronoSeries;
    using Powder::PowderTest::ExtractProChronoSeries_format2;
    using Powder::PowderTest::ExtractGarminSeries_csv;
};

class TestParsers : public QObject
{
    Q_OBJECT

private:
    PowderTestExposed *p = nullptr;

    QString fixture(const QString &name) const;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void labradar_velocities();
    void labradar_seriesNumber();
    void labradar_units();

    void magnetospeed_legacySeriesCount();
    void magnetospeed_legacyVelocities();
    void magnetospeed_xfrName();
    void magnetospeed_xfrVelocities();

    void prochrono_format1_velocities();
    void prochrono_format1_seriesNumber();

    void prochrono_format2_velocities();

    void garmin_csv_velocities();
    void garmin_csv_name();
    void garmin_csv_date();
};
