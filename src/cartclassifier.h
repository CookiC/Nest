#ifndef CARTCLASSIFIER_H
#define CARTCLASSIFIER_H

#include "abstractclassifier.h"
#include "header.h"

class CARTClassifier : public AbstractClassifier{
public:
    CARTClassifier();
    NTable<int> predictHard(StandardData *data);
    NTable<double> predictSoft(StandardData *data);
    void predict(StandardData *data, NTable<int> *hard, NTable<double> *soft);

protected:
    void fit();

private:
    int attr;
    class Node;
    QVector<Node> pool;

    bool cmp(const int &x,const int &y);
    double gini(const QMap<double,int> &cate, int num);
    int CARTClassifier::treeGenerate(const QVector<int> &pInst, const QVector<int> &pAttr, const QMap<double,int> &pCate);
};

#endif // CARTCLASSIFIER_H
