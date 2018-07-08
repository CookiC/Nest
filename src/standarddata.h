#ifndef STANDARDDATA_H
#define STANDARDDATA_H

#include "header.h"
#include "abstractdata.h"

class StandardData : public AbstractData<double>{
public:
    enum Category : uchar{
        MISS = 0x00,
        NOM = 0x01,
        NUM = 0x02,
    };

private:
    QStringVector *nomName;
    Category **flag;

protected:
    void allocateMemory() override;
    bool loadRow(int i, const QStringVector &row) override;
    void saveRow(int i, QStringVector& row) override;
public:


    StandardData();
    ~StandardData();
    void setMissing(int, int);

    //inline
    void addNomName(int i, const QString &name);
    void cutColFrom(StandardData *src, int colIndex);
    void cutColFrom(StandardData *src, QVector<int> &colIndex);
    static void test();
};

//public
inline void StandardData::addNomName(int i, const QString &name){
    nomName[i].append(name);
}


#endif // STANDARDDATA_H
