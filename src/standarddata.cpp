#include "standarddata.h"

//public
StandardData::StandardData(int rowNum, int colNum):nomName(nullptr),AbstractData(rowNum,colNum){
}

StandardData::~StandardData(){
    if(nomName)
        delete[] nomName;
}

void StandardData::cutCol(StandardData *des, StandardData *src, int index){
    NTable<Category>::cutCol(&(des->flag),&(src->flag),index);
    AbstractData::cutCol(des,src,index);
}

//protect

bool StandardData::loadRow(int index, const QStringVector &row){
    bool ok;
    QVector<double> dataRow;
    QVector<Category> flagRow;
    for(int j=0;j<row.size();++j)
        if(row[j].isEmpty())
            flagRow.append(MISS);
        else{
            flagRow.append(NUM);
            dataRow.append(row[j].toDouble(&ok));
            if(!ok){
                deb<<"Load row wrong!The row maybe has string can't convert to double!";
                return false;
            }
        }
    return flag.insertRow(index,flagRow)&&data.insertRow(index,dataRow);
}

void StandardData::saveRow(int i, QStringVector& row){
    row.clear();
    for(int j=0;j<colNum;++j)
        row.append(QString::number(get(i,j)));
}

//private

//test
void StandardData::test(){
    StandardData stdData;
    stdData.loadCsv("E:/scientific research/experiment/data/Digit Recognizer/train.csv",1,0);
    stdData.saveCsv("E:/scientific research/experiment/data/Digit Recognizer/train1.csv");
}
