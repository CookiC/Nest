#ifndef NTABLE_H
#define NTABLE_H

#include "header.h"

template <typename T>
class NTable{
private:
    QStack<int> colReclaim;
    QStack<int> rowReclaim;
    QVector<int> colIndex;
    QVector<int> rowIndex;
    QVector<QVector<T>> data;


protected:
    int colNum;
    int rowNum;

public:
    NTable();

    void deleteCol(int i);
    void deleteRow(int i);
    bool insertCol(int i, const QVector<T>& col);
    bool insertRow(int i, const QVector<T>& row);
    void print();

    //inline
    bool appendCol(const QVector<T>& col);
    bool appendRow(const QVector<T>& row);
    T& at(int i,int j);
    const T& get(int i,int j) const;
    int getColNum();
    int getRowNum();
    void set(int i,int j,const T& value);
};

template<typename T>
NTable<T>::NTable(){
}

template<typename T>
void NTable<T>::deleteCol(int index){
    colReclaim.push(colIndex[index]);
    --colNum;
    for(index=0;index<colNum;++index)
        colIndex[index]=colIndex[index+1];
}

template<typename T>
void NTable<T>::deleteRow(int index){
    rowReclaim.push(rowIndex[index]);
    --rowNum;
    for(index=0;index<rowNum;++index)
        rowIndex[index]=rowIndex[index+1];
}

template<typename T>
bool NTable<T>::insertCol(int index, const QVector<T>& col){
    int i;
    if(!rowNum){
        rowNum=col.size();
        for(i=0;i<rowNum;++i)
            rowIndex.append(i);
    }
    if(index>colNum||rowNum!=col.size())
        return false;
    int j,k;
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
bool NTable<T>::insertRow(int index, const QVector<T>& row){
    int i;
    if(!colNum){
        colNum=row.size();
        for(i=0;i<colNum;++i)
            colIndex.append(i);
    }
    if(index>rowNum||colNum!=row.size())
        return false;
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

template<typename T>
void NTable<T>::print(){
    int i,j;
    for(i=0;i<rowNum;++i){
        for(j=0;j<colNum;++j){
            if(j)
                deb<<',';
            deb<<get(i,j);
        }
        deb<<'\n';
    }
}


template<typename T>
inline bool NTable<T>::appendCol(const QVector<T>& col){
    return insertCol(colNum,col);
}

template<typename T>
inline bool NTable<T>::appendRow(const QVector<T>& row){
    return insertRow(rowNum,row);
}

template<typename T>
inline T& NTable<T>::at(int i,int j){
    return data[rowIndex[i]][colIndex[j]];
}

template<typename T>
inline const T& NTable<T>::get(int i,int j) const{
    return data[rowIndex[i]][colIndex[j]];
}

template<typename T>
inline int NTable<T>::getColNum(){
    return colNum;
}

template<typename T>
inline int NTable<T>::getRowNum(){
    return rowNum;
}

template<typename T>
inline void NTable<T>::set(int i,int j,const T& value){
    data[rowIndex[i]][colIndex[j]] = value;
}
#endif // NTABLE_H
