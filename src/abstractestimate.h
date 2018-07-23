#ifndef ABSTRACTESTIMATE_H
#define ABSTRACTESTIMATE_H

#include "header.h"
#include "abstractclassifier.h"

class AbstractEstimate{
private:
    AbstractClassifier *classifier;
    StandardData *trainX;
    StandardData *trainY;
    StandardData *testX;
    StandardData *testY;
public:
    AbstractEstimate();

    void setTrainSet(QString path, int indexY);
    //void setIndexY(const QVector<int>& indexY);
    void setClassifer(AbstractClassifier *classifier);
    //void setMeasure();
    void run();

    static void test();
};


inline void AbstractEstimate::setClassifer(AbstractClassifier *classifier){
    this->classifier = classifier;
}

#endif // ABSTRACTESTIMATE_H
