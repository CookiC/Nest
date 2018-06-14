#ifndef GENERICDATA_H
#define GENERICDATA_H

#include "header.h"
#include "standarddata.h"

class StandardData;

class GenericData{
public:
    GenericData();
    ~GenericData();
    bool loadCsv(QString, bool=0, bool=1);
    void saveCsv(QString);
    bool appendRow(const QStringList&, const QString& = "");
    void colStrSplit(int, const QString&, bool=false);
    void colStrSplit(const QString&, const QString&, bool=false);
    void deleteRow(int);
    void deleteCol(int);
    bool deleteRow(const QString&);
    bool deleteCol(const QString&);
    bool insertCol(int, const QStringList&, const QString& = "");
    int getColIndex(const QString&);
    StandardData* toStandardData();

    static void test();

private:
    class Node;

    int numRow;
    int numCol;
    bool rowNameFlag;
    bool colNameFlag;
    Node* head;
    QList<Node*> rowHead;
    QList<Node*> colHead;

    static void deleteNode(Node*);

    void appendColHead(const QString& = "");
    void appendRowHead(const QString& = "");
    void deleteColHead(int);
    void deleteRowHead(int);
    void InsertColHead(int, const QString& = "");

};

#endif // GENERICDATA_H
