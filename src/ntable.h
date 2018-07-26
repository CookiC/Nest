#ifndef NTABLE_H
#define NTABLE_H

#include "header.h"
#include "ntableblock.h"

template <typename T>
class NTable;

namespace NTableFunc {
template <typename T>
void copy(NTable<T> &des, const NTable<T> &src);
template <typename T, typename U>
void copyCol(NTable<T> &des, const NTable<U> &src);
template <typename T, typename U>
void copyRow(NTable<T> &des, const NTable<U> &src);
}

template <typename T>
class NTable{
private:
    QList<int> colIndex;
    QList<int> rowIndex;
    NTableBlock<T> *data;
    int tmp;

    NTable(NTableBlock<T> *data);
    void deleteQuote();
    void setData(NTableBlock<T> *data);
    void quickSortRow(QList<int>::iterator begin, QList<int>::iterator end);

public:
    NTable();
    NTable(int rowNum, int colNum);
    ~NTable();

    bool appendCol(const QVector<T>& col);
    void appendCol(const NTable<T>& src);
    void appendCol(const NTable<T>& src, int index);
    void appendCol(const NTable<T>& src, int position, int n);
    bool appendRow(const QVector<T>& row);
    void appendRow(const NTable<T>& src);
    void appendRow(const NTable<T>& src, int index);
    void appendRow(const NTable<T>& src, int position, int n);
    T& at(int i, int j);
    void clear();
    void clearCol();
    void clearRow();
    void cutCol(NTable<T> &src, int index);
    void cutRow(NTable<T> &src, int index);
    void deleteCol(int index);
    void deleteRow(int index);
    const T& get(int i,int j) const;
    int getColNum() const;
    QVector<T> getRow(int index);
    int getRowNum() const;
    bool insertCol(int index, const QVector<T>& col);
    bool insertRow(int index, const QVector<T>& row);
    void popFrontCol();
    void popFrontRow();
    void selectCol(const NTable<T> &src, const QVector<int> &index);
    void selectRow(const NTable<T> &src, const QVector<int> &index);
    void sortByCol(int index);

    template <typename T>
    friend void NTableFunc::copy(NTable<T> &des, const NTable<T> &src);
    template <typename T, typename U>
    friend void NTableFunc::copyCol(NTable<T> &des, const NTable<U> &src);
    template <typename T, typename U>
    friend void NTableFunc::copyRow(NTable<T> &des, const NTable<U> &src);
};

//public

template <typename T>
NTable<T>::NTable(){
    data = new NTableBlock<T>();
    data->addQuote();
}

template <typename T>
NTable<T>::NTable(int rowNum, int colNum){
    data = new NTableBlock<T>(rowNum, colNum);
    data->addQuote();
    colIndex.reserve(colNum);
    rowIndex.reserve(rowNum);
}

template <typename T>
NTable<T>::~NTable(){
    deleteQuote();
}

template <typename T>
inline bool NTable<T>::appendCol(const QVector<T>& col){
    return insertCol(col.size(),col);
}

template <typename T>
inline void NTable<T>::appendCol(const NTable<T>& src){
    colIndex.append(src.colIndex);
}

template <typename T>
inline void NTable<T>::appendCol(const NTable<T>& src, int index){
    colIndex.append(src.colIndex[index]);
}

template <typename T>
void NTable<T>::appendCol(const NTable<T>& src, int pos, int n){
    if(n<0)
        colIndex.append(src.colIndex);
    else
        for(int i=0;i<n;++i)
            colIndex.append(src.colIndex[pos+i]);
}

template <typename T>
inline bool NTable<T>::appendRow(const QVector<T>& row){
    return insertRow(row.size(),row);
}

template <typename T>
inline void NTable<T>::appendRow(const NTable<T>& src){
    rowIndex.append(src.rowIndex);
}

template <typename T>
inline void NTable<T>::appendRow(const NTable<T>& src, int index){
    rowIndex.append(src.rowIndex[index]);
}

