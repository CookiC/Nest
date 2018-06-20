#ifndef DYNAMICTABLE_H
#define DYNAMICTABLE_H

#include "header.h"

template <typename T>
class DynamicTable{
private:
    QStack<int> colReclaim;
    QStack<int> rowReclaim;
    QVector<int> colIndex;
    QVector<int> rowIndex;
    QVector<QVector<T>> data;

protected:
    int colNum;
    int rowNum;

public:
    DynamicTable();

    void deleteCol(int);
    void deleteRow(int);
    bool insertCol(int, const QVector<T>&);
    bool insertRow(int, const QVector<T>&);

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

    inline bool appendCol(const QVector<T>& col){
        return insertCol(colNum,col);
    }

    inline bool appendRow(const QVector<T>& row){
        return insertRow(rowNum,row);
    }

};

#endif // NTABLE_H
