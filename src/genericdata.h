#ifndef GENERICDATA_H
#define GENERICDATA_H

#include "header.h"
#include "standarddata.h"

class StandardData;

class GenericData{
public:
    GenericData();
    ~GenericData();

    bool appendRow(const QStringList &);
    void colStrSplit(const QString &, const QRegularExpression &);
    void colStrSplit(const QString &, const QString&, bool=false);
    void colStrSplit(int, const QRegularExpression &);
    void colStrSplit(int, const QString &, bool=false);
    void deleteRow(int);
    bool deleteRow(const QString&);
    void deleteCol(int);
    bool deleteCol(const QString&);
    int getColIndex(const QString&);
    bool insertCol(int, const QStringList&, const QString& = "");
    bool loadCsv(QString, bool=1);
    void saveCsv(QString);
    StandardData* toStandardData();

    static void test();

private:
    class Node;

    int numRow;
    int numCol;
    bool colNameFlag;
    Node* head;
    QList<Node*> rowHead;
    QList<Node*> colHead;
    //对列名相同的进行计数并重命名（未决定是否开发）
    //QMap<QString> rowNameCnt;
    //QMap<QString> colNameCnt;

    static void deleteNode(Node*);

    void appendColHead(const QString& = "");
    void appendRowHead(const QString& = "");
    void deleteColHead(int);
    void deleteRowHead(int);
    void InsertColHead(int, const QString& = "");

};

#endif // GENERICDATA_H
