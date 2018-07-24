#ifndef NTABLE_H
#define NTABLE_H

#include "header.h"
#include "ntableblock.h"

template <typename T>
class NTable{
private:
    QList<int> colIndex;
    QList<int> rowIndex;
    NTableBlock<T> *data;
    int tmp;

    NTable(NTableBlock<T> *data);
    void quickSortRow(QVector<int>::iterator b, QVector<int>::iterator e);

public:
    NTable();
    NTable(int rowNum, int colNum);
    ~NTable();

    bool appendCol(const QVector<T>& col);
    bool appendRow(const QVector<T>& row);
    void appendCol(const NTable<T>& src);
    void appendRow(const NTable<T>& src);
    T& at(int i, int j);
    void clear();
    void copy(const NTable<T> &src);
    void copyCol(const NTable<T> &src);
    void copyRow(const NTable<T> &src);
    void cutCol(NTable<T> &src, int index);
    void cutRow(NTable<T> &src, int index);
    void deleteCol(int index);
    void deleteRow(int index);
    const T& get(int i,int j) const;
    int getColNum();
    QVector<T> getRow(int index);
    int getRowNum();
    bool insertCol(int index, const QVector<T>& col);
    bool insertRow(int index, const QVector<T>& row);
    void popFrontCol();
    void popFrontRow();
    void selectCol(const NTable<T> *src, const QVector<int> index);
    void sortByCol(int index);
};

//public

template <typename T>
NTable<T>::NTable(){
    data = new NTableBlock<T>();
}

template <typename T>
NTable<T>::NTable(int rowNum, int colNum){
    data = new NTableBlock<T>(rowNum, colNum);
    colIndex.reserve(colNum);
    rowIndex.reserve(rowNum);
}

template <typename T>
NTable<T>::~NTable(){
    data->deleteQuote();
    if(!data->getQuote())
        delete data;
}

template <typename T>
inline bool NTable<T>::appendCol(const QVector<T>& col){
    return insertCol(col.size(),col);
}

template <typename T>
inline bool NTable<T>::appendRow(const QVector<T>& row){
    return insertRow(row.size(),row);
}

template <typename T>
inline void NTable<T>::appendCol(const NTable<T>& src){
    colIndex.append(src.colIndex);
}

template <typename T>
inline void NTable<T>::appendRow(const NTable<T>& src){
    rowIndex.append(src.rowIndex);
}

template <typename T>
inline T& NTable<T>::at(int i, int j){
    return data->at(i,j);
}

template <typename T>
void NTable<T>::clear(){
   rowIndex.clear();
   colIndex.clear();
}

template <typename T>
void NTable<T>::copy(const NTable<T> *src){
    colIndex.clear();
    rowIndex.clear();
    data = src->data;
    colIndex.append(src->colIndex);
    rowIndex.append(src->rowIndex);
}

template <typename T>
void NTable<T>::copyCol(const NTable<T> &src){
    colIndex.clear();
    colIndex.append(src.colIndex);
}

template <typename T>
void NTable<T>::copyRow(const NTable<T> &src){
    rowIndex.clear();
    rowIndex.append(src.rowIndex);
}

template <typename T>
void NTable<T>::cutCol(NTable<T> &src,int index){
    des->rowIndex.clear();
    des->rowIndex.append(src->rowIndex);
    des->colIndex.append(src->colIndex[index]);
    src->colIndex.removeOne(index);
    des->data = src->data;
}

template <typename T>
void NTable<T>::cutRow(NTable<T> *des, NTable<T> *src,int index){
    des->colIndex.clear();
    des->colIndex.append(src->colIndex);
    des->rowIndex[0] = src->rowIndex[index];
    src->rowIndex.removeOne(index);
    des->data = src->data;
}

template <typename T>
inline void NTable<T>::deleteCol(int index){
    colIndex.removeOne(index);
}

template <typename T>
inline void NTable<T>::deleteRow(int index){
    rowIndex.removeOne(index);
}

template <typename T>
inline const T& NTable<T>::get(int i, int j) const{
    return data->get(i,j);
}

template <typename T>
inline int NTable<T>::getColNum(){
    return colIndex.size();
}

template <typename T>
inline QVector<T> NTable<T>::getRow(int index){
    return data->getRow(index);
}

template <typename T>
inline int NTable<T>::getRowNum(){
    return rowIndex.size();
}

template <typename T>
bool NTable<T>::insertCol(int index, const QVector<T>& col){
    if(!data->appendCol(col))
        return false;
    int n = data->getColNum()-1;
    if(rowIndex.isEmpty()){
        int m = data->getRowNum();
        for(int i=0;i<m;++i)
            rowIndex.append(i);
    }
    colIndex.insert(index, n);
    return true;
}

template <typename T>
bool NTable<T>::insertRow(int index, const QVector<T>& row){
    if(!data->appendRow(row))
        return false;
    if(colIndex.isEmpty()){
        int m = data->getColNum();
        for(int i=0;i<m;++i)
            colIndex.append(i);
    }
    int n = data->getRowNum()-1;
    rowIndex.insert(index, n);
    return true;
}

template <typename T>
void NTable<T>::sortByCol(int index){
    tmp = index;
    quickSortRow(rowIndex.begin(),rowIndex.end());
}

template <typename T>
inline void NTable<T>::popFrontColIndex(){
    colIndex.pop_front();
}

template <typename T>
inline void NTable<T>::popFrontRowIndex(){
    rowIndex.pop_front();
}

template <typename T>
void NTable<T>::selectCol(NTable<T> *des, const NTable<T> *src, const QVector<int> &index){
    des->clear();
    des->data = src->data;
    des->rowIndex.append(src->rowIndex);
    for(auto i:index)
        des->colIndex.append(src->colIndex[i]);
}

//private

template <typename T>
NTable<T>::NTable(NTableBlock<T> *data):data(data){
}

template <typename T>
void NTable<T>::quickSortRow(QVector<int>::iterator b, QVector<int>::iterator e){
    if(b==e)
        return;
    auto m = *b;
    auto l = b;
    auto r = e;
    --r;
    while(l!=r){
        while(l!=r&&data[m][tmp]<data[*r][tmp])	--r;
        *l = *r;
        while(l!=r&&data[*l][tmp]<data[m][tmp])	++l;
        *r = *l;
    }
    *l = m;
    ++r;
    quickSortRow(b,l);
    quickSortRow(r,e);
}

#endif // NTABLE_H
