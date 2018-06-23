#ifndef STANDARDDATA_H
#define STANDARDDATA_H

#include "header.h"
#include "abstractdata.h"

class StandardData : public AbstractData<double>{
private:
    bool **missing;
    bool *numerical;
    QStringVector *nomName;

protected:
    virtual bool loadRow(int i, const QStringVector &row);
    virtual void saveRow(int i, QStringVector& row);

public:
    StandardData();
    ~StandardData();
    void setMissing(int, int);

    //inline
    void addNomName(int i, const QString &name);
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

inline bool StandardData::getNumerical(int i){
    return numerical[i];
}

inline bool StandardData::isNominal(int i){
    return !numerical[i];
}

inline bool StandardData::isMissing(){
    return  missing;
}

inline bool StandardData::isNumerical(int i){
    return numerical[i];
}

inline void StandardData::setNumerical(int i){
    numerical[i]=1;
}

inline void StandardData::setNominal(int i){
    numerical[i]=0;
}

#endif // STANDARDDATA_H
