#include "TestParsers.h"
#include <QtTest>
#include <QFile>
#include <QTextStream>

extern bool readCSVRow(QTextStream &in, QStringList *row);

#ifndef FIXTURES_DIR
#define FIXTURES_DIR "fixtures"
#endif

QString TestParsers::fixture(const QString &name) const
{
    return QString(FIXTURES_DIR) + "/" + name;
}

static void freeSeries(QList<Powder::ChronoSeries *> list)
{
    for (auto *s : list) delete s;
}

void TestParsers::initTestCase()
{
    p = new PowderTestExposed();
}

void TestParsers::cleanupTestCase()
{
    delete p;
    p = nullptr;
}

// ── LabRadar ──────────────────────────────────────────────────────────────────

void TestParsers::labradar_velocities()
{
    QFile f(fixture("labradar.csv"));
    QVERIFY2(f.open(QIODevice::ReadOnly | QIODevice::Text),
             qPrintable("Cannot open " + f.fileName()));
    QTextStream ts(&f);
    auto *s = p->ExtractLabRadarSeries(ts);
    QVERIFY(s->isValid);
    QCOMPARE(s->muzzleVelocities.size(), 3);
    QCOMPARE(s->muzzleVelocities.at(0), 2850.0);
    QCOMPARE(s->muzzleVelocities.at(1), 2875.0);
    QCOMPARE(s->muzzleVelocities.at(2), 2820.0);
    delete s;
}

void TestParsers::labradar_seriesNumber()
{
    QFile f(fixture("labradar.csv"));
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream ts(&f);
    auto *s = p->ExtractLabRadarSeries(ts);
    QCOMPARE(s->seriesNum, 3);
    delete s;
}

void TestParsers::labradar_units()
{
    QFile f(fixture("labradar.csv"));
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream ts(&f);
    auto *s = p->ExtractLabRadarSeries(ts);
    QCOMPARE(s->velocityUnits, QString("ft/s"));
    delete s;
}

// ── MagnetoSpeed legacy ───────────────────────────────────────────────────────

void TestParsers::magnetospeed_legacySeriesCount()
{
    QFile f(fixture("magnetospeed_legacy.csv"));
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream ts(&f);
    auto list = p->ExtractMagnetoSpeedSeries(ts);
    QCOMPARE(list.size(), 2);
    freeSeries(list);
}

void TestParsers::magnetospeed_legacyVelocities()
{
    QFile f(fixture("magnetospeed_legacy.csv"));
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream ts(&f);
    auto list = p->ExtractMagnetoSpeedSeries(ts);
    QCOMPARE(list.size(), 2);
    QCOMPARE(list.at(0)->muzzleVelocities.size(), 3);
    QCOMPARE(list.at(0)->muzzleVelocities.at(0), 2850.0);
    QCOMPARE(list.at(1)->muzzleVelocities.size(), 2);
    QCOMPARE(list.at(1)->muzzleVelocities.at(0), 2900.0);
    freeSeries(list);
}

// ── MagnetoSpeed XFR ─────────────────────────────────────────────────────────

void TestParsers::magnetospeed_xfrName()
{
    QFile f(fixture("magnetospeed_xfr.csv"));
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream ts(&f);
    auto list = p->ExtractMagnetoSpeedSeries(ts);
    QCOMPARE(list.size(), 1);
    QVERIFY(list.at(0)->name != nullptr);
    QCOMPARE(list.at(0)->name->text(), QString("My Load"));
    freeSeries(list);
}

void TestParsers::magnetospeed_xfrVelocities()
{
    QFile f(fixture("magnetospeed_xfr.csv"));
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream ts(&f);
    auto list = p->ExtractMagnetoSpeedSeries(ts);
    QVERIFY(!list.isEmpty());
    QCOMPARE(list.at(0)->muzzleVelocities.size(), 3);
    QCOMPARE(list.at(0)->muzzleVelocities.at(0), 2850.0);
    freeSeries(list);
}

// ── ProChrono format 1 ────────────────────────────────────────────────────────

void TestParsers::prochrono_format1_velocities()
{
    QFile f(fixture("prochrono_format1.csv"));
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream ts(&f);
    auto list = p->ExtractProChronoSeries(ts);
    QCOMPARE(list.size(), 1);
    QCOMPARE(list.at(0)->muzzleVelocities.size(), 3);
    QCOMPARE(list.at(0)->muzzleVelocities.at(0), 2850.0);
    QCOMPARE(list.at(0)->muzzleVelocities.at(1), 2875.0);
    QCOMPARE(list.at(0)->muzzleVelocities.at(2), 2820.0);
    freeSeries(list);
}

void TestParsers::prochrono_format1_seriesNumber()
{
    QFile f(fixture("prochrono_format1.csv"));
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream ts(&f);
    auto list = p->ExtractProChronoSeries(ts);
    // Single series gets seriesNum=1 (reversed, so last becomes 1)
    QCOMPARE(list.at(0)->seriesNum, 1);
    freeSeries(list);
}

// ── ProChrono format 2 ────────────────────────────────────────────────────────

void TestParsers::prochrono_format2_velocities()
{
    QFile f(fixture("prochrono_format2.csv"));
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream ts(&f);
    auto list = p->ExtractProChronoSeries_format2(ts);
    QCOMPARE(list.size(), 1);
    QCOMPARE(list.at(0)->muzzleVelocities.size(), 3);
    QCOMPARE(list.at(0)->muzzleVelocities.at(0), 2850.0);
    QCOMPARE(list.at(0)->muzzleVelocities.at(1), 2875.0);
    QCOMPARE(list.at(0)->muzzleVelocities.at(2), 2820.0);
    freeSeries(list);
}

// ── Garmin CSV ────────────────────────────────────────────────────────────────

void TestParsers::garmin_csv_velocities()
{
    QFile f(fixture("garmin.csv"));
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream ts(&f);
    auto list = p->ExtractGarminSeries_csv(ts);
    QCOMPARE(list.size(), 1);
    QCOMPARE(list.at(0)->muzzleVelocities.size(), 3);
    QVERIFY(std::abs(list.at(0)->muzzleVelocities.at(0) - 2850.0) < 1e-6);
    QVERIFY(std::abs(list.at(0)->muzzleVelocities.at(1) - 2875.0) < 1e-6);
    QVERIFY(std::abs(list.at(0)->muzzleVelocities.at(2) - 2820.0) < 1e-6);
    freeSeries(list);
}

void TestParsers::garmin_csv_name()
{
    QFile f(fixture("garmin.csv"));
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream ts(&f);
    auto list = p->ExtractGarminSeries_csv(ts);
    QCOMPARE(list.size(), 1);
    QVERIFY(list.at(0)->name != nullptr);
    QCOMPARE(list.at(0)->name->text(), QString("My Garmin Series"));
    freeSeries(list);
}

void TestParsers::garmin_csv_date()
{
    QFile f(fixture("garmin.csv"));
    QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream ts(&f);
    auto list = p->ExtractGarminSeries_csv(ts);
    QCOMPARE(list.size(), 1);
    QCOMPARE(list.at(0)->firstDate, QString("2023-01-15"));
    freeSeries(list);
}
