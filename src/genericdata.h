#ifndef GENERICDATA_H
#define GENERICDATA_H

#include "header.h"
#include "standarddata.h"

using GenericList = QVector<string>;

class GenericData{
public:
    friend class StandardData;

    GenericData();
    ~GenericData();
    bool loadCsv(string, bool=0, bool=1);
    void saveCsv(string);
    bool appendRow(const GenericList&, const string& = "");
    void colStrSplit(int, const string&, bool=false);
    void colStrSplit(const string&, const string&, bool=false);
    void deleteRow(int);
    void deleteCol(int);
    bool deleteRow(const string&);
    bool deleteCol(const string&);
    bool insertCol(int, const GenericList&, const string& = "");
    int getColIndex(const string&);
    StandardData& toStandardData();
    static void test();

private:
    class Node;

    int numRow;
    int numCol;
    bool rowNameFlag;
    bool colNameFlag;
    Node* head;
    QVector<Node*> rowHead;
    QVector<Node*> colHead;

    static void deleteNode(Node*);

    void appendColHead(const string& = "");
    void appendRowHead(const string& = "");
    void deleteColHead(int);
    void deleteRowHead(int);
    void InsertColHead(int, const string& = "");

};

#endif // GENERICDATA_H
