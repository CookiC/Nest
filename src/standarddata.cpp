#include "standarddata.h"

//public

StandardData::StandardData():
    missing(nullptr),attrCategory(nullptr),nomName(nullptr){
}

StandardData::~StandardData(){
    if(missing){
        for(int i=0;i<rowNum;++i)
            delete[] missing[i];
        delete[] missing;
    }
    if(attrCategory)
        delete[] attrCategory;
    if(nomName)
        delete[] nomName;
}

void StandardData::setMissing(int i,int j){
    if(!missing){
        missing = new bool*[rowNum];
        for(int k=0;k<rowNum;++k){
            missing[k] = new bool[colNum];
            memset(missing, 0, sizeof(missing[k]));
        }
    }
    missing[i][j]=true;
}

bool StandardData::loadRow(int i, const QStringVector &row){
    bool ok;
    QVector<double> r;
    for(QString s:row){
        r.append(s.toDouble(&ok));
        if(!ok){
            deb<<"Has string!";
            return false;
        }
    }
    return NTable::insertRow(i,r);
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
