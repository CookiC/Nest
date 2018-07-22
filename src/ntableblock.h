﻿#ifndef NTABLEBLOCK_H
#define NTABLEBLOCK_H

#include "header.h"

template <typename T>
class NTableBlock{
private:
    T **data;
    uint colMax;
    uint colNum;
    uint colDel;
    uint *colQuote;
    uint rowMax;
    uint rowNum;
    uint rowDel;
    uint *rowQuote;

    void create(int rowMax,int colMax);
    void move(int newRowMax,int newColMax);
    void moveColBackward(int index);
    void moveRowBackward(int index);
    void moveCol(uint colMax);
    void moveRow(uint rowMax);

public:
    NTableBlock(uint rowNum = 0, uint colNum = 0);
    ~NTableBlock();

    void addColQuote(uint index);
    void addRowQuote(uint index);
    bool appendCol(const QVector<T>& col);
    bool appendRow(const QVector<T>& row);
    T& at(const int &i, const int &j);
    void deleteColQuote(uint index);
    void deleteRowQuote(uint index);
    const T& get(int i,int j) const;
    QVector<T> getCol(int j);
    int getColNum();
    const T* getConstRow(int i);
    QVector<T> getRow(uint index);
    uint getRowNum();
    T* getVarRow(int index);
    void release();
    void reserve(int rowMax, int colMax);
    void resize(int rowSize, int colSize);
    void set(int i, int j, T value);

    T* operator [] (int i);
    const T* operator [] (int i)const;
};

//public

template<typename T>
NTableBlock<T>::NTableBlock(uint rowNum, uint colNum){
    uint i;
    this->colNum = colNum;
    this->rowNum = rowNum;
    if(!colNum)
        colMax = 1;
    else
        colMax = colNum;
    if(!rowNum)
        rowMax = 1;
    else
        rowMax = rowNum;
    colDel = 0;
    rowDel = 0;

    rowQuote = new uint[rowMax];
    for(i=0;i<rowNum;++i)
        rowQuote[i] = 1;
    colQuote = new uint[colMax];
    for(i=0;i<colNum;++i)
        colQuote[i] = 1;
    data = new T*[rowMax];
    for(i=0;i<rowNum;++i)
        data[i] = new T[colMax];
}


template<typename T>
NTableBlock<T>::~NTableBlock(){
    for(int i=0;i<rowNum;++i)
        delete[] data[i];
    delete[] data;
    deb<<"NTableBlock delete";
}

template<typename T>
inline void NTableBlock<T>::addColQuote(uint index){
    ++colQuote[index];
}

template<typename T>
inline void NTableBlock<T>::addRowQuote(uint index){
    ++rowQuote[index];
}

template<typename T>
inline bool NTableBlock<T>::appendCol(const QVector<T>& col){
    if(!rowNum){
        moveRow(col.size());
        rowNum = col.size();
    }
    if(rowNum!=col.size())
        return false;
    if(colNum>=colMax)
        moveCol(colMax+qMax(1u,colMax>>1));
    colQuote[colNum] = 0;
    for(uint i=0;i<rowNum;++i)
        data[i][colNum] = col[i];
    ++colNum;
    return true;
}

template<typename T>
inline bool NTableBlock<T>::appendRow(const QVector<T>& row){
    if(!colNum){
        moveCol(row.size());
        colNum = row.size();
    }
    if(colNum!=row.size())
        return false;
    if(rowNum>=rowMax)
        moveRow(rowMax+qMax(1u,rowMax>>1));
    if(!data[rowNum])
        return false;
    data[rowNum] = new T[colMax];
    rowQuote[rowNum] = 0;
    memmove(data[rowNum],row.data(),sizeof(T)*colNum);
    ++rowNum;
    return true;
}

template<typename T>
inline T& NTableBlock<T>::at(const int &i,const int &j){
    return data[i][j];
}

template<typename T>
void NTableBlock<T>::deleteColQuote(uint index){
    --colQuote[index];
    ++colDel;
    if(colNum-colDel<=(colMax>>1))
        moveCol((colMax>>1)+(colMax>>2));
}

template<typename T>
void NTableBlock<T>::deleteRowQuote(uint index){
    for(int i=index;i<rowNum;++i)
        memmove(data+i,data+i+1,sizeof(T)*colNum);
    --rowNum;
}

template<typename T>
inline const T& NTableBlock<T>::get(int i,int j) const{
    return data[i][j];
}

template<typename T>
QVector<T> NTableBlock<T>::getCol(int j){
    QVector<T> col;
    for(int i=0;i<rowNum;++i)
        col.append(data[i][j]);
    return col;
}

