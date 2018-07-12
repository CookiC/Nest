#ifndef NTABLEMANAGER_H
#define NTABLEMANAGER_H

#include "header.h"
#include "ntable.h"

template <typename T>
class NTableManager{
private:
    QVector<uint> colQuote;
    QVector<uint> rowQuote;
    NTable<T> *table;

public:
    NTableManager();
    NTableManager(int rowNum, int colNum);
    void setTable(NTable<T> *table);
};

template <typename T>
NTableManager<T>::NTableManager(int rowNum, int colNum){

}

template <typename T>
inline void NTableManager<T>::setTable(NTable<T> *table){
    this->table = table;
    int i;
    int n = table->getRowNum();
    int m = table->getColNum();
    rowQuote.resize(n);
    for(i=0;i<n;++i)
        rowQuote[i]=1;
    colQuote.resize(m);
    for(i=0;i<m;++i)
        colQuote[i]=1;
}

#endif // NTABLEMANAGER_H
