#include "dynamictable.h"

template<typename T>
DynamicTable<T>::DynamicTable():colNum(0),rowNum(0){
}

template<typename T>
void DynamicTable<T>::deleteCol(int index){
    colReclaim.push(colIndex[index]);
    --colNum;
    for(index=0;index<colNum;++index)
        colIndex[index]=colIndex[index+1];
}

template<typename T>
void DynamicTable<T>::deleteRow(int index){
    rowReclaim.push(rowIndex[index]);
    --rowNum;
    for(index=0;index<rowNum;++index)
        rowIndex[index]=rowIndex[index+1];
}

template<typename T>
bool DynamicTable<T>::insertCol(int index, const QVector<T>& col){
    if(index>colNum||rowNum!=col.size())
        return false;
    int i,j,k;
    if(colReclaim.isEmpty()){
        colIndex.insert(index,colNum);
        for(k=0;k<rowNum;++k){
            i=rowIndex[k];
            data[i].append(col[k]);
        }
    }
    else{
        colIndex.insert(index,colReclaim.pop());
        j=colIndex[index];
        for(k=0;k<rowNum;++k){
            i=rowIndex[k];
            data[i][j]=col[k];
        }
    }
    ++colNum;
    return true;
}

template<typename T>
bool DynamicTable<T>::insertRow(int index, const QVector<T>& row){
    if(index>rowNum||colNum!=row.size())
        return false;
    int i;
    if(rowReclaim.isEmpty()){
        rowIndex.insert(index,rowNum);
        data.append(row);
    }
    else{
        rowIndex.insert(index,rowReclaim.pop());
        i=rowIndex[index];
        data[i]=row;
    }
    ++rowNum;
    return true;
}
