#include "cartclassifier.h"

class CARTClassifier::Node{
public:
    int left;
    int right;
    int attrIndex;
    double splite;
    double probability;
    int category;
    int (*next)(double*);

    Node(){}

    Node(int category,double probability):
        category(category),probability(probability),left(-1),right(-1),next(nullptr){}

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

bool CARTClassifier::cmp(const int &x,const int &y){
    return train->get(x,attr)<train->get(y,attr);
}

int CARTClassifier::treeGenerate(const QVector<int> &pInst, const QVector<int> &pAttr, const QMap<double,int> &pCate){
    if(pCate.size()<=1){
        pool.append(Node(static_cast<int>(pCate.firstKey()),1));
        return pool.size()-1;
    }

    bool same;
    QVector<int> sAttr;
    int i,j;
    for(j=0;j<pAttr.size();++j){
        same=true;
        for(i=0;i<pInst.size()-1&&same;++i)
            if(train->get(pInst[i],pAttr[j])!=train->get(pInst[i+1],pAttr[j]))
                same = false;
        if(!same)
            sAttr.append(pAttr[i]);
    }
    double n = pInst.size();
    int maxCate=0;
    double maxProb=0;
    double prob;
    for(auto e = pCate.cbegin();e!=pCate.cend();++e){
        prob = e.value()/n;
        if(prob>maxProb){
            maxCate = e.key();
            maxProb = prob;
        }
    }
    pool.append(Node(maxCate,maxProb));
    if(!sAttr.size())
        return pool.size()-1;

    Node &node = pool.last();
    double tmp;
    double g=0;
    double maxG=0;
    QList<int> lInst = pInst.toList();
    QList<int> rInst;
    QList<int> lMaxInst;
    QList<int> rMaxInst;
    QMap<double, int> lCate;
    QMap<double, int> rCate;
    bool (CARTClassifier::*f)(const int &, const int &);
    f = &CARTClassifier::cmp;
    for(auto J:pAttr){
        attr = J;
        //qSort(lInst.begin(),lInst.end(),f);
        for(auto e:lInst)
            if(lCate.contains(e))
                ++lCate[e];
            else
                lCate[e]=1;

        tmp = train->getY(lInst[0]);
        do{
            for(auto e:rInst){
                lInst.append(e);
                ++lCate[e];
            }
            rInst.clear();
            rCate.clear();
            do{
                int I = lInst[0];
                rInst.append(I);
                ++rCate[train->getY(I)];
                --lCate[train->getY(I)];
                lInst.removeFirst();
            }while(train->getY(lInst[0])==train->getY(lInst[1]));
            g = gini(lCate,lInst.size());
            if(g>maxG){
                maxG = g;
                node.attrIndex = attr;
                node.splite = rInst[0];
                lMaxInst = lInst;
                rMaxInst = rInst;
            }
        }while(tmp != lInst[0]);
    }
    return 1;
}

double CARTClassifier::gini(const QMap<double,int> &cate, int num){
    double res = 1;
    double n = num;
    for(auto e:cate)
        res -= e/n;
    return res;
}
