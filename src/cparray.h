#ifdef RELEASE
#ifndef CPARRAY_H
#define CPARRAY_H

#include "header.h"

class CPArray{
public:
    int n;
    ll* dim;
    void* data;

    CPArray();
    CPArray(int, ll*, void*);
};


#endif // CPARRAY_H
#endif
