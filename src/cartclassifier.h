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
    using Flag = NTable<StandardData::Category>;
    using Inst = NTable<double>;

    int attr;
    class Node;
    QVector<Node> pool;

    bool cmp(const int &x,const int &y);
    double gini(const QMap<double,int> &cate, int num);
    int treeGenerate(const Inst &instX, const Flag &flagX, const Inst &instY, const QMap<double,int> &cate);
    void quickSort(QList<int>::iterator b, QList<int>::iterator e);
};

#endif // CARTCLASSIFIER_H
