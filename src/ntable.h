#ifndef NTABLE_H
#define NTABLE_H

#include "header.h"

template <typename T>
class NTable{
private:
    int colMax;
    int rowMax;
    T **data;
    NTable<T> *parent;
    QVector<NTable<T>*> son;

protected:
    int colNum;
    int rowNum;

public:
    NTable();
    NTable(int rowNum, int colNum);
    ~NTable();

    void deleteCol(int i);
    void deleteRow(int i);
    bool insertCol(int i, const QVector<T>& col);
    bool insertRow(int i, const QVector<T>& row);
    QVector<T> getCol(int j);
    QVector<T> getRow(int i);

    //inline
    T& at(int i,int j);
    bool appendCol(const QVector<T>& col);
    bool appendRow(const QVector<T>& row);
    const T& get(int i,int j) const;
    int getColNum();
    const T* getCRow(int i);
    int getRowNum();

    T* operator [] (int i);
    const T* operator [] (int i)const;
};

template<typename T>
NTable<T>::NTable():colNum(0),rowNum(0),colMax(0),rowMax(0),data(nullptr),parent(nullptr){
}

template<typename T>
NTable<T>::NTable(int rowMax, int colMax):colNum(0),rowNum(0),colMax(colMax),rowMax(rowMax),data(nullptr),parent(nullptr){
    data = new *T[rowMax];
    for(int i=0;i<rowMax;++i)
        data[i] = new T[colMax];
}


template<typename T>
NTable<T>::~NTable(){
    int i;
    for(i=0;i<rowNum;++i)
        delete[] data;
    delete[] data;
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
    if(index>colNum||rowNum!=col.size())
        return false;
    int i,j;
    if(colNum>=colMax){
        int l = colMax+(colMax>>1);
        T *r;
        for(i=0;i<rowNum;++i){
            r = new T[l];
            for(j=0;j<colNum;++j)
                r[j] = data[i][j];
            delete[] data[i];
            data[i] = r;
        }
        colMax = l;
    }

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
    if(index>rowNum||colNum!=row.size())
        return false;
    int i;
    if(rowNum>=rowMax){
        int l = rowMax+(rowMax>>1);
        T **d = new T*[l];
        for(i=0;i<rowNum;++i)
            d[i] = data[i];
        delete[] data;
        data = d;
        rowMax = l;
    }

    for(i=rowNum;i>index;--i)
        data[i] = data[i-1];
    data[index] = new T[colNum];
    for(i=0;i<colNum;++i)
        data[index][i]=row[i];
    ++rowNum;
    return true;
}

template<typename T>
QVector<T> NTable<T>::getCol(int j){
    QVector<T> col;
    for(int i=0;i<rowNum;++i)
        col.append(date[i][j]);
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
inline T& NTable<T>::at(int i,int j){
    return data[i][j];
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
#endif // NTABLE_H
