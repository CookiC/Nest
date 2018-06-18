#ifndef DYNAMICTABLE_H
#define DYNAMICTABLE_H

#include "header.h"

template <typename T>
class DynamicTable{
private:
    int colNum;
    int rowNum;
    QQueue<int> colReclaim;
    QQueue<int> rowReclaim;
    QList<int> colIndex;
    QList<int> rowIndex;
    QList<QList<T>> data;

public:
    DynamicTable();

    bool appendCol(const QList<T>&);
    bool appendRow(const QList<T>&);
    void deleteCol(int);
    void deleteRow(int);
    bool insertCol(int, const QList<T>&);
    bool insertRow(int, const QList<T>&);

    inline int getColNum(){
        return colNum;
    }

    inline int getRowNum(){
        return rowNum;
    }

    inline T& at(int i,int j){
        return data[rowIndex[i]][colIndex[j]];
    }

    inline const T& at(int i,int j) const{
        return data[rowIndex[i]][colIndex[j]];
    }
};

#endif // NTABLE_H
