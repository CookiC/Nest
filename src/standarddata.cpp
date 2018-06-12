#include "standarddata.h"

StandardData::StandardData(int numRow, int numCol):numRow(numRow),numCol(numCol){
    int i;
    type = new DataType[numCol];
    isMissing = new bool*[numRow];
    data = new double*[numRow];
    for(i=0;i<numRow;++i){
        isMissing[i] = new bool[numCol];
        data[i] = new double[numCol];
    }
}

StandardData::~StandardData(){
    int i;
    for(i=0;i<numRow;++i){
        delete[] isMissing[i];
        delete[] data[i];
    }
    delete[] isMissing;
    delete[] data;
    delete[] type;
}

StandardData::getNumRow(){
    return numRow;
}

StandardData::getNumCol(){
    return numCol;
}
