#ifndef GENERICDATA_H
#define GENERICDATA_H

#include "header.h"
#include "abstractdata.h"
#include "standarddata.h"
#include "ntable.h"

class GenericData : public AbstractData<QString>{
protected:
    bool loadRow(int i, const QStringVector &row);
    void saveRow(int i, QStringVector& row);

public:
    GenericData();
    ~GenericData();

    static void test();

    void colStrSplit(int, const QRegularExpression &);
    void colStrSplit(int, const QString &, int = 1);
    //StandardData& toStandardData();
};

#endif // GENERICDATA_H
