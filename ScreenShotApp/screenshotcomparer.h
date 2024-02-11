#ifndef SCREENSHOTCOMPARER_H
#define SCREENSHOTCOMPARER_H

#include <QtWidgets>
#include <QScreen>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QGridLayout>
#include <QTimer>
#include <QThread>
#include <QSqlTableModel>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlError>

class ScreenshotComparer : public QObject {
    Q_OBJECT
public:
    explicit ScreenshotComparer(QObject* parent = nullptr) : QObject(parent)
    {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName("screenshots.db");
        if (!m_db.open())
        {
            qWarning() << "Cannot open database";
        }
        createTable();
    }
    ~ScreenshotComparer()
    {
        m_db.close();
    }

    QSqlTableModel model;
    void loadDataFromDatabase()
    {
        model.setQuery("SELECT screenshot, similarity FROM screenshots");
    }

private:
    void insertDataIntoDatabase(const QPixmap& screenshot, double similarity);
    QByteArray QPixmapToByteArray(const QPixmap& pixmap);
    double compareScreenshots(const QPixmap& prevScreenshot, const QPixmap& currentScreenshot);
    void createTable();


public slots:
    void captureAndCompare();

signals:
    void comparisonFinished(double similarity, QPixmap screenshot);

private:
    QSqlDatabase m_db;
    QPixmap m_previousScreenshot;

};




#endif // SCREENSHOTCOMPARER_H
