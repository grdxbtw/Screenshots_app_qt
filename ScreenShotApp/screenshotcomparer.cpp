#include "screenshotcomparer.h"


void ScreenshotComparer::captureAndCompare()
{
    // make screenshot
    QPixmap screenshot = QGuiApplication::primaryScreen()->grabWindow(0);

    // compare to the previous screen
    if (m_previousScreenshot.isNull())
    {
        m_previousScreenshot = screenshot;
        emit comparisonFinished(0, screenshot); // first screen
    } else
    {
        double similarity = compareScreenshots(m_previousScreenshot, screenshot);
        emit comparisonFinished(similarity, screenshot);
        m_previousScreenshot = screenshot;

        // save to db
        insertDataIntoDatabase(screenshot, similarity);
    }
}

void ScreenshotComparer::insertDataIntoDatabase(const QPixmap &screenshot, double similarity)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO screenshots (screenshot, similarity) VALUES (:screenshot, :similarity)");
    query.bindValue(":screenshot", QPixmapToByteArray(screenshot));
    query.bindValue(":similarity", similarity);

    if (!query.exec())
    {
        qDebug() << "Error inserting data into database:" << query.lastError().text();
    } else
    {
        qDebug() << "Data inserted successfully";
    }
}

QByteArray ScreenshotComparer::QPixmapToByteArray(const QPixmap &pixmap)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    return byteArray;
}

double ScreenshotComparer::compareScreenshots(const QPixmap &prevScreenshot, const QPixmap &currentScreenshot)
{
    QImage image1 = prevScreenshot.toImage();
    QImage image2 = currentScreenshot.toImage();

    int matchedPixels = 0;
    int totalPixels = image1.width() * image1.height();

    //pixel comparison
    for (int y = 0; y < image1.height(); ++y)
    {
        for (int x = 0; x < image1.width(); ++x)
        {
            QColor color1 = image1.pixelColor(x, y);
            QColor color2 = image2.pixelColor(x, y);
            if (color1 == color2)
            {
                ++matchedPixels;
            }
        }
    }

    // percent of similarity
    return (static_cast<double>(matchedPixels) / totalPixels) * 100.0;
}

void ScreenshotComparer::createTable()
{
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS screenshots (id INTEGER PRIMARY KEY AUTOINCREMENT, screenshot BLOB, similarity REAL)"))
    {
        qWarning() << "Failed to create table:" << query.lastError().text();
    }
}
