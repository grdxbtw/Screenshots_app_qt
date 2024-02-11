#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    make_main_widget();
}

MainWindow::~MainWindow()
{

}

void MainWindow::make_main_widget()
{
    setWindowTitle("Screenshot Comparison");

    startButton = new QPushButton("Start");
    stopButton = new QPushButton("Stop");
    stopButton->setEnabled(false);
    comparisonLabel = new QLabel("No comparison yet");
    comparisonLabel->setAlignment(Qt::AlignHCenter);
    comparisonLabel->setStyleSheet("font-size: 15px;");
    comparisonView = new QTableView;


    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(startButton);
    layout->addWidget(stopButton);
    layout->addWidget(comparisonLabel);
    layout->addWidget(comparisonView);

    QWidget* centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    comparer.loadDataFromDatabase();
    comparisonView->setModel(&comparer.model);
    comparisonView->setItemDelegate(new ImageDelegate);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::startComparison);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopComparison);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::captureAndCompare);
}

void MainWindow::startComparison()
{
    stopButton->setEnabled(true);
    timer->start(60000); // every minute
    connect(&comparer, &ScreenshotComparer::comparisonFinished, this, &MainWindow::updateComparison);
    comparer.loadDataFromDatabase();
    startButton->setEnabled(false);
}


void MainWindow::stopComparison()
{
    startButton->setEnabled(true);
    timer->stop();
    disconnect(&comparer, &ScreenshotComparer::comparisonFinished, this, &MainWindow::updateComparison);
    stopButton->setEnabled(false);
}

void MainWindow::captureAndCompare()
{
    workerTask = new WorkerTask(&comparer);
    QThreadPool::globalInstance()->start(workerTask);
}

void MainWindow::updateComparison(double similarity, QPixmap screenshot)
{
    comparisonLabel->setText(QString("Similarity: %1%").arg(similarity));
    comparer.loadDataFromDatabase(); // Обновляем данные из базы данных
    comparisonView->reset();
}


