#include "abstractestimate.h"
#include "cartclassifier.h"

AbstractEstimate::AbstractEstimate():trainX(nullptr),trainY(nullptr){
}

void AbstractEstimate::setTrainSet(QString path, int indexY){
    if(!trainX)
        trainX = new StandardData();
    if(!trainY)
        trainY = new StandardData();
    trainX->loadCsv(path);
    trainY->cutCol(*trainX, indexY);
}

void AbstractEstimate::run(){
    classifier->fit(trainX,trainY);
}

void AbstractEstimate::test(){
    AbstractEstimate est;
    est.setClassifer(new CARTClassifier());
    est.setTrainSet("E:/scientific research/experiment/data/Digit Recognizer/train.csv",0);
    est.run();
}
