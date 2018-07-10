#include "abstractestimate.h"

AbstractEstimate::AbstractEstimate(){
}

void AbstractEstimate::setClassifer(AbstractClassifier *classifier){
    this->classifier = classifier;
}

void AbstractEstimate::setTrainSet(QString path, int indexY){
    if(!trainX)
        trainX = new StandardData();
    if(!trainY)
        trainY = new StandardData();
    trainX->loadCsv(path);
    trainY->cutColFrom(trainX,indexY);
}

void AbstractEstimate::run(){
    classifier->fit(trainX,trainY);
}
