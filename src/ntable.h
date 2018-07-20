#ifndef NTABLE_H
#define NTABLE_H

#include "header.h"
#include "ntableblock.h"

template <typename T>
class NTable{
private:
    QVector<uint> colIndex;
    QVector<uint> rowIndex;
    NTableBlock<T> *data;

    NTable(NTableBlock<T> *data);

public:
    NTable(uint rowNum = 0, uint colNum = 0);
    ~NTable();

    bool appendCol(const QVector<T>& col);
    bool appendRow(const QVector<T>& row);
    T& at(int i, int j);
    NTableBlock<T>* cutCol(uint index);
    NTableBlock<T>* cutRow(uint index);
    void deleteCol(uint index);
    void deleteRow(uint index);
    const T& get(int i,int j) const;
    QVector<T> getRow(int index);
    bool insertCol(int index, const QVector<T>& col);
    bool insertRow(int index, const QVector<T>& row);
    NTableBlock<T>* select(const QVector<T>& rowRank,const QVector<T>& colRank);
};

//public

template <typename T>
NTable<T>::NTable(uint rowNum, uint colNum){
    data = new NTableBlock<T>(rowNum, colNum);
    colIndex.resize(colNum);
    rowIndex.resize(rowNum);
    uint i;
    for(i=0;i<colNum;++i)
        data->addColQuote(i);
    for(i=0;i<rowNum;++i)
        data->addRowQuote(i);
}

template <typename T>
bool NTable<T>::appendCol(const QVector<T>& col){
    if(!data->appendCol(col))
        return false;
    uint n = data->getColNum()-1;
    data->addColQuote(n);
    colIndex.append(n);
    return true;
}

template <typename T>
bool NTable<T>::appendRow(const QVector<T>& row){
    if(!data->appendRow(row))
        return false;
    uint n = data->getRowNum()-1;
    data->addRowQuote(n);
    rowIndex.append(n);
    return true;
}

template <typename T>
inline T& NTable<T>::at(int i, int j){
    return data->at(i,j);
}

template <typename T>
NTableBlock<T>* NTable<T>::cutCol(uint index){
    NTable<T> *cut = new NTable(data,rowNum,colNum);
    memcpy(cut->rowIndex,rowIndex,sizeof(uint)*rowNum);
    cut->colIndex[0] = colIndex[index];
    memmove(colIndex+index,colIndex+index+1,sizeof(uint)*(colNum-index-1));
    data->addColQuote(index);
    return cut;
}

template <typename T>
NTableBlock<T>* NTable<T>::cutRow(uint index){
    NTable<T> *cut = new NTable(data,rowNum,colNum);
    memcpy(cut->colIndex, colIndex, sizeof(uint)*colNum);
    cut->rowIndex[0] = rowIndex[index];
    memmove(rowIndex+index, rowIndex+index+1, sizeof(uint)*(rowNum-index-1));
    data->addColQuote(index);
    return cut;
}

template <typename T>
inline void NTable<T>::deleteCol(uint index){
    data->deleteColQuote(colIndex[index]);
    colIndex.remove(index);
}

template <typename T>
inline void NTable<T>::deleteRow(uint index){
    data->deleteRowQuote(rowIndex[index]);
    rowIndex.remove(index);
}

template <typename T>
inline const T& NTable<T>::get(int i, int j) const{
    return data->get(i,j);
}

template <typename T>
inline QVector<T> NTable<T>::getRow(int index){
    return data->getRow(index);
}

template <typename T>
bool NTable<T>::insertCol(int index, const QVector<T>& col){
    if(!data->appendCol(col))
        return false;
    uint n = data->getColNum()-1;
    data->addColQuote(n);
    colIndex.insert(index, n);
    return true;
}

template <typename T>
bool NTable<T>::insertRow(int index, const QVector<T>& row){
    if(!data->appendRow(row))
        return false;
    uint n = data->getRowNum()-1;
    data->addRowQuote(n);
    rowIndex.insert(index, n);
    return true;
}

template <typename T>
NTableBlock<T>* NTable<T>::select(const QVector<T> &rowRank, const QVector<T> &colRank){
    return nullptr;
}

//private

template <typename T>
NTable<T>::NTable(NTableBlock<T> *data):data(data){
}

#endif // NTABLE_H
