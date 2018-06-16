#include "standarddata.h"

StandardData::StandardData(int numRow, int numCol){
    type = new DataType[numCol];
    nomName = new QStringList[numCol];
    missing = new bool*[numRow];
    data = new double*[numRow];
    for(int i=0;i<numRow;++i){
        missing[i] = new bool[numCol];
        data[i] = new double[numCol];
    }
}

StandardData::~StandardData(){
    for(int i=0;i<numRow;++i){
        delete[] missing[i];
        delete[] data[i];
    }
    delete[] missing;
    delete[] data;
    delete[] type;
}
