#include "nimage.h"

void NImage::delete_data(void * info) {
    delete (uchar*)info;
}

QImage NImage::fitImage(QImage &image, int width, int height, NImage::Fit fit, QColor color) {
                        QImage fittedImage;

    if(!(width | height)) {
        fittedImage = QImage(image);
    }else{
        if(!width) width = image.width();
        if(!height) height = image.height();
        switch(fit) {
        //TODO other fit
        case Fit_Stretch:
            fittedImage = image.scaled(width, height);
            break;
        default:
            break;
        }
    }

    return fittedImage;
}

QVector<double> NImage::qImageToStandardRow(QImage &image, int width, int height,
                                            NImage::Fit fit, QColor color)
{
    QImage fittedImage = fitImage(image, width, height, fit, color);
    if(fittedImage.isNull()) {
        //TODO error handling
    }

    uchar* bits = fittedImage.bits();
    int widthInByte = (fittedImage.width() + 3) / 4 * 4;
    QVector<double> row = QVector<double>();
    for(int i = 0; i < fittedImage.sizeInBytes(); i++) {
        if(i % widthInByte < fittedImage.width())
            row.append(double(bits[i]));
    }

    return row;
}

QVector<double> NImage::loadStandardRow(QString path, int width, int height, QImage::Format format,
                                         NImage::Fit fit, QColor color)
{
    QImage image = QImage(path);
    if(image.isNull()) {
        //TODO error handling
        throw "no such fucking thing: " + path;
    }
    image = image.convertToFormat(format);

    return qImageToStandardRow(image, width, height, fit, color);
}

QImage NImage::standardRowToQImage(StandardData &stdData, int index,
                                    int width, int height, QImage::Format format)
{
    return standardRowToQImage(stdData.getRow(index), width, height, format);
}

void NImage::saveStandardRow(StandardData &stdData, QString path, int index, int width, int height,
                             QImage::Format format)
{
    saveStandardRow(stdData.getRow(index), path, width, height, format);
}

QImage NImage::standardRowToQImage(QVector<double> row, int width, int height, QImage::Format format)
{
    int widthInByte = (width + 3) / 4 * 4, i, j;

    uchar* bits = (uchar*) malloc(sizeof(uchar) * widthInByte * height);
    for(i = 0; i < height; i++) {
        for(j = 0; j < width; j++) {
            bits[i * widthInByte + j] = row.at(i * width + j);
        }
    }

    QImage image = QImage(bits, width, height, format, delete_data, bits);

    return image;
}

void NImage::saveStandardRow(QVector<double> row, QString path, int width, int height,
                             QImage::Format format)
{
    QImage image = standardRowToQImage(row, width, height, format);
    image.save(path);
}

StandardData NImage::qImageToStandardData(QVector<QImage> &images, int width, int height,
                                          NImage::Fit fit, QColor color)
{
    StandardData stdData = StandardData();
    if(!width || !height) {
        bool w = !width, h = !height;
        for(auto itr = images.begin(); itr < images.end(); itr++) {
            if(w && width < itr->width()) width = itr->width();
            if(h && height < itr->height()) height = itr->height();
        }
    }

    QVector<double> row;
    int i;
    for(i = 0; i < images.size(); i++) {
        row = qImageToStandardRow(images[i], width, height, fit, color);
        stdData.appendRow(row, QString::number(i));
    }

    return stdData;
}

StandardData NImage::loadStandardData(QStringList &paths, int width, int height,
                                      QImage::Format format, NImage::Fit fit, QColor color)
{
    QVector<QImage> images = QVector<QImage>();
    for(auto itr = paths.begin(); itr < paths.end();itr++) {
        images.append(QImage(*itr).convertToFormat(format));
    }

    return qImageToStandardData(images, width, height, fit, color);
}

StandardData NImage::loadStandardData(QString folderPath, int width, int height,
                                      QImage::Format format, NImage::Fit fit, QColor color)
{
    //TODO image formats
    if(folderPath.endsWith("/")) folderPath.chop(1);

    QDir folder = QDir(folderPath);
    QStringList filters;
    filters << "*.png" << "*.bmp" << "*.jpg";
    QStringList paths = folder.entryList(filters);
    for(auto itr = paths.begin(); itr < paths.end(); itr++) {
        *itr = folderPath + "/" + *itr;
    }
    deb << paths;
    return loadStandardData(paths, width, height, format, fit, color);
}

QVector<QImage> NImage::standardDataToQImage(StandardData &stdData, int width, int height,
                                             int start, int end, QImage::Format format)
{
    QVector<QImage> images = QVector<QImage>();

    if(!end) end = stdData.getRowNum();
    for(int i = start; i < end; i++) {
        images.append(standardRowToQImage(stdData, i, width, height, format));
    }

    return images;
}

void NImage::saveStandardData(StandardData &stdData, QVector<QString> &paths, int width, int height,
                              int start, int end, QImage::Format format)
{
    QVector<QImage> images = standardDataToQImage(stdData, width, height, start, end, format);
    for(int i = 0; i < images.size(); i++) {
        images[i].save(paths[i]);
    }
}

void NImage::saveStandardData(StandardData &stdData, QString name, int width, int height, int start, int end, QImage::Format format)
{
    QVector<QImage> images = standardDataToQImage(stdData, width, height, start, end, format);
    for(int i = 0; i < images.size(); i++) {
        images[i].save(QString::asprintf(name.toStdString().c_str(), i));
    }
}

void NImage::test() {
    StandardData testData = StandardData();
    testData.loadCsv("../../tests/data/test.csv", true, false);
    //saveStandardData(testData, "../../test/images/data%d.png", 28, 28);
}
