#ifndef ABSTRACTTABLE_H
#define ABSTRACTTABLE_H

#include "header.h"

template <typename T>
class AbstractTable{
protected:
    int colNum;
    int rowNum;
    T **data;

public:
    void getColNum();
    void getRowNum();
    void deleteCol(int index);
    void deleteRow(int index);
};

template <typename T>
inline int AbstractTable<T>::getColNum(){
    return colNum;
}

template <typename T>
inline int AbstractTable<T>::getRowNum(){
    return rowNum;
}

template<typename T>
void AbstractTable<T>::deleteCol(int index){
    --colNum;
    for(int i=0;i<rowNum;++i)
        memmove(data[i]+index,data[i]+index+1,sizeof(T)*(colNum-index-1));
}

template<typename T>
void AbstractTable<T>::deleteRow(int index){
    int i;
    --rowNum;
    for(i=index;i<rowNum;++i)
        memmove(data+i,data+i+1,sizeof(T)*colNum);
}
#endif // ABSTRACTTABLE_H
