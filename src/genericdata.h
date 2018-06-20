#ifndef GENERICDATA_H
#define GENERICDATA_H

#include "header.h"
#include "standarddata.h"
#include "dynamictable.h"

class StandardData;

class GenericData{
private:
    bool colNameFlag;
    bool rowNameFlag;
    QVector<int> colNameCnt;
    QVector<int> rowNameCnt;
    QStringVector colName;
    QStringVector rowName;
    DynamicTable<QString> data;

    void insertColName(int, const QString &);
    void insertRowName(int, const QString &);
    void deleteColName(int);
    void deleteRowName(int);
    QStringVector splitCsvLine(const QByteArray &, QString&);

public:
    GenericData();
    ~GenericData();

    bool appendCol(const QStringVector &, const QString & = "");
    bool appendRow(const QStringVector &, const QString & = "");
    void colStrSplit(int, const QRegularExpression &);
    void colStrSplit(int, int, const QString &);
    bool deleteCol(int);
    bool deleteRow(int);
    int getColIndex(const QString &);
    bool insertCol(int, const QStringVector &, const QString & = "");
    bool insertRow(int, const QStringVector &, const QString & = "");


    inline int getColNum(){
        return data.getColNum();
    }

    inline int getRowNum(){
        return data.getRowNum();
    }

    inline QString& at(int i,int j){
        return data.at(i,j);
    }

    inline const QString& at(int i,int j) const{
        return data.at(i,j);
    }
};

#endif // GENERICDATA_H
