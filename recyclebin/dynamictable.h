#ifndef DYNAMICTABLE_H
#define DYNAMICTABLE_H

#include "abstracttable.h"

template <typename T>
class DynamicTable : public AbstractTable<T>{
    void deleteCol(int index) override;
    void deleteRow(int index) override;
};

template<typename T>
void DynamicTable<T>::deleteCol(int index){
    for(int i=0;i<rowNum;++i)
        memmove(data[i]+index,data[i]+index+1,sizeof(T)*(colNum-index-1));
    --colNum;
}

template<typename T>
void DynamicTable<T>::deleteRow(int index){
    for(int i=index;i<rowNum;++i)
        memmove(data+i,data+i+1,sizeof(T)*colNum);
    --rowNum;
}
#endif // DYNAMICTABLE_H