template <typename T>
void NTable<T>::appendRow(const NTable<T>& src, int pos, int n){
    if(n<0)
        rowIndex.append(src.rowIndex);
    else
        for(int i=0;i<n;++i)
                rowIndex.append(src.rowIndex[pos+i]);
}

template <typename T>
inline T& NTable<T>::at(int i, int j){
    return data->at(i,j);
}

template <typename T>
inline void NTable<T>::clear(){
   rowIndex.clear();
   colIndex.clear();
}

template <typename T>
inline void NTable<T>::clearCol(){
   colIndex.clear();
}

template <typename T>
inline void NTable<T>::clearRow(){
   rowIndex.clear();
}

template <typename T>
void NTable<T>::cutCol(NTable<T> &src,int index){
    rowIndex.clear();
    rowIndex.append(src.rowIndex);
    colIndex.append(src.colIndex[index]);
    src.colIndex.removeOne(index);
    setData(src.data);
}

template <typename T>
void NTable<T>::cutRow(NTable<T> &src,int index){
    colIndex.clear();
    colIndex.append(src.colIndex);
    rowIndex[0] = src.rowIndex[index];
    src.rowIndex.removeOne(index);
    setData(src.data);
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
inline int NTable<T>::getColNum() const{
    return colIndex.size();
}

template <typename T>
inline QVector<T> NTable<T>::getRow(int index){
    return data->getRow(index);
}

template <typename T>
inline int NTable<T>::getRowNum() const{
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
void NTable<T>::setData(NTableBlock<T> *data){
    deleteQuote();
    this->data = data;
    data->addQuote();
}

template <typename T>
void NTable<T>::sortByCol(int index){
    tmp = index;
    quickSortRow(rowIndex.begin(),rowIndex.end());
}

template <typename T>
inline void NTable<T>::popFrontCol(){
    colIndex.pop_front();
}

template <typename T>
inline void NTable<T>::popFrontRow(){
    rowIndex.pop_front();
}

template <typename T>
void NTable<T>::selectCol(const NTable<T> &src, const QVector<int> &index){
    setData(src.data);
    rowIndex.append(src.rowIndex);
    for(auto i:index)
        colIndex.append(src.colIndex[i]);
}

template <typename T>
void NTable<T>::selectRow(const NTable<T> &src, const QVector<int> &index){
    setData(src.data);
    colIndex.append(src.colIndex);
    for(auto i:index)
        rowIndex.append(src.rowIndex[i]);
}

namespace NTableFunc {

template <typename T>
void copy(NTable<T> &des, const NTable<T> &src){
    des.colIndex.clear();
    des.rowIndex.clear();
    des.setData(src.data);
    des.colIndex.append(src.colIndex);
    des.rowIndex.append(src.rowIndex);
}

template <typename T, typename U>
void copyCol(NTable<T> &des, const NTable<U> &src){
    des.colIndex.clear();
    des.colIndex.append(src.colIndex);
}

template <typename T, typename U>
void copyRow(NTable<T> &des, const NTable<U> &src){
    des.rowIndex.clear();
    des.rowIndex.append(src.rowIndex);
}
}

//private

template <typename T>
NTable<T>::NTable(NTableBlock<T> *data):data(data){
}

template <typename T>
inline void NTable<T>::deleteQuote(){
    data->deleteQuote();
    if(!data->getQuote())
        delete data;
}

template <typename T>
void NTable<T>::quickSortRow(QList<int>::iterator b, QList<int>::iterator e){
    if(b==e)
        return;
    auto m = *b;
    auto l = b;
    auto r = e;
    --r;
    while(l!=r){
        while(l!=r&&data->get(m,tmp)<=data->get(*r,tmp))     --r;
        *l = *r;
        while(l!=r&&data->get(*l,tmp)<data->get(m,tmp))    ++l;
        *r = *l;
    }
    *l = m;
    ++r;
    quickSortRow(b,l);
    quickSortRow(r,e);
}

#endif // NTABLE_H
