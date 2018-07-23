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
    QMap<double, int> cate;
    int n = trainY->getRowNum();
    int i;
    for(i=0;i<n;++i)
        ++cate[trainY->get(i,0)];
    treeGenerate(trainX->getDate(), trainX->getFlag(), trainY->getDate(), cate);
}

NTable<int> CARTClassifier::predictHard(StandardData *data){
    return NTable<int>();
}

NTable<double> CARTClassifier::predictSoft(StandardData *data){
    return NTable<double>();
}

void CARTClassifier::predict(StandardData *data, NTable<int> *hard, NTable<double> *soft){

}

int CARTClassifier::treeGenerate(const Inst &uInstX, const Flag &uFlagX, const Inst &uInstY, const QMap<double, int> &uCate){
/* This function returns the index of new node in the memory pool.
*/
    //Instances are all in the same category.
    if(uCate.size()<=1){
        pool.append(Node(static_cast<int>(uCate.firstKey()),1));
        return pool.size()-1;
    }

    //Instances in the different categories have the same value.

    //SonAttr records different categories in parInst.
    int i,j;
    int n = uInstX.getRowNum();
    int m = uInstX.getColNum();
    QVector<int> attrIndex;
    {
        bool same;
        for(j=0;j<m;++j){
            same=true;
            for(i=0;i<n-1&&same;++i)
                if(uInstX->get(i,j)!=uInstX->get(i,j))
                    same = false;
            if(!same)
                attrIndex.append(j);
        }
    }

    {
        int maxCate=0;
        double maxProb=0;
        double prob;
        for(auto e = uCate.cbegin();e!=uCate.cend();++e){
            prob = e.value()/static_cast<double>(n);
            if(prob>maxProb){
                maxCate = e.key();
                maxProb = prob;
            }
        }
        pool.append(Node(maxCate,maxProb));
        if(!attrIndex.size())
            return pool.size()-1;
    }

    Inst vInstX,lInstX;
    Inst vInstY,lInstY;
    Flag vFlagX,lFlagX;
    Inst::selectCol(&vInstX, &uInstX, attrIndex);
    Inst::copy(&vInstY,&uInstY);
    Flag::selectCol(&vFlagX, &uFlagX, attrIndex);
    n = vInstX.getRowNum();
    m = vInstX.getColNum();
    Node &node = pool.last();
    double tmp;
    double g=0;
    double maxG=0;
    QList<int> lMaxInst;
    QList<int> rMaxInst;
    QMap<double, int> lCate = uCate;
    QMap<double, int> rCate;

    lInstY.sortByCol(0);
    Inst::copyCol(lInstY,lInstX);
    Flag::copyCol(lFlagX,lInstX);
    for(j=0;j<m;++j){
        if(vFlagX.get(0,j)==StandardData::NUM){
            tmp = lInstY.get(0,0);
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

