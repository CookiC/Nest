#include "abstractclassifier.h"

AbstractClassifier::AbstractClassifier(){
}

bool AbstractClassifier::compareAttr(StandardData *data){
    //if(memcmp(data->)!=0)
    //    return false;
    return true;
}

void AbstractClassifier::fit(StandardData *data){
    train = data;
    fit();
}
