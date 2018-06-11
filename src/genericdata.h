#ifndef GENERICDATA_H
#define GENERICDATA_H

#include "header.h"

using GenericList = vector<string>;

class GenericData{
public:
    GenericData();
    ~GenericData();
    bool loadCsv(string, bool=0, bool=1);
    void saveCsv(string);
    bool appendRow(const GenericList&, const string="");
    void deleteRow(int);
    static void test();

private:
    class Node;

    int numRow;
    int numCol;
    bool rowNameFlag;
    bool colNameFlag;
    Node* head;
    vector<Node*> rowHead;
    vector<Node*> colHead;

    static void deleteNode(Node*);
    void appendRowHead(const string="");
    void deleteRowHead(int);
    void appendColHead(const string="");

};

#endif // GENERICDATA_H
