﻿#ifndef ABSTRACTCLASSIFIER_H
#define ABSTRACTCLASSIFIER_H

#include "standarddata.h"

class AbstractClassifier{
public:
    AbstractClassifier();
    void fit(StandardData *trainX,StandardData *trainY);
    virtual NTable<int> predictHard(StandardData *data) = 0;
    virtual NTable<double> predictSoft(StandardData *data) = 0;
    virtual void predict(StandardData *data, NTable<int> *hard, NTable<double> *soft) = 0;

protected:
    StandardData *dataX;
    StandardData *dataY;
    int attrNum;

    virtual void fit() = 0;
    bool compareAttr(StandardData *data);
private:
    char *attrCategory;

};

#endif // ABSTRACTCLASSIFIER_H
