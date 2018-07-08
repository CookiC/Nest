#include "standarddata.h"

//public

StandardData::StandardData():flag(nullptr),nomName(nullptr){
}

StandardData::~StandardData(){
    if(flag)
        delete[] flag;
    if(nomName)
        delete[] nomName;
}

void StandardData::cutColFrom(StandardData *src, int colIndex){
}

void StandardData::cutColFrom(StandardData *src, QVector<int> &colIndex){
}

//protect

void StandardData::allocateMemory(){
    flag = new Category*[rowNum];
    for(int i=0;i<rowNum;++i)
        flag[i] = new Category[colNum];
}

bool StandardData::loadRow(int i, const QStringVector &row){
    bool ok;
    QVector<double> r;
    for(int j=0;j<row.size();++j)
        if(row[j].isEmpty())
            flag[i][j] = MISS;
        else{
            flag[i][j] = NUM;
            r.append(row[j].toDouble(&ok));
            if(!ok){
                deb<<"Load row wrong!The row maybe has string can't convert to double!";
                return false;
            }
        }
    return data->insertRow(i,r);
}

void StandardData::saveRow(int i, QStringVector& row){
    row.clear();
    for(int j=0;j<colNum;++j)
        row.append(QString::number(get(i,j)));
}

void StandardData::test(){
    StandardData stdData;
    stdData.loadCsv("E:/scientific research/experiment/data/Digit Recognizer/train.csv",1,0);
    stdData.saveCsv("E:/scientific research/experiment/data/Digit Recognizer/train1.csv");
}
