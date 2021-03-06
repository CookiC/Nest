﻿#include "cartclassifier.h"

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
                if(uInstX.get(i,j)!=uInstX.get(i+1,j))
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

    Inst lInstX,rInstX;
    Inst lInstY,rInstY;
    Flag lFlagX,rFlagX;
    lInstX.selectCol(uInstX, attrIndex);
    NTableFunc::copy(lInstY,uInstY);
    lFlagX.selectCol(uFlagX, attrIndex);
    n = lInstX.getRowNum();
    m = lInstX.getColNum();
    Node &node = pool.last();
    double tmp;
    double g=0;
    double maxG=0;
    QList<int> lMaxInst;
    QList<int> rMaxInst;
    QMap<double, int> lCate = uCate;
    QMap<double, int> rCate;

    lInstY.sortByCol(0);
    NTableFunc::copyCol(lInstX,lInstY);
    NTableFunc::copyCol(lFlagX,lInstY);
    for(j=0;j<m;++j){
        if(lFlagX.get(0,j)==StandardData::NUM){
            lInstX.sortByCol(j);
            NTableFunc::copyCol(lInstY,lInstX);
            NTableFunc::copyCol(lFlagX,lInstX);
            while(lInstX.getRowNum()>1){
                tmp = lInstX.get(0,j);
                while(tmp == lInstX.get(0,j)){
                    ++rCate[lInstY.get(0,0)];
                    rInstX.appendCol(lInstX, 0);
                    --lCate[lInstY.get(0,0)];
                    lInstX.popFrontCol();
                    lInstY.popFrontCol();
                }

            }
            lInstX.appendCol(rInstX);
            /*
            do{
                for(i=0;lInstY.get(i,0)==lInstY.get(0,0);++i)
                    ++l
                rInstX.appendRow(rInstX, 0, i);
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
    */
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

