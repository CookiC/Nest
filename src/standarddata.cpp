#include "standarddata.h"

//public

StandardData::StandardData(int numRow, int numCol, bool isMissing):
    numRow(numRow),numCol(numCol),isMissing(isMissing){
    type = new DataType[numCol];
    nomName = new QStringList[numCol];
    data = new double*[numRow];
    for(int i=0;i<numRow;++i)
        data[i] = new double[numCol];
    if(isMissing){
        missing = new bool*[numRow];
        for(int i=0;i<numRow;++i)
            missing[i] = new bool[numCol];
    }
    else
        missing = nullptr;
}

StandardData::~StandardData(){
    for(int i=0;i<numRow;++i){
        delete[] missing[i];
        delete[] data[i];
    }
    delete[] data;
    delete[] type;
    delete[] missing;
    delete[] nomName;
}

void StandardData::saveCsv(const QString& path){
    int i,j;
    QFile file(path);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out(&file);
    deb<<numCol<<' '<<numRow<<'\n';
    if(colName.size()){
        if(rowName.size())
            out<<',';
        out<<colName[0];
        for(i=1;i<numCol;++i)
            out<<','<<colName[i];
        out<<'\n';
    }
    for(i=0;i<numRow;++i){
        if(rowName.size())
            out<<rowName[i]<<',';
        out<<data[i][0];
        for(j=1;j<numCol;++j)
            if(missing[i][j])
                out<<',';
            else
                out<<','<<data[i][j];
        out<<'\n';
    }
}
