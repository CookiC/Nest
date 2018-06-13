#ifndef STANDARDDATA_H
#define STANDARDDATA_H

#include "header.h"
#include "strictdata.h"

class StandardData: public StrictData{
    enum DataType{NUM,NOM};
public:
    StandardData(int, int);
    ~StandardData();

private:
    bool **missing;
    DataType *type;
    QStringList rowName;
    QStringList colName;
    QStringList *nomName;
};

#endif // STANDARDDATA_H
