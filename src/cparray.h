#ifndef CPARRAY_H
#define CPARRAY_H

#include "header.h"

class CPArray{
public:
    int n;
    long long* dim;
    void* data;

    CPArray();
    CPArray(int, long long*, void*);
};


#endif // CPARRAY_H
