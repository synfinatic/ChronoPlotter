#pragma once
#include <QObject>

class TestUntar : public QObject
{
    Q_OBJECT

private slots:
    void normalFile_isExtracted();
    void normalFile_contentIsCorrect();
    void dotdotPrefix_isBlocked();
    void deepTraversal_isBlocked();
    void absolutePath_isBlocked();
    void safeFileAfterUnsafe_isExtracted();
};
