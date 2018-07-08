#ifndef NTABLE_H
#define NTABLE_H

#include "header.h"

template <typename T>
class NTable{
private:
    int colMax;
    int rowMax;
    T *pool;
    void move(int newRowMax,int newColMax);
    void moveColBackward(int index);
    void moveRowBackward(int index);

protected:
    T **data;
    int colNum;
    int rowNum;

public:
    NTable();
    NTable(int rowNum, int colNum);
    ~NTable();

    T& at(const int &i, const int &j);
    bool appendCol(const QVector<T>& col);
    bool appendRow(const QVector<T>& row);
    void cutColFrom(NTable<T> *src, int colIndex);
    void cutColFrom(NTable<T> *src, const QVector<int> &colIndex);
    void deleteCol(int i);
    void deleteRow(int i);
    bool insertCol(int i, const QVector<T>& col);
    bool insertRow(int i, const QVector<T>& row);
    const T& get(int i,int j) const;
    QVector<T> getCol(int j);
    int getColNum();
    const T* getConstRow(int i);
    QVector<T> getRow(int i);
    int getRowNum();
    void release();
    void resize(int rowSize, int colSize);
    void set(int i, int j, T value);

    T* operator [] (int i);
    const T* operator [] (int i)const;
};

//public

template<typename T>
inline T& NTable<T>::at(const int &i,const int &j){
    return data[i][j];
}

template<typename T>
NTable<T>::NTable():colNum(0),rowNum(0),colMax(0),rowMax(0),data(nullptr),pool(nullptr){
}

template<typename T>
NTable<T>::NTable(int rowNum, int colNum):colNum(colNum),rowNum(rowNum),colMax(colNum),rowMax(rowNum),data(nullptr){
    pool = new T[rowMax*colMax];
    data = new T*[rowMax];
    for(int i=0;i<rowMax;++i)
        data[i] = &pool[i*colMax];
}


template<typename T>
NTable<T>::~NTable(){
    delete[] data;
    delete[] pool;
    deb<<"NTable delete";
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
void NTable<T>::cutColFrom(NTable<T> *src,int colIndex){
    int i,j;
    resize(src->rowNum,1);
    for(i=0;i<rowNum;++i){
        data[i][0] = src->data[i][colIndex];
        for(j=colIndex;j+1<src->colNum;++j)
            src->data[i][j]=src->data[i][j+1];
    }
    --src->colNum;
}

template<typename T>
void NTable<T>::cutColFrom(NTable<T> *src, const QVector<int> &colIndex){
    int i,j,k;
    QVector<int> tmp = colIndex;
    resize(src->rowNum,colIndex.size());
    qSort(tmp.begin(),tmp.end());
    for(i=0;i<rowNum;++i){
        for(j=0;j<colNum;++j)
            data[i][j] = src->data[i][colIndex[j]];
        k=0;
        for(j=tmp[0];j+k<src->colNum;++j){
            if(k<colNum&&j==tmp[k])
                ++k;
            src->data[i][j] = src->data[i][j+k];
        }
    }
    src->colNum -= colNum;
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
    int i,j;
    --rowNum;
    for(i=index;i<rowNum;++i)
        memcpy(data[i],data[i+1],sizeof(T)*colNum);
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
        move(rowMax,colMax+qMax(1,(colMax>>1)));

    moveColBackward(index);
    for(int i=0;i<rowNum;++i)
        data[i][index]=col[i];
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
        move(rowMax+qMax(1,(rowMax>>1)),colMax);

    moveRowBackward(index);
    memcpy(data[index],row.data(),sizeof(T)*colNum);
    return true;
}

template<typename T>
inline const T& NTable<T>::get(int i,int j) const{
    return data[i][j];
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
    if(pool&&(rowNum<rowMax||colNum<colMax))
        move(rowNum,colNum);
}
template<typename T>
void NTable<T>::resize(int rowSize, int colSize){
    if(rowSize>rowMax||colSize>colMax)
        move(rowSize,colSize);
    rowNum = qMin(rowNum,rowSize);
    colNum = qMin(colNum,colSize);
}

template<typename T>
inline int NTable<T>::getColNum(){
    return colNum;
}

template<typename T>
inline const T* NTable<T>::getConstRow(int i){
    return data[i];
}

template<typename T>
inline int NTable<T>::getRowNum(){
    return rowNum;
}

template<typename T>
inline void NTable<T>::set(int i, int j, T value){
    data[i][j]=value;
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
void NTable<T>::move(int newRowMax,int newColMax){
    rowMax = newRowMax;
    colMax = newColMax;
    int i;
    T *newPool = new T[rowMax*colMax];
    T **newData = new T*[rowMax];
    for(i=0;i<rowMax;++i)
        newData[i] = &newPool[i*colMax];
    if(pool){
        for(i=0;i<rowNum;++i)
            memcpy(newData[i],data[i],sizeof(T)*colNum);
        delete[] pool;
        delete[] data;
    }
    pool = newPool;
    data = newData;
}

template<typename T>
void NTable<T>::moveColBackward(int index){
    int i,j;
    for(i=0;i<rowNum;++i)
        for(j=colNum;j>index;--j)
            data[i][j]=data[i][j-1];
    ++colNum;
}

template<typename T>
void NTable<T>::moveRowBackward(int index){
    for(int i=rowNum;i>index;--i)
        memcpy(data[i],data[i-1],sizeof(T)*colNum);
    ++rowNum;
}
#endif // NTABLE_H
