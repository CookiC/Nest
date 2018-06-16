#ifndef STANDARDDATA_H
#define STANDARDDATA_H

#include "header.h"
#include "genericdata.h"

class StandardData{
    friend class GenericData;

public:
    //数据类型分为连续型和离散型
    enum DataType{NUM,NOM};
    StandardData(int, int);
    ~StandardData();

private:
    int numRow;
    int numCol;
    double **data;
    bool **missing;
    DataType *type;
    QStringList rowName;
    QStringList colName;
    QStringList *nomName;
};

#endif // STANDARDDATA_H
