#ifndef GENERICDATA_H
#define GENERICDATA_H

#include "header.h"
#include "standarddata.h"

class StandardData;

class GenericData{
public:
    GenericData();
    ~GenericData();
    //载入csv文件（文件路径，是否有行名，是否有列名）
    bool loadCsv(QString, bool=0, bool=1);
    //保存csv文件（文件名）
    void saveCsv(QString);
    //添加列（行数据，行名）
    bool appendRow(const QStringList &, const QString& = "");
    void colStrSplit(int, const QString &, bool=false);
    void colStrSplit(int, const QRegularExpression &);
    void colStrSplit(const QString &, const QString&, bool=false);
    void colStrSplit(const QString &, const QRegularExpression &);
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
