#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QScreen>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QGridLayout>
#include <QTimer>
#include <QThread>
#include <QSqlTableModel>

#include "workerthread.h"
#include "imagedelegate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void make_main_widget();

public slots:
    void startComparison();
    void stopComparison();

    void captureAndCompare();
    void updateComparison(double similarity, QPixmap screenshot);

private:
    Ui::MainWindow* ui;
    QPushButton* startButton;
    QPushButton* stopButton;
    QLabel* comparisonLabel;
    QTableView* comparisonView;
    QSqlTableModel* model;
    ScreenshotComparer comparer;
    WorkerTask* workerTask;
    QTimer* timer;
};


#endif // MAINWINDOW_H
