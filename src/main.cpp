#include "cpython.h"
#include "genericdata.h"
#include "standarddata.h"
#include "nimage.cpp"
#include "abstractestimate.h"

int main(){
    deb<<"Programm start!";

//  StandardData::test();
//    NImage::test();
    AbstractEstimate::test();
    deb << "Program end!";
    return 0;
}
