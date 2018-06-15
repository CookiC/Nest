#include <QCoreApplication>
#include <QImage>
#include <QColor>
#include <iostream>
#include <QFile>
#include <QString>
#include <vector>

using namespace std;

bool loadCsv(QString filePath, vector<vector<uchar>> *contents);
bool v2c(vector<vector<uchar>> vcontents, uchar** pcontents);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    vector<vector<uchar>> vdata;
    uchar* pixels;

    if(loadCsv("data/test.csv", &vdata)) {
        v2c(vdata, &pixels);
        QImage *image = new QImage(pixels, vdata.at(0).size(), vdata.size(), QImage::Format_Grayscale8);
        image -> save("images/data.o.png");

        cout << "done" << endl;
    }
    return a.exec();
}

/* copy from genericdata.cpp */
bool loadCsv(QString filePath, vector<vector<uchar>> *contents){
    QFile fileIn(filePath);
    QByteArray line, val;
    int i;
    vector<uchar> row;

    fileIn.open(QIODevice::ReadOnly | QIODevice::Text);

    if(!fileIn.isOpen()){
        cout << "error: " << fileIn.errorString().toStdString() << endl;
        return false;
    }

    fileIn.readLine();

    while(!fileIn.atEnd()) {
        line=fileIn.readLine();
        val.clear();
        row.clear();
        for(i=0;i<line.size()-1;++i){
            if(line[i]==','){
                row.push_back(uchar(val.toInt()));
                val.clear();
            }
            else if(line[i]=='"'){
                for(++i;line[i]!='"'&&i<line.size();++i)
                    val.append(line[i]);
            }
            else
                val.append(line[i]);
        }
        row.push_back(uchar(val.toInt()));

        contents->push_back(row);
    }
    fileIn.close();
    cout<<"Load end!"<<endl;
    return true;
}

bool v2c(vector<vector<uchar>> vcontents, uchar** pcontents){
    int width = int(vcontents.at(0).size());
    int size = int(vcontents.size()) * width;
    int i;

    using _puchar = uchar*;
    static _puchar contents = _puchar(malloc(sizeof(uchar) * (unsigned long long int)size));

    for(i = 0; i < size; i++)
        contents[i] = vcontents.at((unsigned long long int)(i / width)).at((unsigned long long int)(i % width));

    *pcontents = contents;

    return true;
}
