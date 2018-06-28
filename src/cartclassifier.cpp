#include "cartclassifier.h"

class CARTClassifier::Node{
    int left;
    int right;
    int attrIndex;
    double splite;

    Node(int left, int right, int attrIndex = 0, double splite = 0):
        left(left),right(right),attrIndex(attrIndex),splite(splite){}

    int nextNom(double *inst){
        if(inst[attrIndex]==splite)
            return left;
        return right;
    }

    int nextNum(double *inst){
        if(inst[attrIndex]<splite)
            return left;
        return right;
    }
};

CARTClassifier::CARTClassifier(){
}

void CARTClassifier::fit(){

}

NTable<int> CARTClassifier::predictHard(StandardData *data){
    return NTable<int>();
}

NTable<double> CARTClassifier::predictSoft(StandardData *data){
    return NTable<double>();
}

void CARTClassifier::predict(StandardData *data, NTable<int> *hard, NTable<double> *soft){

}

void CARTClassifier::treeGenerate(int o, const QVector<int> instSet, const QVector<int> attrSet){
    bool same=true;
    int i,j,k,l;
    for(i=0;i<instSet.size()-1&&same;++i){
        for(j=0;j<attrSet.size()&&same;++j)
            if(train->get(instSet[i],attrSet[j])!=train->get(instSet[i+1],attrSet[j]))
                same = false;
    }
    if(same)
        pool.append(Node(-1,-1));

}
