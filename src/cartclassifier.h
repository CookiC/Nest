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
    class Node;
    QVector<int> pool;
    void treeGenerate(int o, const QVector<int> instSet, const QVector<int> attrSet);
};

#endif // CARTCLASSIFIER_H
