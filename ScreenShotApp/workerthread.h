#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QtWidgets>
#include <QScreen>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QGridLayout>
#include <QTimer>
#include <QThread>

#include "screenshotcomparer.h"

class WorkerTask : public QRunnable {
public:
    WorkerTask(ScreenshotComparer* comparer) : m_comparer(comparer) {}

    void run() override
    {
        m_comparer->captureAndCompare();
    }

private:
    ScreenshotComparer* m_comparer;
};

#endif // WORKERTHREAD_H
