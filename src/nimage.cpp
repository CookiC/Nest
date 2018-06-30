#include "nimage.h"


QImage &NImage::fitImage(QImage &image, int width, int height, NImage::Fit fit, QColor color) {
    QImage* fittedImage = new QImage();

    if(!(width > 0 && height > 0)) {
        //TODO
        fittedImage = new QImage(image);
    }else{
        switch(fit) {
        case Fit_Stretch:
            *fittedImage = image.scaled(width, height);
            break;
        default:
            break;
        }
    }

    return *fittedImage;
}

QVector<double> &NImage::qImageToStandardRow(QImage &image, int width, int height,
                                             NImage::Fit fit, QColor color)
{
    QImage fittedImage = fitImage(image, width, height, fit, color);
    if(fittedImage.isNull()) {
        //TODO error handling
        throw "something smells fishy..";
    }

    uchar* bits = fittedImage.bits();
    int widthInByte = (fittedImage.width() + 3) / 4 * 4;
    QVector<double>* row = new QVector<double>();
    for(int i = 0; i < fittedImage.sizeInBytes(); i++) {
        if(i % widthInByte < fittedImage.width())
            row->append(double(bits[i]));
    }

    return *row;
}

QVector<double> &NImage::loadStandardRow(QString path, int width, int height, NImage::Fit fit, QColor color)
{
    //TODO
    QVector<double>* row = new QVector<double>();
    return *row;
}

QImage &NImage::standardRowToQImage(StandardData &stdData, int index,
                                    int width, int height, QImage::Format format)
{
    return standardRowToQImage(stdData.getRow(index), width, height, format);
}

QImage &NImage::saveStandardRow(StandardData &stdData, QString path, int index, int width, int height,
                                QImage::Format format)
{
    return saveStandardRow(stdData.getRow(index), path, width, height, format);
}

QImage &NImage::standardRowToQImage(QVector<double>& row, int width, int height, QImage::Format format)
{
    int widthInByte = (width + 3) / 4 * 4, i, j;

    uchar* bits = (uchar*) malloc(sizeof(uchar) * widthInByte * height);
    for(i = 0; i < height; i++) {
        for(j = 0; j < width; j++) {
            bits[i * widthInByte + j] = row.at(i * width + j);
        }
    }

    QImage* image = new QImage(bits, width, height, format);

    return *image;
}

QImage &NImage::saveStandardRow(QVector<double> &row, QString path, int index, int width, int height, QImage::Format format)
{
    //TODO
    QImage* image = new QImage();
    return *image;
}

StandardData &NImage::qImageToStandardData(QImage &image, int width, int height,
                                           NImage::Fit fit, QColor color)
{
    StandardData* stdData;
    return *stdData;
}

StandardData &NImage::loadStandardData(QVector<QString> &paths, int width, int height, NImage::Fit fit, QColor color)
{
    StandardData* stdData;
    return *stdData;
}

StandardData &NImage::loadStandardData(QString &folderPath, int width, int height, NImage::Fit fit, QColor color)
{
    StandardData* stdData;
    return *stdData;
}

QVector<QImage> &NImage::standardDataToQImage(StandardData &stdData, int width, int height,
                                              int start, int end, QImage::Format format)
{
    QVector<QImage>* images;
    return *images;
}

void NImage::saveStandardData(StandardData &stdData, QVector<QString> &paths, int width, int height, int start, int end, QImage::Format format)
{
}

void NImage::saveStandardData(StandardData &stdData, QString folderPath, QString name, int width, int height, int start, int end, QImage::Format format)
{
}

void NImage::test() {
    QImage image = QImage("../images/test1.png");
    if(image.isNull()) {
        deb << "Cannot load the image.";
        return;
    }
    deb << "Image is successfully loaded.";
    image = image.convertToFormat(QImage::Format_Grayscale8);
    QVector<double> row = qImageToStandardRow(image, 100, 100);
    QImage image2 = standardRowToQImage(row, 100, 100);
    image2.save("../images/test2.png");
    deb << "Image is successfully saved.";
}
