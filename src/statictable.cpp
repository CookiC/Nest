#include "statictable.h"

template <typename T>
StaticTable<T>::StaticTable(int rowNum, int colNum):
    rowNum(rowNum),colNum(colNum){
    data = new T*[rowNum];
    for(int i=0;i<rowNum;++i)
        data[i] = new T[colNum];
}

template <typename T>
StaticTable<T>::~StaticTable(){
    for(int i=0;i<rowNum;++i)
        delete[] data[i];
    delete[] data;
}
