#ifndef STANDARDDATA_H
#define STANDARDDATA_H

#include "header.h"
#include "genericdata.h"

class StandardData{
    friend class GenericData;
public:
    //数据类型分为连续型和离散型
    enum DataType{NUM,NOM};

private:
    int numRow;
    int numCol;
    double **data;
    bool isMissing;
    bool **missing;
    DataType *type;
    QStringList colName;
    QStringList rowName;
    QStringList *nomName;

public:
    StandardData(int, int, bool=1);
    ~StandardData();

    void saveCsv(const QString&);

    inline void setMissing(int i,int j, bool b){
        missing[i][j]=b;
    }

    inline double** getData(){
        return data;
    }

    inline bool getIsMissing(){
        return  isMissing;
    }

    inline int getNumRow(){
        return numRow;
    }

    inline int getNumCol(){
        return numCol;
    }

    inline bool getMissing(int i,int j){
        return isMissing?missing[i][j]:false;
    }

    inline DataType getType(int i){
        return type[i];
    }

    inline double*& operator[](const int i){
        return data[i];
    }

    inline const double* operator[](const int i)const {
        return data[i];
    }

private:

};

#endif // STANDARDDATA_H
