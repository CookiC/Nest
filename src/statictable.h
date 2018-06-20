#ifndef STATICTABLE_H
#define STATICTABLE_H


template <typename T>
class StaticTable{
private:
    int colNum;
    int rowNum;
    T **data;

public:
    StaticTable(int, int);
    ~StaticTable();

    inline int getColNum(){
        return colNum;
    }

    inline int getRowNum(){
        return rowNum;
    }

    inline T& at(int i,int j){
        return data[i][j];
    }

    inline const T& at(int i,int j) const{
        return data[i][j];
    }

    inline T* operator[](int i){
        return data[i];
    }
};

#endif // STATICTABLE_H
