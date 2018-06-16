#include "cpython.h"
#include "genericdata.h"

int main(int argc, char *argv[]){
    GenericData::test();
    cout<<sizeof(void*)<<endl;
    return 0;
}
