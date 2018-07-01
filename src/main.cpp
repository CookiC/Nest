#include "cpython.h"
#include "genericdata.h"
#include "standarddata.h"
#include "nimage.cpp"

int main(int argc, char *argv[]){
    deb<<"Programm start!";

//  StandardData::test();
    NImage::test();

    deb << "Program end!";
    return 0;
}
