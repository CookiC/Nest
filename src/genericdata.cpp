#include "genericdata.h"

//public:

GenericData::GenericData(){
}

GenericData::~GenericData(){
}

void GenericData::colStrSplit(int index, const QRegularExpression &regExp){
    int i,j=index;
    int size=0;
    QString name = getColName(index);
    QStringList *row = new QStringList[rowNum];
    QStringVector col;

    for(i=0;i<rowNum;++i){
        row[i] = get(i,j).split(regExp);
        if(row[i].size()>size)
            size = row[i].size();
    }
    deleteCol(index);

    for(i=0;i<size;++i){
        col.clear();
        for(j=0;j<rowNum;++j){
            if(row[j].size()-size+i>=0)
                col.append(row[j][row[j].size()-size+i]);
            else
                col.append("");
        }
        insertCol(index+i,col,name);
    }
    delete[] row;
}

void GenericData::colStrSplit(int index, const QString& delimiter, int rank){
    QString name = getColName(index);
    QStringVector list;
    int i;
    int j = index;
    int k = delimiter.size();
    int l,cnt;

    for(i=0;i<rowNum;++i){
        l=-1;
        cnt=0;
        do{
            l=get(i,j).indexOf(delimiter,l+1);
            ++cnt;
        }while(cnt<rank&&l>=0);
        if(l==-1)
            list.append("");
        else{
            list.append(get(i,j).left(j));
            data[i][j].remove(0,j+k);
        }
    }
    insertCol(index,list,name);
}

bool GenericData::loadRow(int i, const QStringVector &row){
    if(!data.insertRow(i,row))
        return false;
    return true;
}

void GenericData::saveRow(int i, QStringVector& row){
    row.clear();
    for(int j=0;j<colNum;++j)
        if(get(i,j).contains(','))
            row.append("\""+get(i,j)+"\"");
        else
            row.append(get(i,j));
}

/*
StandardData& GenericData::toStandardData(){
    bool ok;
    int i,j;
    StandardData *stdData=new StandardData(rowNum, colNum);
    if(colNameFlag)
        stdData->setColName(colName);
    if(rowNameFlag)
        stdData->setRowName(rowName);

    for(j=0;j<data.getColNum();++j){
        stdData->setNumerical(j);
        for(i=0;i<data.getRowNum();++i){
            if(data.at(i,j).isEmpty())
                stdData->setMissing(i,j);
            else{
                if(stdData->isNumerical(j)){
                    stdData->at(i,j)=data.at(i,j).toDouble(&ok);
                    if(!ok)
                        stdData->setNominal(j);
                }
            }
        }
    }
    QMap<QString, int> mp;
    for(j=0;j<data.getColNum();++j){
        if(stdData->isNominal(j)){
            mp.clear();
            for(i=0;i<data.getRowNum();++i){
                QString &s=data.at(i,j);
                if(mp.contains(s))
                    stdData->at(i,j)=mp.value(s);
                else{
                    stdData->at(i,j)=mp.size();
                    stdData->addNomName(j,s);
                    mp.insert(s,mp.size());
                }
            }
        }
    }
    deb<<"Transform success！";
    return *stdData;
}*/

void GenericData::test(){
    GenericData genData;
    genData.loadCsv("E:/scientific research/experiment/data/Digit Recognizer/test.csv",1,0);
    //genData.saveCsv("E:/scientific research/experiment/data/Digit Recognizer/test1.csv");
}
