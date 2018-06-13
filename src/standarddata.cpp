#include "standarddata.h"

StandardData::StandardData(int numRow, int numCol):StrictData(numRow,numCol){
    type = new DataType[numCol];
    nomName = new QStringList[numCol];
    missing = new bool*[numRow];
    for(int i=0;i<numRow;++i)
        missing[i] = new bool[numCol];
}

StandardData::~StandardData(){
    for(int i=0;i<numRow;++i)
        delete[] missing[i];
    delete[] missing;
    delete[] type;
}
