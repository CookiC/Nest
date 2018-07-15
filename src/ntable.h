#ifndef NTABLE_H
#define NTABLE_H

#include "header.h"
#include "ntableblock.h"

template <typename T>
class NTable{
private:
    uint colNum;
    uint rowNum;
    uint *colIndex;
    uint *rowIndex;
    NTableBlock<T> *data;

    NTable(NTableBlock<T> *data, uint rowNum = 0, uint colNum = 0);

public:
    NTable(uint rowNum = 0, uint colNum = 0);
    ~NTable();

    NTableBlock<T>* select(const QVector<T>& rowRank,const QVector<T>& colRank);
    NTableBlock<T>* cutCol(uint colRank);
};

//public

template <typename T>
NTable<T>::NTable(uint rowNum, uint colNum):colNum(colNum),rowNum(rowNum){
    data = new NTableBlock(rowNum, colNum);
    colIndex = new uint[rowNum];
    rowIndex = new uint[colNum];
    int i;
    for(i=0;i<colIndex;++i)
        colIndex[i] = i;
    for(i=0;i<rowIndex;++i)
        rowIndex[i] = i;
}

template <typename T>
NTable<T>* NTable<T>::select(const QVector<T> &rowRank, const QVector<T> &colRank){
    return nullptr;
}

template <typename T>
NTable<T>* NTable<T>::cutCol(uint colRank){
    NTable<T> *cut = new NTable(data,rowNum,colNum);
    memcpy(cut->rowIndex,rowIndex,sizeof(uint)*rowNum);
    cut->colIndex[0]=colIndex[colRank];
    memmove(colIndex+colRank,colIndex+colRank+1,sizeof(uint)*(colNum-colRank-1));
    return cut;
}

//private

template <typename T>
NTable<T>::NTable(NTableBlock<T> *data, uint rowNum, uint colNum):data(data),colNum(colNum),rowNum(rowNum){
    colIndex = new uint[rowNum];
    rowIndex = new uint[colNum];
}

#endif // NTABLE_H
