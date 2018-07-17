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
    T& at(const int &i, const int &j);
    NTableBlock<T>* cutCol(uint colRank);
    NTableBlock<T>* cutRow(uint rowRank);
    const T& get(const int &i,const int &j) const;
    NTableBlock<T>* select(const QVector<T>& rowRank,const QVector<T>& colRank);
};

//public

template <typename T>
NTable<T>::NTable(uint rowNum, uint colNum):colNum(colNum),rowNum(rowNum){
    data = new NTableBlock(rowNum, colNum);
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
inline T& NTable<T>::at(const int &i, const int &j){
    return data->at(i,j);
}

template <typename T>
NTableBlock<T>* NTable<T>::cutCol(uint colRank){
    NTable<T> *cut = new NTable(data,rowNum,colNum);
    memcpy(cut->rowIndex,rowIndex,sizeof(uint)*rowNum);
    cut->colIndex[0] = colIndex[colRank];
    memmove(colIndex+colRank,colIndex+colRank+1,sizeof(uint)*(colNum-colRank-1));
    data->addColQuote(colRank);
    return cut;
}

template <typename T>
NTableBlock<T>* NTable<T>::cutRow(uint rowRank){
    NTable<T> *cut = new NTable(data,rowNum,colNum);
    memcpy(cut->colIndex, colIndex, sizeof(uint)*colNum);
    cut->rowIndex[0] = rowIndex[rowRank];
    memmove(rowIndex+rowRank, rowIndex+rowRank+1, sizeof(uint)*(rowNum-rowRank-1));
    data->addColQuote(rowRank);
    return cut;
}

template <typename T>
inline const T& NTable<T>::get(const int &i, const int &j) const{
    return data->get(i,j);
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
