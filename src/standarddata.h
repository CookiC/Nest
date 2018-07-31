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
    NTable<Category> flag;

protected:
    bool loadRow(int i, const QStringVector &row) override;
    void saveRow(int i, QStringVector& row) override;
public:
    StandardData(int rowNum = 0, int colNum = 0);
    ~StandardData();

    //inline
    void addNomName(int i, const QString &name);
    StandardData* cutColFrom(const QVector<int> &colIndex);
    const NTable<Category>& getFlag();
    void setMissing(int, int);
    void cutCol(StandardData &src, int index);

    static void test();
};

//public
inline void StandardData::addNomName(int i, const QString &name){
    nomName[i].append(name);
}

inline const NTable<StandardData::Category>& StandardData::getFlag(){
    return flag;
}

#endif // STANDARDDATA_H
