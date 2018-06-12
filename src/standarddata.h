#ifndef STANDARDDATA_H
#define STANDARDDATA_H


class StandardData{
    enum DataType{NUM,NOM};
public:
    StandardData(int, int);
    ~StandardData();
    int getNumRow();
    int getNumCol();

private:
    int numRow;
    int numCol;
    DataType *type;
    bool **isMissing;
    double **data;
};

#endif // STANDARDDATA_H
