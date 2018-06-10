#ifndef GENERICDATA_H
#define GENERICDATA_H

#include "header.h"

class GenericData{
public:
    GenericData();
    ~GenericData();
    bool loadCsv(string, bool=0, bool=1);
    void saveCsv(string);
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
    void rowAppend(const string="");
    void colAppend(const string="");
};

#endif // GENERICDATA_H
