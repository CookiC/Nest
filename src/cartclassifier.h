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

    class Node;
    QVector<Node> pool;

    double gini(const QMap<double,int> &cate, int num);
    int treeGenerate(const Inst &instX, const Flag &flagX, const Inst &instY, const QMap<double,int> &cate);
};

#endif // CARTCLASSIFIER_H
