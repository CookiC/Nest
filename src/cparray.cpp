#include "cparray.h"

CPArray::CPArray():n(0),dim(nullptr),data(nullptr){
}

CPArray::CPArray(int n, ll* dim, void *data):n(n),dim(dim),data(data){
}
