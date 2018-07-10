﻿#ifndef ABSTRACTESTIMATE_H
#define ABSTRACTESTIMATE_H

#include "header.h"
#include "abstractclassifier.h"

class AbstractEstimate{
private:
    AbstractClassifier *classifier;
    StandardData *trainX;
    StandardData *trainY;

public:
    AbstractEstimate();

    void setTrainSet(QString path, int indexY);
    //void setIndexY(const QVector<int>& indexY);
    void setClassifer(AbstractClassifier *classifier);
    //void setMeasure();
    void run();
};

#endif // ABSTRACTESTIMATE_H