#ifndef NTABLEBLOCK_H
#define NTABLEBLOCK_H

#include "header.h"

template <typename T>
class NTableBlock{
private:
    static int blockCnt;
    int id;

    T **data;
    int colMax;
    int colNum;
    int rowMax;
    int rowNum;
    int quote;

    void create(int rowMax,int colMax);
    void move(int newRowMax,int newColMax);
    void moveColBackward(int index);
    void moveRowBackward(int index);
    void moveCol(int colMax);
    void moveRow(int rowMax);

public:
    NTableBlock(int rowNum = 0, int colNum = 0);
    ~NTableBlock();

    void addQuote();
    bool appendCol(const QVector<T>& col);
    bool appendRow(const QVector<T>& row);
    T& at(const int &i, const int &j);
    void deleteQuote();
    const T& get(int i,int j) const;
    QVector<T> getCol(int j);
    int getColNum();
    const T* getConstRow(int i);
    QVector<T> getRow(int index);
    int getRowNum();
    int getQuote();
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
int NTableBlock<T>::blockCnt = 0;

template<typename T>
NTableBlock<T>::NTableBlock(int rowNum, int colNum):quote(0){
    id = blockCnt++;
    deb<<"Block "<<id<<"create!";
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
    data = new T*[rowMax];
    for(int i=0;i<rowNum;++i)
        data[i] = new T[colMax];
}


template<typename T>
NTableBlock<T>::~NTableBlock(){
    for(int i=0;i<rowNum;++i)
        delete[] data[i];
    delete[] data;
    deb<<"Block "<<id<<"delete!";
}

template<typename T>
inline void NTableBlock<T>::addQuote(){
    ++quote;
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
        moveCol(colMax+qMax(1,colMax>>1));
    for(int i=0;i<rowNum;++i)
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
        moveRow(rowMax+qMax(1,rowMax>>1));
    if(!data[rowNum])
        return false;
    data[rowNum] = new T[colMax];
    memmove(data[rowNum],row.data(),sizeof(T)*colNum);
    ++rowNum;
    return true;
}

template<typename T>
inline T& NTableBlock<T>::at(const int &i,const int &j){
    return data[i][j];
}

template<typename T>
inline void NTableBlock<T>::deleteQuote(){
    --quote;
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
QVector<T> NTableBlock<T>::getRow(int index){
    QVector<T> row;
    for(int j=0;j<colNum;++j)
        row.append(data[index][j]);
    return row;
}

template<typename T>
inline int NTableBlock<T>::getQuote(){
    return quote;
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
inline int NTableBlock<T>::getRowNum(){
    return rowNum;
}

template<typename T>
inline void NTableBlock<T>::set(int i, int j, T value){
    data[i][j] = value;
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
void NTableBlock<T>::moveCol(int colMax){
    if(colMax < 1)
        colMax = 1;
    if(colMax == this->colMax)
        return;
    T *row;
    for(int i=0;i<rowNum;++i){
        row = new T[colMax];
        memcpy(row,data[i],sizeof(T)*colNum);
        delete[] data[i];
        data[i] = row;
    }
    this->colMax = colMax;
}

template<typename T>
void NTableBlock<T>::moveRow(int rowMax){
    if(rowMax < 1)
        rowMax = 1;
    if(rowMax == this->rowMax)
        return;
    T **data = new T*[rowMax];
    memcpy(data,this->data,sizeof(T*)*rowNum);
    delete[] this->data;
    this->data = data;
    this->rowMax = rowMax;
}

template<typename T>
void NTableBlock<T>::move(int rowMax,int colMax){
    this->rowMax = rowMax;
    this->colMax = colMax;
}

template<typename T>
void NTableBlock<T>::moveColBackward(int index){
    for(int i=0;i<rowNum;++i)
        memmove(data[i]+index+1,data[i]+index,sizeof(T)*(colNum-index));
    ++colNum;
}

template<typename T>
void NTableBlock<T>::moveRowBackward(int index){
    memmove(data+index+1,data+index,sizeof(T*)*(rowNum-index));
    ++rowNum;
}
#endif // NBLOCK_H