template<typename T>
QVector<T> NTableBlock<T>::getRow(uint index){
    QVector<T> row;
    for(uint j=0;j<colNum;++j)
        row.append(data[index][j]);
    return row;
}

template <typename T>
inline T* NTableBlock<T>::getVarRow(int index){
    return data+index;
}

template<typename T>
void NTableBlock<T>::release(){
    if(pool&&(rowNum<rowMax||colNum<colMax))
        move(rowNum,colNum);
}

template<typename T>
void NTableBlock<T>::reserve(int rowMax, int colMax){

    if(rowSize>rowMax||colSize>colMax)
        move(rowSize,colSize);
    rowNum = qMin(rowNum,rowSize);
    colNum = qMin(colNum,colSize);
}

template<typename T>
void NTableBlock<T>::resize(int rowSize, int colSize){
    if(rowSize>rowMax||colSize>colMax)
        move(rowSize,colSize);
    rowNum = qMin(rowNum,rowSize);
    colNum = qMin(colNum,colSize);
}

template<typename T>
inline int NTableBlock<T>::getColNum(){
    return colNum;
}

template<typename T>
inline const T* NTableBlock<T>::getConstRow(int i){
    return data[i];
}

template<typename T>
inline uint NTableBlock<T>::getRowNum(){
    return rowNum;
}

template<typename T>
inline void NTableBlock<T>::set(int i, int j, T value){
    data[i][j]=value;
}

template<typename T>
inline T* NTableBlock<T>::operator [] (int i){
    return data[i];
}

template<typename T>
inline const T* NTableBlock<T>::operator [] (int i)const{
    return data[i];
}

//private

template<typename T>
void NTableBlock<T>::moveCol(uint colMax){
    if(colMax < 1)
        colMax = 1;
    if(colMax == this->colMax)
        return;
    uint i,j,k;
    T *row;
    for(i=0;i<rowNum;++i){
        k=0;
        row = new T[colMax];
        for(j=0;j+k<colNum;++j){
            if(!colQuote[j])
                ++k;
            row[j] = data[i][j+k];
        }
        delete[] data[i];
        data[i] = row;
    }

    uint *quote = new uint[colMax];
    if(colDel){
        for(j=0;j+k<colNum;++j){
            if(!colQuote[j])
                ++k;
            quote[j] = colQuote[j+k];
        }
        colNum -= colDel;
        colDel = 0;
        memset(quote+colNum, 0,sizeof(uint)*(colMax-colNum));
        delete[] colQuote;
        colQuote = quote;
    }
    else{
        memcpy(quote, colQuote,sizeof(uint)*colNum);
        memset(quote+colNum, 0,sizeof(uint)*(colMax-colNum));
        delete[] colQuote;
        colQuote = quote;
    }
    this->colMax = colMax;
}

template<typename T>
void NTableBlock<T>::moveRow(uint rowMax){
    if(rowMax < 1)
        rowMax = 1;
    if(rowMax == this->rowMax)
        return;
    uint i,k;
    T **data = new T*[rowMax];
    k=0;
    for(i=0;i+k<rowNum;++i){
        if(!rowQuote[i])
            ++k;
        data[i] = this->data[i+k];
    }
    delete[] this->data;
    this->data = data;

    uint *quote = new uint[rowMax];
    if(rowDel){
        for(i=0;i+k<rowNum;++i){
            if(!rowQuote[i])
                ++k;
            quote[i] = rowQuote[i+k];
        }
        rowNum -= rowDel;
        rowDel = 0;
        memset(quote+rowNum, 0,sizeof(uint)*(rowMax-rowNum));
        delete[] rowQuote;
        rowQuote = quote;
    }
    else{
        memcpy(quote,rowQuote,sizeof(uint)*rowNum);
        memset(quote+rowNum, 0,sizeof(uint)*(rowMax-rowNum));
        delete[] rowQuote;
        rowQuote = quote;
    }
    this->rowMax = rowMax;
}

template<typename T>
void NTableBlock<T>::move(int rowMax,int colMax){
    this->rowMax = rowMax;
    this->colMax = colMax;
}

template<typename T>
void NTableBlock<T>::moveColBackward(int index){
    memmove(colQuote+index+1,colQuote+index,sizeof(uint)*(colNum-index));
    for(uint i=0;i<rowNum;++i)
        memmove(data[i]+index+1,data[i]+index,sizeof(T)*(colNum-index));
    ++colNum;
}

template<typename T>
void NTableBlock<T>::moveRowBackward(int index){
    memmove(rowQuote+index+1,rowQuote+index,sizeof(uint)*(rowNum-index));
    memmove(data+index+1,data+index,sizeof(T*)*(rowNum-index));
    ++rowNum;
}
#endif // NBLOCK_H
