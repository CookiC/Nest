#ifndef NTABLE_H
#define NTABLE_H

#include "header.h"

template <typename T>
class NTable{
private:
    int colMax;
    int rowMax;
    void moveCol(int newMax);
    void moveRow(int newMax);

protected:
    T **data;
    int colNum;
    int rowNum;

public:
    NTable();
    NTable(int rowMax, int colMax);
    ~NTable();

    void deleteCol(int i);
    void deleteRow(int i);
    bool insertCol(int i, const QVector<T>& col);
    bool insertRow(int i, const QVector<T>& row);
    QVector<T> getCol(int j);
    QVector<T> getRow(int i);
    void release();

    //inline
    bool appendCol(const QVector<T>& col);
    bool appendRow(const QVector<T>& row);
    const T& get(int i,int j) const;
    int getColNum();
    const T* getCRow(int i);
    int getRowNum();

    T* operator [] (int i);
    const T* operator [] (int i)const;
};

//public

template<typename T>
NTable<T>::NTable():colNum(0),rowNum(0),colMax(0),rowMax(0),data(nullptr){
}

template<typename T>
NTable<T>::NTable(int rowMax, int colMax):colNum(0),rowNum(0),colMax(colMax),rowMax(rowMax),data(nullptr){
    data = new *T[rowMax];
    for(int i=0;i<rowMax;++i)
        data[i] = new T[colMax];
}


template<typename T>
NTable<T>::~NTable(){
    int i;
    for(i=0;i<rowNum;++i)
        delete[] data[i];
    delete[] data;
    deb<<"NTable delete";
}

template<typename T>
void NTable<T>::deleteCol(int index){
    int i,j;
    --colNum;
    for(i=0;i<rowNum;++i)
        for(j=index;j<colNum;++j)
            data[i][j]=data[i][j+1];
}

template<typename T>
void NTable<T>::deleteRow(int index){
    int i;
    --rowNum;
    for(i=index;i<rowNum;++i)
        data[i]=data[i+1];
}

template<typename T>
bool NTable<T>::insertCol(int index, const QVector<T>& col){
    if(!rowMax){
        rowNum = col.size();
        rowMax = col.size();
    }
    if(index>colNum||rowNum!=col.size())
        return false;
    if(colNum>=colMax)
        moveCol(colMax+qMax(1,(colMax>>1)));

    int i,j;
    for(i=0;i<rowNum;++i){
        for(j=colNum;j>index;--j)
            data[i][j]=data[i][j-1];
        data[i][index]=col[i];
    }
    ++colNum;
    return true;
}

template<typename T>
bool NTable<T>::insertRow(int index, const QVector<T>& row){
    if(!colMax){
        colNum = row.size();
        colMax = row.size();
    }
    if(index>rowNum||colNum!=row.size())
        return false;
    if(rowNum>=rowMax)
        moveRow(rowMax+qMax(1,(rowMax>>1)));

    for(int i=rowNum-1;i>index;--i)
        data[i] = data[i-1];
    data[index] = new T[colMax];
    memcpy(data[index],row.data(),sizeof(T)*colNum);
    ++rowNum;
    return true;
}

template<typename T>
QVector<T> NTable<T>::getCol(int j){
    QVector<T> col;
    for(int i=0;i<rowNum;++i)
        col.append(data[i][j]);
    return col;
}

template<typename T>
QVector<T> NTable<T>::getRow(int i){
    QVector<T> row;
    for(int j=0;j<colNum;++j)
        row.append(data[i][j]);
    return row;
}

template<typename T>
void NTable<T>::release(){
    if(data){
        if(rowNum<rowMax)
            moveRow(rowNum);
        if(colNum<colMax)
            moveCol(colNum);
    }
}

template<typename T>
inline bool NTable<T>::appendCol(const QVector<T>& col){
    return insertCol(colNum,col);
}

template<typename T>
inline bool NTable<T>::appendRow(const QVector<T>& row){
    return insertRow(rowNum,row);
}


template<typename T>
inline const T& NTable<T>::get(int i,int j) const{
    return data[i][j];
}

template<typename T>
inline int NTable<T>::getColNum(){
    return colNum;
}

template<typename T>
inline const T* NTable<T>::getCRow(int i){
    return data[i];
}

template<typename T>
inline int NTable<T>::getRowNum(){
    return rowNum;
}

template<typename T>
inline T* NTable<T>::operator [] (int i){
    return data[i];
}

template<typename T>
inline const T* NTable<T>::operator [] (int i)const{
    return data[i];
}

//private
template<typename T>
void NTable<T>::moveCol(int newMax){
    colMax = newMax;
    T *r;
    for(int i=0;i<rowNum;++i){
        r = new T[colMax];
        memcpy(r,data[i],sizeof(T)*colNum);
        delete[] data[i];
        data[i] = r;
    }
}

template<typename T>
void NTable<T>::moveRow(int newMax){
    rowMax = newMax;
    T **d = new T*[rowMax];
    memcpy(d,data,sizeof(T*)*rowNum);
    if(data)
        delete[] data;
    data = d;
}
#endif // NTABLE_H
