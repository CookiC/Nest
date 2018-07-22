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
    void deleteCol(uint index);
    void deleteRow(uint index);
    const T& get(int i,int j) const;
    uint getColNum();
    QVector<T> getRow(int index);
    uint getRowNum();
    bool insertCol(int index, const QVector<T>& col);
    bool insertRow(int index, const QVector<T>& row);
    NTableBlock<T>* select(const QVector<T>& rowRank,const QVector<T>& colRank);

    static void cutCol(NTable<T> *des, NTable<T> *src,uint index);
    static void cutRow(NTable<T> *des, NTable<T> *src,uint index);
};

//public

template <typename T>
NTable<T>::NTable(uint rowNum, uint colNum){
    data = new NTableBlock<T>(rowNum, colNum);
    colIndex.resize(colNum);
    rowIndex.resize(rowNum);
}

template <typename T>
NTable<T>::~NTable(){
}

template <typename T>
bool NTable<T>::appendCol(const QVector<T>& col){
    return insertCol(col.size(),col);
}

template <typename T>
bool NTable<T>::appendRow(const QVector<T>& row){
    return insertRow(row.size(),row);
}

template <typename T>
inline T& NTable<T>::at(int i, int j){
    return data->at(i,j);
}

template <typename T>
void NTable<T>::cutCol(NTable<T> *des, NTable<T> *src,uint index){
    des->rowIndex.clear();
    des->rowIndex.append(src->rowIndex);
    des->colIndex.append(src->colIndex[index]);
    src->colIndex.remove(index);
    des->data = src->data;
    des->data->addColQuote(index);
}

template <typename T>
void NTable<T>::cutRow(NTable<T> *des, NTable<T> *src,uint index){
    des->colIndex.clear();
    des->colIndex.append(src->colIndex);
    des->rowIndex[0] = src->rowIndex[index];
    src->rowIndex.remove(index);
    des->data = src->data;
    des->data->addRowQuote(index);
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
inline uint NTable<T>::getColNum(){
    return colIndex.size();
}

template <typename T>
inline QVector<T> NTable<T>::getRow(int index){
    return data->getRow(index);
}

template <typename T>
inline uint NTable<T>::getRowNum(){
    return rowIndex.size();
}

template <typename T>
bool NTable<T>::insertCol(int index, const QVector<T>& col){
    if(!data->appendCol(col))
        return false;
    uint n = data->getColNum()-1;
    if(rowIndex.isEmpty()){
        uint m = data->getRowNum();
        for(uint i=0;i<m;++i){
            rowIndex.append(i);
            data->addRowQuote(i);
        }
    }
    data->addColQuote(n);
    colIndex.insert(index, n);
    return true;
}

template <typename T>
bool NTable<T>::insertRow(int index, const QVector<T>& row){
    if(!data->appendRow(row))
        return false;
    if(colIndex.isEmpty()){
        uint m = data->getColNum();
        for(uint i=0;i<m;++i){
            colIndex.append(i);
            data->addColQuote(i);
        }
    }
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
