#include "TestUntar.h"
#include "untar.h"
#include <QtTest>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QDir>
#include <cstring>
#include <cstdio>

// ---------------------------------------------------------------------------
// Minimal ustar tar builder
// ---------------------------------------------------------------------------

static QByteArray makeTarHeader(const char *name, int filesize, char type = '0')
{
    QByteArray header(512, '\0');
    char *h = header.data();

    strncpy(h + 0,   name, 99);
    snprintf(h + 100, 8,  "%07o",  0644);
    snprintf(h + 108, 8,  "%07o",  0);
    snprintf(h + 116, 8,  "%07o",  0);
    snprintf(h + 124, 12, "%011o", filesize);
    snprintf(h + 136, 12, "%011o", 0);
    h[156] = type;
    memcpy(h + 257, "ustar  ", 8);

    // Compute checksum: treat checksum field as spaces
    memset(h + 148, ' ', 8);
    unsigned int sum = 0;
    for (int i = 0; i < 512; i++)
        sum += reinterpret_cast<unsigned char *>(h)[i];
    snprintf(h + 148, 7, "%06o", sum);
    h[154] = '\0';
    h[155] = ' ';

    return header;
}

static QByteArray makeTarEntry(const char *name, const QByteArray &content)
{
    QByteArray result = makeTarHeader(name, content.size());
    result += content;
    int pad = (512 - (content.size() % 512)) % 512;
    result += QByteArray(pad, '\0');
    return result;
}

static QByteArray endOfArchive()
{
    return QByteArray(1024, '\0');
}

// Write tar bytes to a QTemporaryFile and rewind it.
static QTemporaryFile *makeTarFile(const QByteArray &data)
{
    auto *f = new QTemporaryFile();
    if (!f->open()) { delete f; return nullptr; }
    f->write(data);
    f->seek(0);
    return f;
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

void TestUntar::normalFile_isExtracted()
{
    QTemporaryDir extractDir;
    QVERIFY(extractDir.isValid());

    QByteArray tar;
    tar += makeTarEntry("hello.txt", "hello world");
    tar += endOfArchive();

    QScopedPointer<QTemporaryFile> tf(makeTarFile(tar));
    QVERIFY(tf);

    int ret = untar(*tf, extractDir.path());
    QCOMPARE(ret, 0);
    QVERIFY(QFile::exists(extractDir.path() + "/hello.txt"));
}

void TestUntar::normalFile_contentIsCorrect()
{
    QTemporaryDir extractDir;
    QVERIFY(extractDir.isValid());

    QByteArray payload("the quick brown fox");
    QByteArray tar;
    tar += makeTarEntry("content.txt", payload);
    tar += endOfArchive();

    QScopedPointer<QTemporaryFile> tf(makeTarFile(tar));
    QVERIFY(tf);

    QCOMPARE(untar(*tf, extractDir.path()), 0);

    QFile out(extractDir.path() + "/content.txt");
    QVERIFY(out.open(QIODevice::ReadOnly));
    QCOMPARE(out.readAll(), payload);
}

// Use a parent/child directory so we can detect if the file escapes.
static void runTraversalTest(const char *maliciousName)
{
    QTemporaryDir parentDir;
    QVERIFY2(parentDir.isValid(), "Could not create parent temp dir");

    QDir parent(parentDir.path());
    QVERIFY(parent.mkdir("extract"));
    QString extractPath = parentDir.path() + "/extract";

    QByteArray tar;
    tar += makeTarEntry(maliciousName, "malicious content");
    tar += endOfArchive();

    QScopedPointer<QTemporaryFile> tf(makeTarFile(tar));
    QVERIFY(tf);

    int ret = untar(*tf, extractPath);
    QCOMPARE(ret, 0);

    // Nothing should have landed in the extract dir
    QVERIFY(QDir(extractPath).entryList(QDir::Files).isEmpty());

    // For relative traversals the escaped file would be in parentDir
    QStringList escaped = parent.entryList(QDir::Files);
    QVERIFY2(escaped.isEmpty(),
             qPrintable("Escaped file(s) found: " + escaped.join(", ")));
}

void TestUntar::dotdotPrefix_isBlocked()
{
    runTraversalTest("../escape.txt");
}

void TestUntar::deepTraversal_isBlocked()
{
    runTraversalTest("../../deep.txt");
}

void TestUntar::absolutePath_isBlocked()
{
    QTemporaryDir extractDir;
    QVERIFY(extractDir.isValid());

    // Pick a filename we can check in /tmp that should NOT be created
    QString targetName = "chronoplotter_abstest_" +
                         QString::number(QCoreApplication::applicationPid()) + ".txt";
    QString absoluteName = QDir::tempPath() + "/" + targetName;

    // Pre-condition: file must not exist
    QFile::remove(absoluteName);
    QVERIFY(!QFile::exists(absoluteName));

    QByteArray tar;
    tar += makeTarEntry(absoluteName.toLocal8Bit().constData(), "malicious");
    tar += endOfArchive();

    QScopedPointer<QTemporaryFile> tf(makeTarFile(tar));
    QVERIFY(tf);

    int ret = untar(*tf, extractDir.path());
    QCOMPARE(ret, 0);
    QVERIFY(!QFile::exists(absoluteName));
}

void TestUntar::safeFileAfterUnsafe_isExtracted()
{
    QTemporaryDir parentDir;
    QVERIFY(parentDir.isValid());

    QDir parent(parentDir.path());
    QVERIFY(parent.mkdir("extract"));
    QString extractPath = parentDir.path() + "/extract";

    // Traversal entry first, then a legitimate file — both in the same tar
    QByteArray tar;
    tar += makeTarEntry("../should_not_appear.txt", "bad");
    tar += makeTarEntry("safe.txt", "good");
    tar += endOfArchive();

    QScopedPointer<QTemporaryFile> tf(makeTarFile(tar));
    QVERIFY(tf);

    int ret = untar(*tf, extractPath);
    QCOMPARE(ret, 0);

    // The safe file must exist
    QVERIFY(QFile::exists(extractPath + "/safe.txt"));

    // The escaped file must not exist in the parent
    QVERIFY(!QFile::exists(parentDir.path() + "/should_not_appear.txt"));
}
