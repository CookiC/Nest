#include "dynamictable.h"

template<typename T>
DynamicTable<T>::DynamicTable():colNum(0),rowNum(0){
}

template<typename T>
bool DynamicTable<T>::appendCol(const QList<T>& col){
    if(rowNum!=col.size())
        return false;
    if(colReclaim.isEmpty()){
        int i,k;
        colIndex.append(colNum);
        for(k=0;k<rowNum;++k){
            i=rowIndex[k];
            data[i].append(col[k]);
        }
    }
    else{
        int i,j,k;
        colIndex.append(colReclaim.dequeue());
        j=colIndex.last();
        for(k=0;k<rowNum;++k){
            i=rowIndex[k];
            data[i][j]=col[k];
        }
    }
    ++colNum;
    return true;
}

template<typename T>
bool DynamicTable<T>::appendRow(const QList<T>& row){

}

template<typename T>
void DynamicTable<T>::deleteCol(int i){

}

template<typename T>
void DynamicTable<T>::deleteRow(int i){

}

template<typename T>
bool DynamicTable<T>::insertCol(int i, const QList<T>& col){

}

template<typename T>
bool DynamicTable<T>::insertRow(int i, const QList<T>& row){

}
