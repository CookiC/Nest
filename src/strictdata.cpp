#include "strictdata.h"

StrictData::StrictData(int numRow, int numCol):numRow(numRow),numCol(numCol){
    data = new double*[numRow];
    for(int i=0;i<numRow;++i)
        data[i] = new double[numCol];
}

StrictData::~StrictData(){
    for(int i=0;i<numRow;++i)
        delete[] data[i];
    delete[] data;
}

int StrictData::getNumRow(){
    return numRow;
}

int StrictData::getNumCol(){
    return numCol;
}
