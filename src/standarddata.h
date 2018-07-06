#ifndef STANDARDDATA_H
#define STANDARDDATA_H

#include "header.h"
#include "abstractdata.h"

class StandardData : public AbstractData<double>{
private:
    bool **missing;
    char *attrCategory;
    QStringVector *nomName;
    uchar **flag;

protected:
    virtual bool loadRow(int i, const QStringVector &row);
    virtual void saveRow(int i, QStringVector& row);

public:
    enum Flag : uchar{
        A,B,C
    };

    StandardData();
    ~StandardData();
    void setMissing(int, int);

    //inline
    void addNomName(int i, const QString &name);
    const char* getAttrCategory();
    bool getMissing(int i,int j);
    bool getNumerical(int i);
    bool isNominal(int i);
    bool isMissing();
    bool isNumerical(int i);
    void setNumerical(int i);
    void setNominal(int i);
    static void test();
};

//public
inline void StandardData::addNomName(int i, const QString &name){
    nomName[i].append(name);
}

inline bool StandardData::getMissing(int i,int j){
    return missing?missing[i][j]:false;
}

inline const char * StandardData::getAttrCategory(){
    return attrCategory;
}

inline bool StandardData::isNominal(int i){
    return !attrCategory[i];
}

inline bool StandardData::isMissing(){
    return  missing;
}

inline bool StandardData::isNumerical(int i){
    return attrCategory[i];
}

inline void StandardData::setNumerical(int i){
    attrCategory[i]=1;
}

inline void StandardData::setNominal(int i){
    attrCategory[i]=0;
}

#endif // STANDARDDATA_H
