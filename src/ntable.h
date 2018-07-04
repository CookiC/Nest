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
    NTable(int rowNum, int colNum);
    ~NTable();

    bool appendCol(const QVector<T>& col);
    bool appendRow(const QVector<T>& row);
    NTable<T>* cutCol(int colIndex);
    NTable<T>* cutCol(const QVector<int> &colIndex);
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

    T*          operator [] (int i);
    const T*    operator [] (int i)const;
};

//public

template<typename T>
NTable<T>::NTable():colNum(0),rowNum(0),colMax(0),rowMax(0),data(nullptr){
}

template<typename T>
NTable<T>::NTable(int rowNum, int colNum):colNum(colNum),rowNum(rowNum),colMax(colNum),rowMax(rowNum),data(nullptr){
    data = new *T[rowNum];
    for(int i=0;i<rowNum;++i)
        data[i] = new T[colNum];
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
inline bool NTable<T>::appendCol(const QVector<T>& col){
    return insertCol(colNum,col);
}

template<typename T>
inline bool NTable<T>::appendRow(const QVector<T>& row){
    return insertRow(rowNum,row);
}

template<typename T>
NTable<T>* NTable<T>::cutCol(int colIndex){
    int i,j;
    NTable<T> *cut = new NTable<T>(rowNum,1);
    for(i=0;i<rowNum;++i){
        cut[i][0] = data[i][colIndex];
        for(j=colIndex;j+1<colNum;++j)
            data[i][j]=data[i][j+1];
    }
    --colNum;
    return cut;
}

template<typename T>
NTable<T>* NTable<T>::cutCol(const QVector<int> &colIndex){
    int i,j,k;
    int cutSize = colIndex.size();
    NTable<T> *cut = new NTable<T>(rowNum,cutSize);
    qSort(colIndex.begin(),colIndex.end());
    for(i=0;i<rowNum;++i){
        for(j=0;j<cutSize;++j)
            cut[i][j] = data[i][colIndex[j]];
        k=0;
        for(j=colIndex[0];j+k<colNum;++j){
            if(k<cutSize&&j==colIndex[k])
                ++k;
            data[i][j]=data[i][j+k];
        }
    }
    colNum -= cutSize;
    return cut;
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
    if(data){
        if(rowNum<rowMax)
            moveRow(rowNum);
        if(colNum<colMax)
            moveCol(colNum);
    }
}
template<typename T>
void NTable<T>::resize(int rowSize, int colSize){
    if(rowSize>rowMax){
        moveRow(rowSize);
        rowNum = rowSize;
    }
    else
        rowNum = rowSize;
    if(colSize>colNum){
        moveCol(colSize);
        colNum = colSize;
    }
    else
        colNum = colSize;
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
