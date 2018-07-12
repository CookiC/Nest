#ifndef ABSTRACTTABLE_H
#define ABSTRACTTABLE_H

#include "header.h"

template <typename T>
class AbstractTable{
protected:
    int colNum;
    int rowNum;
    T **data;

public:
    bool appendCol(const QVector<T>& col);
    bool appendRow(const QVector<T>& row);
    T& at(const int &i, const int &j);
    void getColNum();
    void getRowNum();
    QVector<T> getCol(int index);
    const T* getConstRow(int index);
    QVector<T> getRow(int index);
    T* getVarRow(int index);

    virtual void deleteCol(int index) = 0;
    virtual void deleteRow(int index) = 0;
    virtual bool insertCol(int i, const QVector<T>& col) = 0;
    virtual bool insertRow(int i, const QVector<T>& row) = 0;
};

template <typename T>
inline bool AbstractTable<T>::appendCol(const QVector<T>& col){
    insertCol(colNum, col);
}

template <typename T>
inline bool AbstractTable<T>::appendRow(const QVector<T>& row){
    insertRow(rowNum, row);
}

template<typename T>
inline T& AbstractTable<T>::at(const int &i,const int &j){
    return data[i][j];
}

template <typename T>
inline int AbstractTable<T>::getColNum(){
    return colNum;
}

template <typename T>
inline int AbstractTable<T>::getRowNum(){
    return rowNum;
}

template <typename T>
inline const T* AbstractTable<T>::getConstRow(int index){
    return data+index;
}

template <typename T>
inline T* AbstractTable<T>::getVarRow(int index){
    return data+index;
}

template<typename T>
QVector<T> AbstractTable<T>::getCol(int index){
    QVector<T> col;
    for(int i=0;i<rowNum;++i)
        col.append(data[i][index]);
    return col;
}

template<typename T>
QVector<T> AbstractTable<T>::getRow(int index){
    QVector<T> row;
    for(int j=0;j<colNum;++j)
        row.append(data[index][j]);
    return row;
}

#endif // ABSTRACTTABLE_H
