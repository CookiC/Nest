﻿#ifndef ABSTRACTDATA_H
#define ABSTRACTDATA_H

#include "header.h"
#include "ntable.h"

template <typename T>
class AbstractData : public NTable<T>{
private:
    QVector<int> colNameCnt;
    QVector<int> rowNameCnt;
    QStringVector colName;
    QStringVector rowName;
    int y;

protected:
    bool colNameFlag;
    bool rowNameFlag;

    void deleteColName(int index);
    void deleteRowName(int index);
    void insertColName(int index, const QString& name);
    void insertRowName(int index, const QString& name);
    QStringVector splitCsvLine(const QByteArray &line, QString& name);

    //inline
    void appendColName(const QString& name);
    void appendRowName(const QString& name);
    const QString& getColName(int i);
    const QString& getRowName(int i);
    virtual bool loadRow(int i, const QStringVector &row) = 0;
    virtual void saveRow(int i, QStringVector& row) = 0;

public:
    AbstractData();

    bool appendCol(const QVector<T>& col, const QString& name);
    bool appendRow(const QVector<T>& col, const QString& name);
    void deleteCol(int index);
    void deleteRow(int index);
    int getColIndex(const QString& name);
    bool insertCol(int index, const QVector<T>& col, const QString& name);
    bool insertRow(int index, const QVector<T>& row, const QString& name);
    bool loadCsv(const QString &path, bool hasColName, bool hasRowName);
    void saveCsv(const QString &);

    //inline
    void setY(int i);
    int getY();
    int getY(int i);
};

//public
template <typename T>
AbstractData<T>::AbstractData(){
    y=0;
}

template <typename T>
bool AbstractData<T>::appendCol(const QVector<T>& col, const QString& name){
    if(!NTable::appendCol(col))
        return false;
    if(colNameFlag)
        colName.append(name);
    return true;
}

template <typename T>
bool AbstractData<T>::appendRow(const QVector<T>& col, const QString& name){
    if(!NTable::appendRow(col))
        return false;
    if(rowNameFlag)
        rowName.append(name);
    return true;
}

template <typename T>
void AbstractData<T>::deleteCol(int index){
    NTable::deleteCol(index);
    deleteColName(index);
}

template <typename T>
void AbstractData<T>::deleteRow(int index){
    NTable::deleteRow(index);
    deleteColName(index);
}

template <typename T>
int AbstractData<T>::getColIndex(const QString& name){
    for(int i=0;i<colNum;++i)
        if(colName[i]==name)
            return i;
    return -1;
}

template <typename T>
bool AbstractData<T>::insertCol(int index, const QVector<T>& col, const QString& name){
    if(!NTable::insertCol(index,col))
        return false;
    if(colNameFlag)
        insertColName(index,name);
    return true;
}

template <typename T>
bool AbstractData<T>::insertRow(int index, const QVector<T>& row, const QString& name){
    if(!NTable::insertRow(index,row))
        return false;
    if(rowNameFlag)
        insertRowName(index,name);
    return true;
}

template <typename T>
bool AbstractData<T>::loadCsv(const QString &path, bool hasColName, bool hasRowName){
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!file.isOpen())
        return false;

    int i;
    QByteArray line;
    QString name;
    QStringVector row;
    colNameFlag = hasColName;
    rowNameFlag = hasRowName;

    if(colNameFlag){
        line = file.readLine();
        row = splitCsvLine(line,name);
        for(i=0;i<row.size();++i)
            colName.append(row[i]);
    }

    deb<<"Colhead load Success!";
    for(i=0;!file.atEnd();++i) {
        line = file.readLine();
        row = splitCsvLine(line,name);
        if(rowNameFlag)
            appendRowName(name);
        if(!loadRow(i,row)){
            deb<<i;
            deb<<row;
            deb<<"Load fail!";
            return false;
        }
    }
    deb<<"Load Success!";
    file.close();
    return true;
}

template <typename T>
void AbstractData<T>::saveCsv(const QString &filePath){
    QFile file(filePath);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    if(!file.isOpen()){
        deb<<"Open fail!";
        return;
    }
    int i,j;
    QTextStream out(&file);
    if(colNameFlag){
        if(rowNameFlag)
            out<<',';
        out<<colName[0];
        for(i=1;i<colNum;++i)
            out<<','<<colName[i];
        out<<'\n';
    }

    QStringVector row;
    for(i=0;i<rowNum;++i){
        if(rowNameFlag)
            out<<rowName[i]<<',';
        saveRow(i,row);
        j=0;
        out<<row[0];
        for(j=1;j<colNum;++j)
            out<<','<<row[j];
        out<<'\n';
    }
    file.close();
}

//public inline

template <typename T>
void AbstractData<T>::setY(int i){
    y = i;
}

template <typename T>
int AbstractData<T>::getY(){
    return y;
}

template <typename T>
int AbstractData<T>::getY(int i){
    return get(i,y);
}

//protect

template <typename T>
void AbstractData<T>::insertColName(int index, const QString& name){
    int i;
    colName.prepend(QString());
    int n = colName.size();
    for(i=n-1;i>index;--i)
        colName[i]=colName[i-1];
    colName[index]=name;
}

template <typename T>
void AbstractData<T>::insertRowName(int index, const QString& name){
    int i;
    rowName.prepend(QString());
    int n = rowName.size();
    for(i=n-1;i>index;--i)
        rowName[i]=rowName[i-1];
    rowName[index]=name;
}

template <typename T>
void AbstractData<T>::deleteColName(int index){
    int n = colName.size();
    for(int i=index;i<n-1;++i)
        colName[i]=colName[i+1];
    colName.removeLast();
}

template <typename T>
void AbstractData<T>::deleteRowName(int index){
    int n = rowName.size();
    for(int i=index;i<n-1;++i)
        rowName[i]=rowName[i+1];
    rowName.removeLast();
}

template <typename T>
QStringVector AbstractData<T>::splitCsvLine(const QByteArray &line, QString& name){
    int i = 0;
    int n = line.size()-1;
    QString t;
    QStringVector v;
    if(rowNameFlag){
        while(line[i]!=','&&i<n){
            if(line[i]=='"')
                for(++i;line[i]!='"'&&i<n;++i)
                    t.append(line[i]);
            else
                t.append(line[i]);
            ++i;
        }
        name = t;
        t.clear();
    }
    while(i<n){
        if(line[i]==','){
            v.push_back(t);
            t.clear();
        }
        else if(line[i]=='"'){
            for(++i;line[i]!='"'&&i<n;++i)
                t.append(line[i]);
        }
        else
            t.append(line[i]);
        ++i;
    }
    return v;
}

template <typename T>
inline void AbstractData<T>::appendColName(const QString& name){
    colName.append(name);
}

template <typename T>
inline void AbstractData<T>::appendRowName(const QString& name){
    rowName.append(name);
}

template <typename T>
const QString& AbstractData<T>::getColName(int i){
    return colName[i];
}

template <typename T>
const QString& AbstractData<T>::getRowName(int i){
    return rowName[i];
}
#endif // ABSTRACTDATA_H