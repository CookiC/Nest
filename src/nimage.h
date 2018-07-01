#ifndef NIMAGE_H
#define NIMAGE_H

#include "header.h"
#include "standarddata.h"

class NImage
{
private:
    static void delete_data(void*);
public:
    enum Fit {
        Fit_Center, Fit_Fill, Fit_Fit, Fit_Stretch, Fit_Tile
    };

    static QImage& fitImage(QImage &image, int width, int height, Fit fit, QColor color);
    static QVector<double>& qImageToStandardRow(QImage& image, int width = 0, int height = 0,
                                                Fit fit = Fit_Stretch, QColor color = qRgb(0, 0, 0));
    static QVector<double>& loadStandardRow(QString path, int width = 0, int height = 0,
                                            QImage::Format format = QImage::Format_Grayscale8,
                                            Fit fit = Fit_Stretch, QColor color = qRgb(0, 0, 0));
    static QImage& standardRowToQImage(StandardData& stdData, int index, int width, int height,
                                       QImage::Format format = QImage::Format_Grayscale8);
    static void saveStandardRow(StandardData& stdData, QString path,
                                   int index, int width, int height,
                                   QImage::Format format = QImage::Format_Grayscale8);
    static QImage& standardRowToQImage(QVector<double> row, int width, int height,
                                   QImage::Format format = QImage::Format_Grayscale8);
    static void saveStandardRow(QVector<double> row, QString path, int width, int height,
                                   QImage::Format format = QImage::Format_Grayscale8);
    static StandardData& qImageToStandardData(QVector<QImage>& image, int width = 0,int height = 0,
                                              Fit fit = Fit_Stretch, QColor color = qRgb(0, 0, 0));
    static StandardData& loadStandardData(QStringList& paths, int width = 0,int height = 0,
                                          QImage::Format format = QImage::Format_Grayscale8,
                                          Fit fit = Fit_Stretch, QColor color = qRgb(0, 0, 0));
    static StandardData& loadStandardData(QString folderPath, int width = 0,int height = 0,
                                          QImage::Format format = QImage::Format_Grayscale8,
                                          Fit fit = Fit_Stretch, QColor color = qRgb(0, 0, 0));
    static QVector<QImage>& standardDataToQImage(StandardData& stdData,
                                                 int width, int height, int start = 0, int end = 0,
                                                 QImage::Format format = QImage::Format_Grayscale8);
    static void saveStandardData(StandardData& stdData, QVector<QString>& paths,
                                 int width, int height, int start = 0, int end = 0,
                                 QImage::Format format = QImage::Format_Grayscale8);
    static void saveStandardData(StandardData& stdData, QString name,
                                 int width, int height, int start = 0, int end = 0,
                                 QImage::Format format = QImage::Format_Grayscale8);

    static void test();
};

#endif // NIMAGE_H
