#ifndef STRICTDATA_H
#define STRICTDATA_H


class StrictData
{
    enum DataType{NUM,NOM};
public:
    StrictData(int, int);
    ~StrictData();
    int getNumRow();
    int getNumCol();

protected:
    int numRow;
    int numCol;
    double **data;
};

#endif // STRICTDATA_H
